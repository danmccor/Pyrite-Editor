#include "Panda3D.h"
#include "SaveManager.h"

ClockObject globalClock;
Panda3D pandaEngine;

int main(int argc, char* argv[])
{
	pandaEngine.init(NULL, argc, argv, 1920, 1080, 100, 100, true);

	SaveManager manager(NULL);
	manager.LoadBuild();

	Thread* mainloop = Thread::get_current_thread();
	while (pandaEngine.framework.do_frame(mainloop)) {
		pandaEngine.MouseCollider();
		pandaEngine.runLoop();
		globalClock.tick();
	}
	return 0;
}