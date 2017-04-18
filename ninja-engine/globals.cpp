#include "stdafx.h"
#include "stdafx.h"
#include "globals.h"

void DebugTrace( const char * format, ... )
{
	va_list args;
	const int bufsize = 4000;
	static char buffer[bufsize];

	va_start( args, format );
	vsnprintf( buffer, bufsize - 1, format, args );

  #ifdef WIN32
	OutputDebugString(buffer);
	OutputDebugString("\n");
  #else
	fprintf(stderr, "%s", buffer);
  #endif
}

void StringSplit(std::string str, std::string delim, vector<std::string> &results) {
	uint cutAt;
	results.clear();
	while( (cutAt = str.find_first_of(delim)) != -1 ) {
		if(cutAt > 0) {
			results.push_back(str.substr(0, cutAt));
		}

		str = str.substr(cutAt + 1);
	}
	
	if(str.length() > 0)	{
		results.push_back(str);
	}
}

/*
// DEBUG HACK STUFF
#ifndef DEBUG_HACK_NASTY_STUFF
void LogEvent(LOGTYPE event, Object* obj) {
	// Do nothing.
}
#else
#include "object.h"
FILE* fObjectEventLog;

void LogEvent(LOGTYPE event, Object* obj) {
	if (!fObjectEventLog)
		return;

	std::string txt;

	switch (event) {
		case OBJECT_NEW:
			txt = "NEW";
			break;
		case OBJECT_INIT:
			txt = "INIT";
			break;
		case OBJECT_SHUTDOWN:
			txt = "SHUTDOWN";
			break;
		case OBJECT_DELETE:
			txt = "DELETE";
			break;
		case OBJECT_LAYERADD:
			txt = "LAYERADD";
			break;
		case OBJECT_LAYERREMOVE:
			txt = "LAYERREMOVE";
			break;
		default:
			TRACE("ERRORLOGHACK: WRONG VALUE??\n");
			exit(-1);
			break;
	}

	fprintf(fObjectEventLog, "%s:%lu\n", txt.c_str(), obj->unique_id);
}

void InitDebugHackLog() {
	fObjectEventLog = fopen("eventlog-hack.txt", "w");
}

void ShutdownDebugHackLog() {
	if (fObjectEventLog)
		fclose(fObjectEventLog);
}
#endif // DEBUG_HACK_NASTY_STUFF
*/
