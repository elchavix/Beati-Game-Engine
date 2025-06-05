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

include "Beati/vendor/GLFW/premake5.lua"

project "Beati"  
	location "Beati"  
	kind "SharedLib"  
	language "C++"  
	cppdialect "C++17"  

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
		"%{includeDir.GLFW}"
	}  

	buildoptions { "/utf-8" }

	links  
	{  
		"GLFW",  
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"  
		cppdialect "C++17"  
		staticruntime "On"  
		systemversion "latest"  

		defines  
		{  
			"BE_PLATFORM_WINDOWS",  
			"BE_BUILD_DLL"  
		}  

		postbuildcommands  
		{  
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")  
		}  

	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		symbols "On"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		optimize "On"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }  
		optimize "On"  


project "Sandbox"  
	location "Sandbox"  
	kind "ConsoleApp"  
	language "C++"  
	cppdialect "C++17"  

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
		staticruntime "On"  
		systemversion "latest"  

		defines  
		{  
			"BE_PLATFORM_WINDOWS"  
		}  


	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		symbols "On"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		optimize "On"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }  
		optimize "On"