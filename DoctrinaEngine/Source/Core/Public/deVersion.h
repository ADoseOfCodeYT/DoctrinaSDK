#ifndef _DEVERSION_H_
#define _DEVERSION_H_

#include <string>

namespace de::Version
{
	const int ENGINE_MAJOR = 0;
	const int ENGINE_MINOR = 2;
	const int ENGINE_REVISION = 6;

	const std::string ENGINE_VERSION = std::to_string(ENGINE_MAJOR) + "." + std::to_string(ENGINE_MINOR) + "." + std::to_string(ENGINE_REVISION);
}

#endif // !_DEVERSION_
