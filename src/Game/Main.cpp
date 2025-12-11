#include "Core/Application.h"
#include <iostream>

int main()
{
	try
	{
		Tassathras::Application app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "unhandled exception: " << e.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cerr << "unkonwn exception.\n";
		return -1;
	}

	return 0;
}
