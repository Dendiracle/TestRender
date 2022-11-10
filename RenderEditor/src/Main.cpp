#include <iostream>

#include <Core/Application.hpp>

int main(int argc, char** argv)
{
	rec::Application* application = new rec::Application(argv);
	application->start_window(1360, 760, "Window");
	return 0;
}