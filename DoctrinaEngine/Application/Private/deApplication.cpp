#include "Application/Public/deApplication.h"
#include "Core/Public/deVersion.h"

#include <iostream>

void de::Application::Initialize()
{
	std::cout << de::Version::ENGINE_VERSION << "\n";
}
