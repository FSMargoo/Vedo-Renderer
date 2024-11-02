# Vedo Render

Vedo Render is a path tracer render. Vedo Render based on Skia, gifting Vedo a ability of 2D rendering.

Vedo Render is a toy project for me, it maybe used to reproduce some paper or for some graphics experiments.

## How Vedo Render Come Through the SKSL problems?

Vedo Render is based on Skia, however, Skia's native shader language is not strong enough for render developing, so I modified it and apply some features to the SKSL.

I modified the source code of Skia (Version M101) by adding a `MarcoManager` in SKSL parser and let it support a Marco define operation like:

```GLSL
#define fs(i) (fract(sin((i)*114.514)*1919.810))
#alias MOD3 (vec3(.16532,.17369,.15787))
```

Command `#define` is for the general Marco function define. `#alias` command is for the alias of a token. 

Since SKSL doesn't provide a natural variable length array uniform support, I added a `Vedo::Shader` as a wrapper of SKSL. `Vedo::Shader` accept a variant of SKSL I call it "Vedo Shader".

Vedo Shader will process the shader code first, by using `Vedo::Shader::Link` method to bind a constant value with shader. Shader being written in Vedo Shader will be like: 

```GLSL
const int length = $Length$;

uniform float arrayInput[length];

half4 main(vec2 coord) {
    float array[length];
    for (int i = 0; i < length; ++i) {
        array[i] = 0;
    }
    for (int i = 0; i < length; ++i) {
        array[i] += arrayInput[i];
    }

    return half4(array[0], array[1], array[2], array[3]);
}
```

In C++ code, you need to use `Link` method like this:

```C++
auto shader = Vedo::Shader::MakeFromFile("./shader.sksl");
shader->Link("Length", 2);
// Get Skia effect object
auto effect = shader->MakeEffect();
```

In summary, the working flow of the Vedo Shader:

<image src="./readme/SKSL-flow.svg" height="20%"></image>

By applying this work flow, Vedo Render successfully come through the shortcomings of Skia and SKSL.