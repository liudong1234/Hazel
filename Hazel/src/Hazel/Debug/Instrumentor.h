#pragma once
#include<iostream>
#include<chrono>
#include <algorithm>
#include <fstream>
#include <string>
#include <thread>

namespace Hazel
{

    struct profileResult
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
        InstrumentationSession* m_currentSession;
        std::ofstream m_outStream;
        int m_profileCount;
    public:
        Instrumentor() :m_currentSession(nullptr), m_profileCount(0)
        {
        }
        void BeginSession(const std::string& name, const std::string& filepath = "result.json")
        {
            m_outStream.open(filepath);
            WriteHeader();
            m_currentSession = new InstrumentationSession{ name };
        }

        void EndSession()
        {
            WriteFooter();
            m_outStream.close();
            delete m_currentSession;
            m_currentSession = nullptr;
            m_profileCount = 0;
        }

        void WriteHeader()
        {
            m_outStream << "{\"otherDate\": {}, \"traceEvents\":[";
            m_outStream.flush();
        }

        void WriteFooter()
        {
            m_outStream << "]}";
            m_outStream.flush();
        }

        static Instrumentor& Get()
        {
            static Instrumentor* instance = new Instrumentor();
            return *instance;
        }


        void WriteProfile(const profileResult& result)
        {
            if (m_profileCount++ > 0)
                m_outStream << ",";
            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_outStream << "{";
            m_outStream << "\"cat\":\"function\",";
            m_outStream << "\"dur\":" << (result.end - result.start) << ",";
            m_outStream << "\"name\":\"" << name << "\",";
            m_outStream << "\"ph\":\"X\",";
            m_outStream << "\"pid\":0,";
            m_outStream << "\"tid\":" << result.threadId << ",";
            m_outStream << "\"ts\":" << result.start;
            m_outStream << "}";
            m_outStream.flush();
        }
    };

    //利用类的构造析构自动计时
    class InstrumentationTimer
    {
    private:
        const char* name;//������
        std::chrono::time_point<std::chrono::steady_clock> m_start;
        bool m_stop;
    public:
        InstrumentationTimer(const char* name)
        {
            this->name = name;
            this->m_stop = false;
            m_start = std::chrono::high_resolution_clock::now();
        }

        void Stop()
        {
            auto m_end = std::chrono::high_resolution_clock::now();
            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(m_end).time_since_epoch().count();

            uint32_t threadId = (uint32_t)std::hash<std::thread::id>()(std::this_thread::get_id());

            //std::cout << name << ": " << (end - start) << "ms\n";

            Instrumentor::Get().WriteProfile({ name, start, end, threadId });

            m_stop = true;
        }

        ~InstrumentationTimer()
        {
            if (!m_stop)
                Stop();
        }
    };
}

#define HZ_PROFILE 1

#if HZ_PROFILE
#define HZ_PROFILE_BEGIN_SESSION(name, filepath) Hazel::Instrumentor::Get().BeginSession(name, filepath)
#define HZ_PROFILE_END_SESSION() Hazel::Instrumentor::Get().EndSession()
#define HZ_PROFILE_SCOPE(name) Hazel::InstrumentationTimer timer##_LINE__(name) //具体到函数的一个过程
#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__) //一个函数

#else
#define HZ_PROFILE_BEGIN_SESSION(name, filepath) 
#define HZ_PROFILE_END_SESSION()
#define HZ_PROFILE_SCROPE(name)
#define HZ_PROFILE_FUNCTION()

#endif
