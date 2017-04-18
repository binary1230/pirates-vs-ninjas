#ifndef STARTUP_H
#define STARTUP_H

#define DllExport   __declspec( dllexport )  

DllExport int run_ninjas_engine___helper(const int argc, const char * argv[]);

DllExport bool ninjas_engine_init(const int argc, const char * argv[]);

DllExport void ninjas_engine_shutdown();

DllExport void ninjas_engine_run__blocking_helper();

DllExport bool ninjas_engine_should_exit();

DllExport void ninjas_engine_process_events();

DllExport void ninjas_engine_tick();

#endif // STARTUP_H