#pragma once

<<<<<<< HEAD
#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/TimeStep.h"
=======

#include "Hazel/Core/Core.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Events/Event.h"

>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7

namespace Hazel
{
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}