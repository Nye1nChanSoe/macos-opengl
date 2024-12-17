workspace "macos-opengl"
    configurations { "Debug", "Release" }
    platforms { "x64", "arm64" }
    location "build" -- premake generate files

project "macos-opengl"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"

    -- Set minimum macOS deployment target
    xcodebuildsettings {
        ["MACOSX_DEPLOYMENT_TARGET"] = "15.1"
    }

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

    filter "system:macosx"
        externalincludedirs { "/opt/homebrew/include" } -- Xcode-specific include path for GLFW

    -- filter "configurations:Debug"
    --     symbols "On"

    -- filter "configurations:Release"
    --     optimize "On"
