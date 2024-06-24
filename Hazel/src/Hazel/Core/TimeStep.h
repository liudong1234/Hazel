#pragma once

namespace Hazel
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0) :m_Time(time)
		{}

		operator float() const { return this->m_Time; }
		inline float GetSeconds() { return this->m_Time; }
		inline float GetMilliSeconds() { return this->m_Time * 1000; }

	private:
		float m_Time;
	};
}