#ifndef STARTUP_H
#define STARTUP_H

#define DllExport   __declspec( dllexport )  

DllExport int start_ninjas_engine(const int argc, const char * argv[]);

#endif // STARTUP_H