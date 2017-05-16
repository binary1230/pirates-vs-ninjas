#include "startup.h"

void mainloop() {
	while (!ninjas_engine_should_exit()) {
		ninjas_engine_process_events();
		ninjas_engine_tick();
	}
}

int main(int argc, const char** argv) 
{
	bool init_ok = ninjas_engine_init(argc, argv);

	if (init_ok) {
		mainloop();
	}

	ninjas_engine_shutdown();

	return 0 ? init_ok : -1;
}