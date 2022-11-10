#pragma once

#include <iostream>

#include "Core/Utils/Keys.hpp"
#include "Core/Utils/Input.hpp"
#include "Core/Utils/WindowEvents.hpp"

#include "Core/ApplicationWindow/WindowBase.hpp"

#include <memory>


namespace rec
{
	class Camera;

	class Application
	{
	public:

		Application(char** argv);
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		Camera get_camera();

		virtual int start_window(unsigned int windowWidth, unsigned int windowHeight, const char* title);

		virtual void on_update() {}

		virtual void on_mouse_button_event(const MouseButton button_code, const double x_pos, const double y_pos, const bool pressed) {}

	protected:

		Camera* m_pCamera;
		std::unique_ptr<class WindowBase> m_pWindow;

		EventDispatcher m_event_dispatcher;
		bool m_should_window_close = false;
	};
}
