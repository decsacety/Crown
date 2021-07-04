/*
*/

//#include"core/error/callstack.h"
#include"core/error/error.h"
#include"core/memory/temp_allocator.inl"
//#include"core/strings/string_stream.inl"
//#include"device/log.h"
#include<stdarg.h>
#include<stdlib.h> // exit

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>// TODO: remove that

// LOG_SYSTEM(ERROR, "error")

namespace crown {

	namespace error
	{
		
		void abort(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			//vabort(format, args);
			va_end(args);
		}

	}

}

