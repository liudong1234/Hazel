project "Assimp"
    kind "StaticLib"
    language "C"
    staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/*.h",
        "include/*.hpp",
        "src/*",
        "contrib/*"
    }

    includedirs
    {
        "include",
        "src"
    }

    filter "system:windows"
        systemversion "10.0"
        

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"