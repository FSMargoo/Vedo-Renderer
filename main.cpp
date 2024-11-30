#include <include/render/VeCamera.h>
#include <include/render/VeObject.h>

#include <glfw/glfw3.h>

sk_sp<const GrGLInterface> GLInterface;
sk_sp<SkShader> Shader;

#define WIDTH  640
#define HEIGHT 480

GLFWwindow *GLWindow;

/**
 * Init OpenGL interface object
 */
void InitGLInterface();
/**
 * Create GLFW window
 */
void InitWindow();
/**
 * Init Skia resource
 */
void InitResource();
/**
 * The frame buffer call back function
 */
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height);
/**
 * The error call back function of GLFW
 * @param Error The error code
 * @param Description The description string of the error
 */
void ErrorCallBack(int Error, const char *Description);
/**
 * Call Skia API to draw context
 */
void Draw(int Width, int Height);

int main() {
    Vedo::Camera camera;

    camera.Ratio = 256.f / 192.f;
    camera.Width = WIDTH;
    camera.SPP = 200;
    camera.Depth = 50;

    camera.FOV = 90;
    camera.LookFrom = Vedo::Vec3(13, 2, 3);
    camera.LookAt = Vedo::Vec3(0, 0, 0);
    camera.VUP = Vedo::Vec3(0, 1, 0);

    camera.DeFocusAngle = 0.6;
    camera.FocusDistance = 10.f;

    camera.Init();

    Vedo::Object sphere;
    sphere.Center = Vedo::Vec3(0, 0, 0);
    sphere.Radius = 7.5f;
    sphere.Material = Vedo::MetalMaterial;
    sphere.Shape = Vedo::SphereGeometry;
    sphere.Albedo = Vedo::Vec3(43.f / 255.f, 45.f / 255.f, 48.f / 255.f);
    sphere.Fuzz = 2.f;

    try {
        std::vector<Vedo::IShaderStructureUniform *> cameraUniform = {&camera};
        std::vector<Vedo::IShaderStructureUniform *> objectUniform = {&sphere};

        auto shader = Vedo::Shader::MakeFromFile("../shaders/path_tracing.sksl");

        shader->BindUniform("u_seed", time(nullptr));
        shader->BindUniform("u_SPP", int(camera.SPP));
        shader->BindUniform("u_Depth", int(camera.Depth));
        shader->BindUniform("u_ObjectCount", objectUniform.size());
        shader->BindUniformArray("u_camera", cameraUniform);
        shader->BindUniformArray("u_object", objectUniform);

        auto code = shader->MakeCode();
        SkRuntimeEffect::Result effectResult = SkRuntimeEffect::MakeForShader(SkString(code));
        sk_sp<SkRuntimeEffect> runtimeEffect = effectResult.effect;
        Shader = runtimeEffect->makeShader(nullptr, {});

        // Create an offscreen surface
        const int width = 512, height = 512;

    	InitWindow();
    	InitResource();
    	InitGLInterface();

    	Draw(WIDTH, HEIGHT);
    	while (!glfwWindowShouldClose(GLWindow)) {
    		glfwPollEvents();
    	}
    } catch (std::exception &e) {
        printf("Error occurred: %s.", e.what());

        exit(-1);
    }

    return 0;
}

void InitGLInterface() {
	GLInterface = GrGLMakeNativeInterface();
}
void InitWindow() {
	glfwSetErrorCallback(ErrorCallBack);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	GLWindow = glfwCreateWindow(WIDTH, HEIGHT, "Skia Tester", nullptr, nullptr);
	if (!GLWindow) {
		printf("Failed to create GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetFramebufferSizeCallback(GLWindow, FrameBufferCallBack);

	glfwMakeContextCurrent(GLWindow);
}
void InitResource() {

}
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height) {
	Draw(Width, Height);
}
void Draw(int Width, int Height) {
	GrBackendRenderTarget glRenderTarget = {Width, Height, 0, 0, GrGLFramebufferInfo{.fFBOID = 0, .fFormat = GL_RGBA8}};
	sk_sp<GrDirectContext> glContext = GrDirectContext::MakeGL(GLInterface);
	SkColorType	   colorType = kRGBA_8888_SkColorType;
	SkSurfaceProps property(SkSurfaceProps::Flags::kDynamicMSAA_Flag, SkPixelGeometry::kUnknown_SkPixelGeometry);

	// In fact, we don't need to proc the render target destroying
	auto glSurface =
		SkSurface::MakeFromBackendRenderTarget(glContext.get(), glRenderTarget,
											   kBottomLeft_GrSurfaceOrigin, colorType, nullptr, &property);

	auto canvas = glSurface->getCanvas();
	canvas->clear(SK_ColorBLACK);
	SkPaint paint;
	paint.setShader(Shader);
	canvas->drawPaint(paint);

	canvas->flush();
	glContext->flushAndSubmit();

	glfwSwapBuffers(GLWindow);
}
void ErrorCallBack(int Error, const char *Description) {
	fputs(Description, stderr);
}