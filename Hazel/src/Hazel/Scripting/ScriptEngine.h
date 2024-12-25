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
		UShort, UInt, ULong,
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
			return *(T*)m_FieldValue;
		}
		template<typename T>
		void SetValue(T& value)
		{
			static_assert(sizeof(T) <= 8, "Type is large!");
			if constexpr (sizeof(T) <= 8)
				memcpy(m_FieldValue, &value, sizeof(T));
			else
				static_assert(false, "Type too large!");
		}

		//const char* GetBuffer() const { return m_FieldValue; }
	private:
		uint8_t m_FieldValue[8];

		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className);

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
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);


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
			bool success = SetFieldValueInternal(name, &value);
			if (success == false)
				HZ_CORE_ERROR("Not Exist {}", name);
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
	static char s_FieldValue[8];
}
