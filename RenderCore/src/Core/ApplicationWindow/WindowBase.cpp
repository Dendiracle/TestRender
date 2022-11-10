#include "Core/ApplicationWindow/WindowBase.hpp"

#include <iostream>

namespace rec
{
    WindowBase::WindowBase(unsigned int windowWidth, unsigned int windowHeight, const char* title)
        : m_Data({ windowWidth, windowHeight, std::move(title) })
    {
    }

    WindowBase::~WindowBase()
	{
        delete[] this->m_Data.p_Title;
	}

}