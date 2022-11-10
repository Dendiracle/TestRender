#pragma once

#include "Core/Utils/WindowEvents.hpp"

#include <functional>

namespace rec
{
	class WindowBase
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		WindowBase(unsigned int windowWidth = 1360, unsigned int windowHeight = 760, const char* title = "TITLE");
		~WindowBase();

		void set_event_callback(const EventCallbackFn& callback)
		{
			m_Data.eventCallbackFn = callback;
		}

		virtual void onUpdate() {};

	protected:

		virtual int init() { return -1; };
		virtual void shutdown() {};

		struct WindowData
		{
			unsigned int width;
			unsigned int height;
			const char* p_Title;
			EventCallbackFn eventCallbackFn;
		};

		WindowData m_Data;
	};
};