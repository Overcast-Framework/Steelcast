workspace "Steelcast"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    startproject "Steelcast"

-- Define the main project
project "Steelcast"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files { "src/**.cpp", "src/**.h", "include/**.h" }
    includedirs { "third_party/SDL/include", "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um", "include",  "third_party/glm/"}
    
    libdirs { "third_party/SDL/VisualC/x64/Release", os.findlib("d3d11.lib"), os.findlib("dxgi.lib"), os.findlib("d3dcompiler.lib") }
    links { "SDL3", "d3d11", "d3dcompiler", "dxgi" }

    postbuildcommands {
        -- Copy the DLL to the output directory
        '{COPY} third_party/SDL/VisualC/x64/Release/SDL3.dll "%{cfg.targetdir}/"'
    }

    filter "system:windows"
    	defines { "SDL_MAIN_HANDLED" }

    -- Build configuration for your main project
    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
