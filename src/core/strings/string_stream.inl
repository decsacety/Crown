/*
*/

#pragma once 

#include"core/containers/array.inl"
#include"core/strings/string.inl"
#include"core/strings/string_stream.h"

namespace crown
{
	inline StringStream& operator<<(StringStream& s, char val)
	{
		array::push_back(s, val);
		return s;
	}//相较于原有的string重载的+,<<操作符有点让人迷惑，如果重载的是+就好了

	inline StringStream& operator<<(StringStream& s, s16 val)
	{
		return string_stream::stream_printf(s, "%hd", val);
	}

	inline StringStream& operator<<(StringStream& s, u16 val)
	{
		return string_stream::stream_printf(s, "%hu", val);
	}

	inline StringStream& operator<<(StringStream&s, s32 val) 
	{
		return string_stream::stream_printf(s, "%d", val);
	}

	inline StringStream& operator<<(StringStream& s, u32 val)
	{
		return string_stream::stream_printf(s, "%u", val);
	}

	inline StringStream& operator<<(StringStream& s, s64 val)
	{
		return string_stream::stream_printf(s, "%lld", val);
	}

	inline StringStream& operator<<(StringStream& s, u64 val)
	{
		return string_stream::stream_printf(s, "%llu", val);
	}

	inline StringStream& operator<<(StringStream& s, f32 val)
	{
		return string_stream::stream_printf(s, "%.8f", val);
	}

	inline StringStream& operator<<(StringStream& s, f64 val)
	{
		return string_stream::stream_printf(s, "%.16g", val);
	}

	inline StringStream& operator<<(StringStream& s, const char* str)
	{
		array::push(s, str, strlen(str));
		return s;
	}

	namespace string_stream
	{
		inline const char* c_str(StringStream& s)
		{
			array::push_back(s, '\0');
			array::pop_back(s);
			return array::begin(s);
		}

		template <typename T>
		inline StringStream& stream_printf(StringStream& s, const char* format, T& val)
		{
			char buf[32];
			snprintf(buf, sizeof(buf), format, val);
			return s << buf;
		} 


	}// namespace string_stream

}//namespace crown