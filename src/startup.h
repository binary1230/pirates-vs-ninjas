#ifndef STARTUP_H
#define STARTUP_H

#define DllExport   __declspec( dllexport )  

DllExport int run_ninjas_engine___helper(const int argc, const char * argv[]);

DllExport bool ninjas_engine_init(const int argc, const char * argv[]);

DllExport void ninjas_engine_shutdown();

DllExport void ninjas_engine_run__blocking_helper();

#endif // STARTUP_H