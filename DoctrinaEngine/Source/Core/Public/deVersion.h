#ifndef _DEVERSION_
#define _DEVERSION_

#include <string>

namespace de::Version
{
	const int ENGINE_MAJOR = 0;
	const int ENGINE_MINOR = 4;
	const int ENGINE_REVISION = 8;

	const std::string ENGINE_VERSION = std::to_string(ENGINE_MAJOR) + "." + std::to_string(ENGINE_MINOR) + "." + std::to_string(ENGINE_REVISION);
}

#endif // !_DEVERSION_
