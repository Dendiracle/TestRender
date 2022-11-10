#pragma once

#include "Core/ApplicationWindow/WindowBase.hpp"

#include <iostream>

#include "Core/Utils/WindowEvents.hpp"

struct GLFWwindow;

namespace rec
{

	class WindowOpenGL : public virtual WindowBase
	{
	public:

		WindowOpenGL(unsigned int windowWidth = 1360, unsigned int windowHeight = 760, const char* title = "TITLE");
		~WindowOpenGL();

		virtual void onUpdate() override;

	protected:

		virtual int init() override;
		virtual void shutdown() override;

		GLFWwindow* m_pGLWindow = nullptr;
	};
};