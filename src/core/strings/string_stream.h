/*
*/

#pragma once 

#include"core/containers/types.h"

namespace crown
{
	typedef Array<char> StringStream;

	namespace string_stream
	{
		//Return the stream as a NUL_terminated string
		const char* c_str(StringStream& s);

		template<typename T> StringStream& stream_printf(StringStream& s, const char* format, T& val);

	}//namespace string_stream

}//namespace crown