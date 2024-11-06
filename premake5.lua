workspace "Mint"
    configurations { "Debug", "Release", "Dist" }
    architecture "x64"  -- Only 64-bit support
    startproject "Editor"  -- Start with the Editor project

    cppdialect "C++20"
    cdialect "C17"
    toolset "v143"  -- Use the appropriate toolset for Visual Studio 2022

    -- Define the global settings
    staticruntime "On"
    characterset ("MBCS")

    targetdir ("Build/%{cfg.buildcfg}/Binaries")
    objdir ("Build/%{cfg.buildcfg}/ObjectFiles")

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "configurations:Dist"
        defines { "NDEBUG", "DIST" }
        optimize "Full"

    filter {}  -- Clear filters for the following project definitions

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"

    pchheader "EnginePCH.h"  -- Define the PCH header file
    pchsource "Engine/Source/EnginePCH.cpp"  -- Source file for the PCH
    forceincludes { "EnginePCH.h" }  -- Include the PCH in all source files

    files { "Engine/Source/**.h", "Engine/Source/**.cpp", "Engine/Source/**.c" }
    includedirs { "Engine/Source", "ThirdParty/Include" }

project "ImGui"
    location "ThirdParty/ImGui"
    kind "StaticLib"
    language "C++"

    files { "ThirdParty/ImGui/*.h", "ThirdParty/ImGui/*.cpp", "ThirdParty/ImGui/backends/imgui_impl_dx11.*", "ThirdParty/ImGui/backends/imgui_impl_win32.*" }
    includedirs { "ThirdParty/ImGui" }

project "Editor"
    location "Editor"
    kind "WindowedApp"
    language "C++"

    pchheader "EditorPCH.h"  -- Define the PCH header file
    pchsource "Editor/Source/EditorPCH.cpp"  -- Source file for the PCH
    forceincludes { "EditorPCH.h" }  -- Include the PCH in all source files

    buildoptions { "/utf-8" }  -- UTF-8 because spdlog wants it

    files { "Editor/Source/**.h", "Editor/Source/**.cpp", "Editor/Source/**.c" }
    includedirs { "Editor/Source", "ThirdParty/spdlog/include" }

    links { "Engine", "ImGui" }

    libdirs { "ThirdParty/Libs" }
