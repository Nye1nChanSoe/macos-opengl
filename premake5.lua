workspace "macos-opengl"
    configurations { "Debug", "Release" }
    platforms { "x64", "arm64" }
    location "build" -- premake generate files

project "macos-opengl"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    systemversion "15.1.1"

    -- to be compiled and linked
    files { 
        "src/**.cpp", 
        "assets/shaders/**.glsl"
    }

    -- include headers
    includedirs { 
        "/opt/homebrew/include"
    }

    -- Library directories and linking
    -- TODO: dynamic lib linking
    libdirs { "/opt/homebrew/lib" }
    links { "GLFW", "OpenGL.framework" }

    -- To include /opt/homebrew/include in the system header search paths
    filter "system:macosx"
        externalincludedirs { "/opt/homebrew/include" }