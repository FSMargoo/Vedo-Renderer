cmake_minimum_required(VERSION 3.29)
project(VedoRenderer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty/glfw)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(MSVC_USE_MT_RUNTIME_LIBRARY OFF)
    set(MSVC_USE_MD_RUNTIME_LIBRARY OFF)
    set(MSVC_USE_MTd_RUNTIME_LIBRARY OFF)
    set(MSVC_USE_MDd_RUNTIME_LIBRARY ON)

    message("Vedo Building under Windows")
elseif()
    message("Vedo Building under Linux/Unix")
endif()

add_library(libvedo STATIC main.cpp
        include/shader/VeShader.h
        source/shader/VeShader.cpp
        include/skia/VeSkia.h
        include/VeBase.h
        include/math/VeVector.h
        source/math/VeVector.cpp
        include/render/VeCamera.h
        source/render/VeCamera.cpp
        include/render/VeRender.h
        source/render/VeRender.cpp
        thirdparty/stb_c_lexer/stb_c_lexer.h
        thirdparty/stb_c_lexer/stb_c_lexer.cpp
        include/render/VeObject.h)

target_include_directories(libvedo PUBLIC ./include)
target_include_directories(libvedo PUBLIC ./)
target_include_directories(libvedo PUBLIC ./thirdparty)
target_include_directories(libvedo PUBLIC ./thirdparty/SkiaM101Binary)
target_include_directories(libvedo PUBLIC ./thirdparty/glad/include)
target_include_directories(libvedo PUBLIC ./thirdparty/OpenString-CMake)

# Judge MSVC compile runtime
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message("Vedo Build under Windows OS")
    message("Vedo Using MSVC/Clang-CL Compiler")

    # MSVC use different runtime in Debug(D/S) and Release(D/S), make them happy :)
    if (MSVC_USE_MT_RUNTIME_LIBRARY)
        target_link_directories(libvedo PUBLIC libs/skia/win/MT)
        message("Vedo (Skia Library) : Using MSVC MT Runtime")
        add_compile_options(
                $<$<CONFIG:>:/MT>
                $<$<CONFIG:Debug>:/MT>
                $<$<CONFIG:Release>:/MT>
        )
    endif()
    if (MSVC_USE_MD_RUNTIME_LIBRARY)
        target_link_directories(libvedo PUBLIC libs/skia/win/MD)
        message("Vedo (Skia Library) : Using MSVC MD Runtime")
        add_compile_options(
                $<$<CONFIG:>:/MD>
                $<$<CONFIG:Debug>:/MD>
                $<$<CONFIG:Release>:/MD>
        )
    endif()
    if (MSVC_USE_MTd_RUNTIME_LIBRARY)
        target_link_directories(libvedo PUBLIC libs/skia/win/MTd)
        message("Vedo (Skia Library) : Using MSVC MTd Runtime")
        add_compile_options(
                $<$<CONFIG:>:/MTd>
                $<$<CONFIG:Debug>:/MTd>
                $<$<CONFIG:Release>:/MTd>
        )
    endif()
    if (MSVC_USE_MDd_RUNTIME_LIBRARY)
        target_link_directories(libvedo PUBLIC libs/skia/win/MDd)
        message("Vedo (Skia Library) : Using MSVC MDd Runtime")
        add_compile_options(
                $<$<CONFIG:>:/MDd>
                $<$<CONFIG:Debug>:/MDd>
                $<$<CONFIG:Release>:/MDd>
        )
    endif()
endif()

# The behaviour of other OS will be written after VUILib3 was done on Windows
if(WIN32)
    message("Link Skia Under Windows OS")
    target_link_libraries(libvedo PUBLIC
            expat
            harfbuzz
            icu
            imgui
            libjpeg
            libmicrohttpd
            libpng
            libwebp
            libwebp_sse41
            nanobench
            particles
            pathkit
            skia
            skiaserve
            skottie
            skparagraph
            skresources
            sksg
            skshaper
            sktext
            skunicode
            svg
            zlib
            opengl32
            glfw
    )
else()
    message("Link Skia Under Linux/UNix")
    target_link_directories(libvedo PUBLIC ./libs/skia)
    target_link_libraries(libvedo PUBLIC
            expat
            harfbuzz
            icu
            imgui
            jpeg
            microhttpd
            png
            webp
            webp_sse41
            particles
            pathkit
            skia
            skottie
            skparagraph
            skresources
            sksg
            skshaper
            sktext
            fontconfig
            skunicode
            svg
            freetype
            zlib
            GL
            GLX
            glfw
    )
endif()

add_executable(vedoTestScene main.cpp)

add_executable(vedoTestShader tests/VeShaderTest/main.cpp)

target_link_libraries(vedoTestShader PRIVATE libvedo)
target_include_directories(vedoTestShader PRIVATE ./include)
target_include_directories(vedoTestShader PRIVATE ./)
target_include_directories(vedoTestShader PRIVATE ./thirdparty)
target_include_directories(vedoTestShader PRIVATE ./thirdparty/SkiaM101Binary)
target_include_directories(vedoTestShader PRIVATE ./thirdparty/glad/include)
target_include_directories(vedoTestShader PRIVATE ./thirdparty/OpenString-CMake)

target_link_libraries(vedoTestScene PRIVATE libvedo)
target_include_directories(vedoTestScene PRIVATE ./include)
target_include_directories(vedoTestScene PRIVATE ./)
target_include_directories(vedoTestScene PRIVATE ./thirdparty)
target_include_directories(vedoTestScene PRIVATE ./thirdparty/SkiaM101Binary)
target_include_directories(vedoTestScene PRIVATE ./thirdparty/glad/include)
target_include_directories(vedoTestScene PRIVATE ./thirdparty/OpenString-CMake)