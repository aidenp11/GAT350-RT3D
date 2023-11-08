#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "World06.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Core/StringUtils.h"

using namespace std;

int main(int argc, char* argv[])
{
	INFO_LOG("Initialize Engine...")

	//lady::MemoryTracker::Initialize();
	lady::seedRandom((unsigned int)time(nullptr));
	lady::setFilePath("assets");

	ENGINE.Initialize();

	cout << lady::ToUpper("FoRtNiTe") + '\n';
	cout << lady::ToLower("FoRtNiTe") + '\n';
	cout << lady::CreateUnique("fortnite") + '\n';
	cout << lady::CreateUnique("fortnite") + '\n';
	cout << lady::IsEqualIgnoreCase("fortnite", "FoRtNiTe") + '\n';

	auto world = make_unique<lady::World06>();
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
