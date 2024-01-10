#pragma once

namespace Hazel
{
	class TimeStep
	{
	public:
		TimeStep(float time) :
			m_Time(time)
		{}

		operator float() { return this->m_Time; }

		float GetSeconds() const { return this->m_Time; }
		float GetMilliSeconds() const { return this->m_Time * 1000; }

	private:
		int nbFrames = 0;
		float m_Time;
	};
}