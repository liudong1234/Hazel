#include "hzpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
namespace Hazel 
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	//将文件加载到字节数组
	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		HZ_CORE_ASSERT(stream, "Failed to open the file");
		//if (!stream)
		//{
		//	// Failed to open the file
		//	return nullptr;
		//}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}
	
	//加载 C# 程序集
	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		//HZ_CORE_ASSERT(status == MONO_IMAGE_OK, mono_image_strerror(status));
		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			// Log some error message using the errorMessage data
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			HZ_CORE_TRACE("{}.{}\n", nameSpace, name);
		}
	}

	void ScriptEngine::Init()
    {
		s_Data = new ScriptEngineData();
		InitMono();
    }
    
	void ScriptEngine::Shutdown()
    {
		ShutdownMono();
		delete s_Data;
    }
    
	void ScriptEngine::InitMono()
    {
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("HazelJITRuntime");

		HZ_CORE_ASSERT(rootDomain);

		s_Data->RootDomain = rootDomain;
		s_Data->AppDomain = mono_domain_create_appdomain("HazelScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Hazel-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly); 

		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "Hazel", "Main");
		
		//1.create a object
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		
		//2. call function
		MonoMethod* printMessageFun =  mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageFun, instance, nullptr, nullptr);


		//(1)printints
		MonoMethod* printInts = mono_class_get_method_from_name(monoClass, "PrintInts", 2);

		int value1 = 100;
		int value2 = 200;

		void* params[2] = {
			&value1,
			&value2
		};
		mono_runtime_invoke(printInts, instance, params, nullptr);


		MonoMethod* PrintMessageFunParam = mono_class_get_method_from_name(monoClass, "PrintMessage", 1);
		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello world from c++");

		/*MonoString*  monoString = mono_string_new_wrapper("hello hazel");*/
		void* param = monoString;
		mono_runtime_invoke(PrintMessageFunParam, instance, &param, nullptr);


    }
	void ScriptEngine::ShutdownMono()
	{
		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}






}
