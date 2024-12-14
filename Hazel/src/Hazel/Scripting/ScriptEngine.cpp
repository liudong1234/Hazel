#include "hzpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/attrdefs.h"
#include "ScriptGlue.h"

namespace Hazel
{
	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap = 
	{
		{"System.Single", ScriptFieldType::Float},
		{"System.Double", ScriptFieldType::Double},
		{"System.Boolean", ScriptFieldType::Bool},
		{"System.Byte", ScriptFieldType::Byte},
		{"System.Char", ScriptFieldType::Char},
		{"System.Int16", ScriptFieldType::Short},
		{"System.Int32", ScriptFieldType::Int},
		{"System.Int64", ScriptFieldType::Long},
		{"System.UInt16", ScriptFieldType::UShort},
		{"System.UInt32", ScriptFieldType::UInt},
		{"System.UInt64", ScriptFieldType::ULong},
		{"Hazel.Vector2", ScriptFieldType::Vector2},
		{"Hazel.Vector3", ScriptFieldType::Vector3},
		{"Hazel.Vector4", ScriptFieldType::Vector4},
		{"Hazel.Entity", ScriptFieldType::Entity},

	};

	namespace Utils
	{
		//将文件加载到字节数组
		char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
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
		MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
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

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
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
			MonoClass* entityClass = mono_class_from_name(image, "Hazel", "Entity");

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				HZ_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}
	
		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			const char* typeName = mono_type_get_name(monoType);
			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				HZ_CORE_WARN("Unknown type: {}", typeName);
				return ScriptFieldType::None;
			}
			else
				return it->second;		
		}

		std::string FieldTypeToString(ScriptFieldType fileType)
		{
			switch (fileType)
			{
				case ScriptFieldType::None: return "None";
				case ScriptFieldType::Float: return "Float";
				case ScriptFieldType::Double: return "Double";
				case ScriptFieldType::Int: return "Int";
				case ScriptFieldType::Long: return "Long";
				case ScriptFieldType::Bool: return "Bool";
				case ScriptFieldType::Byte: return "Byte";
				case ScriptFieldType::Char: return "Char";
				case ScriptFieldType::Short: return "Short";
				case ScriptFieldType::UShort: return "UShort";
				case ScriptFieldType::UInt: return "UInt";
				case ScriptFieldType::ULong: return "ULong";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				default: return "<Invalid>";
			}
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass EntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;

		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;


	void ScriptEngine::Init()
    {
		s_Data = new ScriptEngineData();
		
		InitMono();

		LoadAssembly("Resources/Scripts/Hazel-ScriptCore.dll");
		LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");
		LoadAssemblyClasses();
	
		auto& classes = s_Data->EntityClasses;
		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		s_Data->EntityClass = ScriptClass("Hazel", "Entity");

#if 0
		//1.
		MonoObject* instance = s_Data->EntityClass.Instantiate();
		//2.示例1
		MonoMethod* printMessageFun = s_Data->EntityClass.GetMethod("PrintMessage", 0);
		s_Data->EntityClass.InvokeMethod(printMessageFun, instance);
		//示例2
		MonoMethod* printInts = s_Data->EntityClass.GetMethod("PrintInts", 2);
		int value1 = 100;
		int value2 = 200;
		void* params[2] = {
			&value1,
			&value2
		};
		s_Data->EntityClass.InvokeMethod(printInts, instance, params);
		//示例3
		MonoMethod* printString = s_Data->EntityClass.GetMethod("PrintMessage", 1);
		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello world from c++");
		void* param = monoString;
		s_Data->EntityClass.InvokeMethod(printString, instance, &param);
		/*
		//检索和实例化类
		MonoClass* monoClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Hazel", "Main");
		//1.create a object
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		//2. call Method
		MonoMethod* printMessageFun = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageFun, instance, nullptr, nullptr);

		// call function with printInts
		MonoMethod* printInts = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		int value1 = 100;
		int value2 = 200;
		void* params[2] = {
			&value1,
			&value2
		};
		mono_runtime_invoke(printInts, instance, params, nullptr);

		//call function with string parameter
		MonoMethod* PrintMessageFunParam = mono_class_get_method_from_name(monoClass, "PrintMessage", 1);
		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello world from c++");

		//MonoString*  monoString = mono_string_new_wrapper("hello hazel");
		void* param = monoString;
		mono_runtime_invoke(PrintMessageFunParam, instance, &param, nullptr);
		*/
#endif

    }

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("HazelScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_Data->AppAssembly);

	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}
	
	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;

		s_Data->EntityInstances.clear();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName))
		{
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity);
			s_Data->EntityInstances[entity.GetUUID()] = instance;
			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, TimeStep ts)
	{
		UUID entityUUID = entity.GetUUID();
		HZ_CORE_ASSERT(s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end());
		Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
		instance->InvokeOnUpdate(ts);
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto& it = s_Data->EntityInstances.find(entityID);
		if (it == s_Data->EntityInstances.end())
			return nullptr;
		else
			return it->second;
	}

	void ScriptEngine::InitMono()
    {
		mono_set_assemblies_path("mono/lib");
		
		MonoDomain* rootDomain = mono_jit_init("HazelJITRuntime");

		HZ_CORE_ASSERT(rootDomain);

		s_Data->RootDomain = rootDomain;
		
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Hazel", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
				
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);
			if (entityClass == monoClass)
				continue;
			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
			s_Data->EntityClasses[fullName] = scriptClass;

			HZ_CORE_TRACE("{}.{}", nameSpace, className);

			int num = mono_class_num_fields(monoClass);
			HZ_CORE_TRACE("{} has {} fields", className, num);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flag = mono_field_get_flags(field);
				if (flag & MONO_FIELD_ATTR_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fileType = Utils::MonoTypeToScriptFieldType(type);
					std::string typeName = Utils::FieldTypeToString(fileType);
					scriptClass->m_Fields[fieldName] = {fileType, typeName, field};
					HZ_CORE_TRACE(" {} - {}", fieldName, typeName);
				}

			}

		}

		//mono_field_get_value(monoobje)

	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}
    
	void ScriptEngine::Shutdown()
    {
		ShutdownMono();
		delete s_Data;
    }
	
	void ScriptEngine::ShutdownMono()
	{
		//mono_domain_unload(s_Data->AppDomain);
		//mono_jit_cleanup(s_Data->RootDomain);

		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}



	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) :
		m_ClassNamespace(classNamespace), m_ClassName(m_ClassName), m_MonoClass(nullptr)
	{
		m_MonoClass = mono_class_from_name(s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& functionName, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, functionName.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoMethod* momoMethod, MonoObject* instance, void** params)
	{
		return mono_runtime_invoke(momoMethod, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity& entity):
		m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();
		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_onCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		//call Entity Constructor
		{
			UUID entityID = entity.GetUUID();
			void* param = &entityID;
			m_ScriptClass->InvokeMethod(m_Constructor, m_Instance, &param);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_onCreateMethod)
			m_ScriptClass->InvokeMethod(m_onCreateMethod, m_Instance);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_OnUpdateMethod, m_Instance, &param);
		}

	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* fieldValue)
	{
		const auto& fields = this->m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& sfield = it->second;

		mono_field_get_value(m_Instance, sfield.ClassField, fieldValue);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, void* value)
	{
		const auto& fields = this->m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& sfield = it->second;
		mono_field_set_value(m_Instance, sfield.ClassField, value);

		return true;
	}

}
