#include "stdafx.h"
#include "globals.h"

void StringSplit(CString str, CString delim, vector<CString> &results) {
	uint cutAt;
	results.clear();
	while( (cutAt = str.find_first_of(delim)) != str.npos ) {
		if(cutAt > 0) {
			results.push_back(str.substr(0,cutAt));
		}

		str = str.substr(cutAt+1);
	}
	
	if(str.length() > 0)	{
		results.push_back(str);
	}
}

