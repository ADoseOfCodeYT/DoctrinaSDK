#ifndef _DECONSOLE_H_
#define _DECONSOLE_H_

#include <string>

namespace de :: Console
{
	enum class LogLevel
	{
		Default,
		Warning,
		Error,
		ExtremeError // will bring console to front
	};

	void Toggle();
	void Lock();
	void UnLock();

	void Post(const char* message, LogLevel level = LogLevel::Default);
	void Post(const std::string& message, LogLevel level = LogLevel::Default);

	bool IsFocus();

}

#endif // !_DECONSOLE_H_
