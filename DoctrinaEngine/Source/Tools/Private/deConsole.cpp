#include "Tools/Public/deConsole.h"

#include <iostream>
#include <windows.h> 

namespace de
{

	void Console::Post(const char* message, LogLevel level)
	{

#if DEBUG
		std::string levelText;

		switch (level)
		{
		case Console::LogLevel::Default:
			levelText = "";
			break;
		case Console::LogLevel::Warning:
			levelText = "[WARNING] ";
			break;
		case Console::LogLevel::Error:
			levelText = "[ERROR] ";
			break;
		case Console::LogLevel::ExtremeError:
			levelText = "[EXTREME ERROR] ";
			break;
		default:
			break;
		}

		std::cout << "" + levelText + message << std::endl;
#endif // DEBUG
	}

	void Console::Post(const std::string& message, LogLevel level)
	{

#if DEBUG
		std::string levelText;

		switch (level)
		{
		case Console::LogLevel::Default:
			levelText = "";
			break;
		case Console::LogLevel::Warning:
			levelText = "[WARNING] ";
			break;
		case Console::LogLevel::Error:
			levelText = "[ERROR] ";
			break;
		case Console::LogLevel::ExtremeError:
			levelText = "[EXTREME ERROR] ";
			break;
		default:
			break;
		}

		std::cout << "" + levelText + message << std::endl;
#endif // DEBUG
	}
}