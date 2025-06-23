workspace "Beati"  
	architecture "x64"  

	configurations  
	{  
		"Debug",  
		"Release",  
		"Dist"  
	}  

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"  

includeDir = {}
includeDir["spdlog"] = "Beati/vendor/spdlog/include"
includeDir["GLFW"] = "Beati/vendor/GLFW/include"
includeDir["Glad"] = "Beati/vendor/Glad/include"
includeDir["ImGui"] = "Beati/vendor/imgui"

include "Beati/vendor/GLFW"
include "Beati/vendor/Glad"
include "Beati/vendor/imgui"

project "Beati"  
	location "Beati"  
	kind "SharedLib"  
	language "C++"  
	cppdialect "C++17"  
	staticruntime "off"

	targetdir ("bin/".. outputdir .. "/%{prj.name}")  
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")  

	pchheader "bepch.h"
	pchsource "Beati/src/bepch.cpp"

	files  
	{  
		"%{prj.name}/src/**.h",  
		"%{prj.name}/src/**.cpp"
	}  

	includedirs  
	{  
		"%{prj.name}/src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}"
	}  

	buildoptions { "/utf-8" }

	links  
	{  
		"GLFW",  
		"Glad",  
		"ImGui",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"  
		cppdialect "C++17"  
		systemversion "latest"  

		defines  
		{  
			"BE_PLATFORM_WINDOWS",  
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}  

		postbuildcommands  
		{  

			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")  
		}  

	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		runtime "Debug"
		symbols "On"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		runtime "Release"
		optimize "On"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }  
		runtime "Release"
		optimize "On"  


project "Sandbox"  
	location "Sandbox"  
	kind "ConsoleApp"  
	language "C++"  
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/".. outputdir .. "/%{prj.name}")  
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")  

	files  
	{  
		"%{prj.name}/src/**.h",  
		"%{prj.name}/src/**.cpp",  
	}  

	includedirs  
	{  
		"Beati/src",
		"Beati/vendor/spdlog/include"  
	}  

	buildoptions { "/utf-8" }

	links  
	{  
		"Beati"  
	}

	filter "system:windows"  
		cppdialect "C++17"  
		systemversion "latest"  

		defines  
		{  
			"BE_PLATFORM_WINDOWS"  
		}  


	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		runtime "Debug"
		symbols "On"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		runtime "Release"
		optimize "On"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }
		runtime "Release"
		optimize "On"
