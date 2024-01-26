project "Assimp"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "code/AssetLib/**/**.cpp",
        "code/AssetLib/**/**.h",
        "code/Common/**.h",
        "code/Common/**.cpp",
        "code/CApi/CInterfaceIOWrapper.h",
        "code/CApi/CInterfaceIOWrapper.cpp",
        "include/assimp/Compiler/**.h",
        "contrib/clipper/**.cpp",
        "contrib/clipper/**.hpp",
        "contrib/Open3DGC/**.h",
        "contrib/Open3DGC/**.cpp",
        "contrib/Open3DGC/**.inl",
        "contrib/openddlparser/code/**.cpp",
        "contrib/openddlparser/include/openddlparser/**.h",
        "contrib/poly2tri/poly2tri/common/**.cc",
        "contrib/poly2tri/poly2tri/common/**.h",
        "contrib/poly2tri/poly2tri/sweep/**.cc",
        "contrib/poly2tri/poly2tri/sweep/**.h",
        "contrib/pugixml/src/**.hpp",
        "contrib/pugixml/src/**.cpp",
        "contrib/stb/stb_image.h",
        "contrib/unzip/**.h",
        "contrib/unzip/**.c",
        "contrib/zip/src/**.h",
        "contrib/zip/src/**.c",
        "contrib/zlib/**.c",
        "contrib/zlib/**.h",
        "contrib/utf8cpp/source/utf8/**.h",
        "contrib/utf8cpp/source/**.h",
        "code/CApi/AssimpCExport.cpp",
        "code/Common/Exporter.cpp",
        "code/Geometry/**.cpp",
        "code/Geometry/**.h",
        "include/assimp/**.h",
        "include/assimp/**.cpp",
        "code/Common/DefaultLogger.cpp",
        "code/Common/FileLogStream.h",
        "code/Common/StdOStreamLogStream.h",
        "code/Common/Win32DebugLogStream.h",
        "code/Material/**.h",
        "code/Material/**.cpp",
        "code/PostProcessing/**.cpp",
        "code/PostProcessing/**.h",
        "code/Common/Assimp.cpp",
        "code/Pbrt/**.cpp",
        "code/Pbrt/**.h"
    }

    removefiles 
    { 
        "code/AssetLib/IFC/IFCReaderGen_4.cpp", 
        "code/AssetLib/IFC/IFCReaderGen_4.h", 
        "contrib/zlib/contrib/**/**"
    }

    includedirs
    {
        "include",
        "include/assimp",
        "code",
        "contrib",
        "contrib/zlib",
        "contrib/pugixml/src",
        "contrib/rapidjson/include",
        "contrib/openddlparser/include",
        "contrib/unzip"
    }

    filter "system:windows"
        systemversion "10.0"
        buildoptions { "/wd4344" }
        flags { "MultiProcessorCompile" }  -- 开启多处理编译

        defines
        {
            "WIN32",
            "_WINDOWS",
            "NDEBUG",
            "ASSIMP_BUILD_DLL_EXPORT",
            "ASSIMP_BUILD_NO_M3D_IMPORTER",
            "ASSIMP_BUILD_NO_M3D_EXPORTER",
            "WIN32_LEAN_AND_MEAN",
            "UNICODE",
            "_UNICODE",
            "ASSIMP_BUILD_NO_C4D_IMPORTER",
            "MINIZ_USE_UNALIGNED_LOADS_AND_STORES=0",
            "ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC=1",
            "RAPIDJSON_HAS_STDSTRING=1",
            "RAPIDJSON_NOMEMBERITERATORCLASS",
            "_SCL_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_WARNINGS",
            "OPENDDLPARSER_BUILD",
            "assimp_EXPORTS",
        }
        

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        buildoptions { "/bigobj" }
        warnings "Off"  -- 关闭将警告视为错误

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        warnings "Off"  -- 关闭将警告视为错误