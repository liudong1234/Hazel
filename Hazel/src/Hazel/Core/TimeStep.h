<<<<<<< HEAD
﻿#pragma once
=======
#pragma once
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7

namespace Hazel
{
	class TimeStep
	{
	public:
<<<<<<< HEAD
		TimeStep(float time = 0) :m_Time(time)
		{}

		operator float() const { return this->m_Time; }
		inline float GetSeconds() { return this->m_Time; }
		inline float GetMilliSeconds() { return this->m_Time * 1000; }

	private:
=======
		TimeStep(float time) :
			m_Time(time)
		{}

		operator float() { return this->m_Time; }

		float GetSeconds() const { return this->m_Time; }
		float GetMilliSeconds() const { return this->m_Time * 1000; }

	private:
		int nbFrames = 0;
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
		float m_Time;
	};
}