#include "startup.h"

int main(int argc, const char** argv) 
{
	int ret_val = 0;

	if (!ninjas_engine_init(argc, argv)) {
		ret_val = -1;
	}
	else {
		ninjas_engine_run__blocking_helper();
	}

	ninjas_engine_shutdown();

	return ret_val;
}
