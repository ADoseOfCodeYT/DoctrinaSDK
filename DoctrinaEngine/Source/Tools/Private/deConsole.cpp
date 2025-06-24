#include "Tools/Public/deConsole.h"

#include <iostream>
#include <windows.h> 

namespace de
{

	void console::Post(const char* message, LogLevel level)
	{

#if DEBUG
		std::string levelText;

		switch (level)
		{
		case console::LogLevel::Default:
			levelText = "";
			break;
		case console::LogLevel::Warning:
			levelText = "[WARNING] ";
			break;
		case console::LogLevel::Error:
			levelText = "[ERROR] ";
			break;
		case console::LogLevel::ExtremeError:
			levelText = "[EXTREME ERROR] ";
			break;
		default:
			break;
		}

		std::cout << "" + levelText + message << std::endl;
#endif // DEBUG
	}

	void console::Post(const std::string& message, LogLevel level)
	{

#if DEBUG
		std::string levelText;

		switch (level)
		{
		case console::LogLevel::Default:
			levelText = "";
			break;
		case console::LogLevel::Warning:
			levelText = "[WARNING] ";
			break;
		case console::LogLevel::Error:
			levelText = "[ERROR] ";
			break;
		case console::LogLevel::ExtremeError:
			levelText = "[EXTREME ERROR] ";
			break;
		default:
			break;
		}

		std::cout << "" + levelText + message << std::endl;
#endif // DEBUG
	}
}