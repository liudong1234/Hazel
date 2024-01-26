#pragma once
#include "Hazel/Core/Layer.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include"Hazel/Events/ApplicationEvent.h"


namespace Hazel
{
	class HAZEL_API ImGuiLayer :
		public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		//		virtual void OnEvent(Event& event) override;
	private:
		/*	bool OnMouseButtonPressedEvent(MouseButtonPressEvent& e);
			bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e);
			bool OnMouseMovedEvent(MouseMovedEvent& e);
			bool OnMouseScrolledEvent(MouseScrolledEvent& e);
			bool OnKeyPressedEvent(KeyPressEvent& e);
			bool OnKeyReleaseEvent(KeyReleaseEvent& e);
			bool OnKeyTypeEvent(KeyTypeEvent& e);
			bool OnWindowResizeEvent(WindowResizeEvent& e);
		*/


	};
}