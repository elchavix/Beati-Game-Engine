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
includeDir["Box2D"] = "Beati/vendor/Box2D/include"
includeDir["GLFW"] = "Beati/vendor/GLFW/include"
includeDir["Glad"] = "Beati/vendor/Glad/include"
includeDir["ImGui"] = "Beati/vendor/imgui"
includeDir["glm"] = "Beati/vendor/glm"
includeDir["stb_image"] = "Beati/vendor/stb_image"
includeDir["entt"] = "Beati/vendor/entt/include"

include "Beati/vendor/Box2D"
include "Beati/vendor/GLFW"
include "Beati/vendor/Glad"
include "Beati/vendor/imgui"

project "Beati"  
	location "Beati"  
	kind "StaticLib"  
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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/glm/glm/**.h"
	}  

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
	}

	includedirs  
	{  
		"%{prj.name}/src",
		"%{includeDir.spdlog}",
		"%{includeDir.Box2D}",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}",
	}  

	buildoptions { "/utf-8" }

	links  
	{  
		"Box2D",
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
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}  

	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		runtime "Debug"
		symbols "on"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		runtime "Release"
		optimize "on"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }  
		runtime "Release"
		optimize "on"  


-- Sandbox Project
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
		"Beati/vendor/spdlog/include",
		"Beati/src",
		"Beati/vendor",
		"%{includeDir.glm}",
		"%{includeDir.entt}",
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
			
		}  


	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		runtime "Debug"
		symbols "on"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		runtime "Release"
		optimize "on"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }
		runtime "Release"
		optimize "on"


-- Mondongo - Entt Usage Example
project "Mondongo" 
	location "Mondongo"  
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
		"Beati/vendor/spdlog/include",
		"Beati/src",
		"Beati/vendor",
		"%{includeDir.glm}",
		"%{includeDir.entt}",
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
			
		}  


	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		runtime "Debug"
		symbols "on"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		runtime "Release"
		optimize "on"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }
		runtime "Release"
		optimize "on"

-- Physics - Simple Physics Example
project "Physics"  
	location "Physics"  
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
		"Beati/vendor/spdlog/include",
		"Beati/src",
		"Beati/vendor",
		"%{includeDir.glm}",
		"%{includeDir.entt}",
		"%{includeDir.Box2D}",

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
			
		}  


	filter "configurations:Debug"  
		defines { "BE_DEBUG" }  
		runtime "Debug"
		symbols "on"  

	filter "configurations:Release"  
		defines { "BE_RELEASE" }  
		runtime "Release"
		optimize "on"  

	filter "configurations:Dist"  
		defines { "BE_DIST" }
		runtime "Release"
		optimize "on"