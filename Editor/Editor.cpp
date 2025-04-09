#include "Editor.h"

int main()
{
	de::Application app("Doctrina Editor", 1280, 720);

	app.Initialize();

	de::RHI_D3D11 rhi;

	rhi.Initialize(1280, 720, app.GetWindow(), false, 1000.0f, 0.3f);

	if (app.Initialised)
	{
		app.Run();
	}

	app.Shutdown();
	return 0;
}