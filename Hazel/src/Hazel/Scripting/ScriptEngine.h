#pragma once
#include <filesystem>
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"
extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Hazel
{
	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Byte, Char, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity,
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;

	};

	//ScriptField + Data storage
	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			const char temp = '\0';
			memcpy(m_FieldValue, &temp, sizeof(m_FieldValue));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type is large!");
			return *(T*)m_FieldValue;
		}
		template<typename T>
		void SetValue(T& value)
		{
			static_assert(sizeof(T) <= 16, "Type is large!");
			memcpy(m_FieldValue, &value, sizeof(T));
		}

		//const char* GetBuffer() const { return m_FieldValue; }
	private:
		uint8_t m_FieldValue[16];

		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();

		MonoMethod* GetMethod(const std::string& functionName, int parameterCount);
		MonoObject* InvokeMethod(MonoMethod* momoMethod, MonoObject* instance, void** params = nullptr);

		const std::map<std::string, ScriptField>& GetFields() { return m_Fields; }

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass;

		std::map<std::string, ScriptField> m_Fields;

		friend class ScriptEngine;
	};

	class ScriptInstance;
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, TimeStep ts);

		static bool EntityClassExists(const std::string& fullClassName);
		static Scene* GetSceneContext();
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static Ref<ScriptClass> GetEntityClass(std::string&);
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID);
		static ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity);

		static MonoObject* GetManagedInstance(UUID uuid);
		static MonoImage* GetCoreAssemblyImage();
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance() = default;
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity& entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }
		MonoObject* GetManagedObject() { return m_Instance; }
		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			bool success = GetFieldValueInternal(name, s_FieldValue);
			if (success == false)
				return T();
			return *(T*)s_FieldValue;
		}

		//TODO
		template<typename T>
		void SetFieldValue(const std::string& name, T& value)
		{
			static_assert(sizeof(T) <= 16, "Type is too large");
			SetFieldValueInternal(name, &value);
		}
		
	private:
		bool GetFieldValueInternal(const std::string& name, void* fieldValue);
		bool SetFieldValueInternal(const std::string& name, void* value);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor= nullptr;
		MonoMethod* m_onCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		friend struct ScriptFieldInstance;
		friend class ScriptEngine;
	};
	static char s_FieldValue[16];


	namespace Utils {
		inline char* ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
				case ScriptFieldType::None:    return "None";
				case ScriptFieldType::Float:   return "Float";
				case ScriptFieldType::Double:  return "Double";
				case ScriptFieldType::Bool:    return "Bool";
				case ScriptFieldType::Byte:    return "Byte";
				case ScriptFieldType::Char:    return "Char";
				case ScriptFieldType::Short:   return "Short";
				case ScriptFieldType::Int:     return "Int";
				case ScriptFieldType::Long:    return "Long";
				case ScriptFieldType::UByte:    return "UByte";
				case ScriptFieldType::UShort:  return "UShort";
				case ScriptFieldType::UInt:    return "UInt";
				case ScriptFieldType::ULong:   return "ULong";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				case ScriptFieldType::Entity:  return "Entity";
			}
			HZ_CORE_ASSERT(false, "Unknown FieldType");
			return "None";
		}
		
		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view typeStr)
	{
		if (typeStr == "None")    return ScriptFieldType::None;
		if (typeStr == "Float")   return ScriptFieldType::Float;
		if (typeStr == "Double")  return ScriptFieldType::Double;
		if (typeStr == "Bool")    return ScriptFieldType::Bool;
		if (typeStr == "Byte")    return ScriptFieldType::Byte;
		if (typeStr == "Char")    return ScriptFieldType::Char;
		if (typeStr == "Short")   return ScriptFieldType::Short;
		if (typeStr == "Int")     return ScriptFieldType::Int;
		if (typeStr == "Long")    return ScriptFieldType::Long;
		if (typeStr == "UByte")    return ScriptFieldType::UByte;
		if (typeStr == "UShort")  return ScriptFieldType::UShort;
		if (typeStr == "UInt")    return ScriptFieldType::UInt;
		if (typeStr == "ULong")   return ScriptFieldType::ULong;
		if (typeStr == "Vector2") return ScriptFieldType::Vector2;
		if (typeStr == "Vector3") return ScriptFieldType::Vector3;
		if (typeStr == "Vector4") return ScriptFieldType::Vector4;
		if (typeStr == "Entity")  return ScriptFieldType::Entity;

		HZ_CORE_ASSERT(false, "Unknown ScriptFieldType");
		return ScriptFieldType::None;

	}
	}


}
