#include "Editor.h"

int main()
{
	de::Application app("Doctrina Editor", 1280, 720);

	app.Initialize();
	if (app.Initialised)
	{
		app.Run();
	}

	app.Shutdown();
	return 0;
}