//#include <debugapi.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include "Profiler.h"
//#include <map>
//#include <iostream>
//#include <cassert>
//
//int VDebugPrintF(const char* format, va_list argList)
//{
//	const unsigned int MAX_CHARS = 1024;
//	static char s_buffer[MAX_CHARS];
//
//	int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, argList);
//
//	OutputDebugStringA(s_buffer);
//
//	return charsWritten;
//}
//
//int DebugPrintF(const char* format, ...)
//{
//	va_list argList;
//	va_start(argList, format);
//
//	int charsWritten = VDebugPrintF(format, argList);
//
//	va_end(argList);
//
//	return charsWritten;
//}

//int VerboseDebugPrintF(int verbosity, const char* format, ...)
//{
//	if (g_verbosity < verbosity)
//		return 0;
//
//	va_list argList;
//	va_start(argList, format);
//	int charsWritten = VDebugPrintF(format, argList);
//	va_end(argList);
//	return charsWritten;
//}

