#include "World01.h"
#include "World02.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{
	INFO_LOG("Initialize Engine...")

	lady::MemoryTracker::Initialize();
	lady::seedRandom((unsigned int)time(nullptr));
	lady::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<lady::World02>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<lady::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
