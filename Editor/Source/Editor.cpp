#include "Editor.h"

int main()
{
	de::Application app("Doctrina Editor");

	app.Initialize(1280, 720);

	if (app.Initialised)
	{
		app.Run();

	}

	app.Shutdown();
	return 0;
}