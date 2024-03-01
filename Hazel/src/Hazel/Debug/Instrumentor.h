#pragma once
#include <iostream>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <string>
#include <thread>

namespace Hazel
{
	struct ProfileResult
	{
		std::string name;
		long long start, end;
		uint32_t threadId;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutStream;
		int m_ProfileCount;
	public:
		Instrumentor():
			m_CurrentSession(nullptr),
			m_ProfileCount(0)
		{}

		void BeginSession(const std::string& name, const std::string& filepath = "result.json")
		{
			this->m_OutStream.open(filepath);
			this->WriteHeader();
			this->m_CurrentSession = new InstrumentationSession({ name });
		}

		void WriteHeader()
		{
			this->m_OutStream << "{\"otherDate\":{}, \"traceEvents\":[";
			this->m_OutStream.flush();
		}

		void WriteFooter()
		{
			this->m_OutStream << "]}";
			this->m_OutStream.flush();
		}
		void EndSession()
		{
			this->WriteFooter();
			this->m_OutStream.close();
			delete this->m_CurrentSession;
			this->m_CurrentSession = nullptr;
			this->m_ProfileCount = 0;
		}

		static Instrumentor& Get()
		{
			static Instrumentor* instance = new Instrumentor();
			return *instance;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (this->m_ProfileCount++ > 0)
				this->m_OutStream << ", ";
			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			this->m_OutStream << "{";
			this->m_OutStream << "\"cat\":\"function\", ";
			this->m_OutStream << "\"dur\":" << (result.end - result.start) << ",";
			this->m_OutStream << "\"name\":\"" << name << "\", ";
			this->m_OutStream << "\"ph\":\"X\",";
			this->m_OutStream << "\"pid\":0,";
			this->m_OutStream << "\"tid\":" << result.threadId << ",";
			this->m_OutStream << "\"ts\":" << result.start;
			this->m_OutStream << "}";
			this->m_OutStream.flush();
		}
	};

	class InstrumentationTimer
	{
	private:
		const char* name;
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		bool m_Stop;
	public:
		InstrumentationTimer(const char* name)
		{
			this->name = name;
			this->m_Stop = false;
			this->m_Start = std::chrono::high_resolution_clock::now();
		}
		
		void Stop()
		{
			auto m_End = std::chrono::high_resolution_clock::now();
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(this->m_Start).time_since_epoch().count();

			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(m_End).time_since_epoch().count();

			uint32_t threadId = uint32_t(std::hash<std::thread::id>()(std::this_thread::get_id()));

			Instrumentor::Get().WriteProfile({ this->name, start, end, threadId });

			this->m_Stop = true;
		}

		~InstrumentationTimer()
		{
			if (!this->m_Stop)
				Stop();
		}
	};
}

#define HZ_PROFILE 1

#if HZ_PROFILE
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath) Hazel::Instrumentor::Get().BeginSession(name, filepath)
	#define HZ_PROFILE_END_SESSION() Hazel::Instrumentor::Get().EndSession()
	#define HZ_PROFILE_SCOPE(name) Hazel::InstrumentationTimer timer##_LINE__(name)
	#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
	#define HZ_PROFILE_END_SESSION() 
	#define HZ_PROFILE_SCOPE(name) 
	#define HZ_PROFILE_FUNCTION()
#endif