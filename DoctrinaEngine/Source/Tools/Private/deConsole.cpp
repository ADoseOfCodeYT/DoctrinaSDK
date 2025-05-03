#include "Tools/Public/deConsole.h"

#include <iostream>
#include <windows.h> 

namespace de
{

	void Console::Post(const char* message, LogLevel level)
	{

#if DEBUG
		std::cout << "" << message << std::endl;
#endif // DEBUG
	}

	void Console::Post(const std::string& message, LogLevel level)
	{

#if DEBUG
		std::cout << "" + message << std::endl;
#endif // DEBUG
	}
}