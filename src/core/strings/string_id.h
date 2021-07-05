/*
*/

#pragma once 

#include"core/types.h"

#define STRING_ID32_BUF_LEN 7 //? 为什么这里的长度定义为7？ 单位是B的话那么32位ID对应的就是64位了。单位是b的话就是2B，单位是块（4个字节）则刚刚好是32， 但是用32B来存一个string_id是不是有点太大了....
#define STRING_ID64_BUF_LEN 17

namespace crown
{
	// Hashed string.
	struct StringId32
	{
		u32 _id;

		StringId32() :_id(0) {}
		StringId32(u32 idx) : _id(idx) {} //32位ID的大小是32个b啊。。。。上面定义的是buffer长度吗
		explicit StringId32(const char* str);
		explicit StringId32(const char* str, u32 len);

		void hash(const char* str, u32 len);

		//Parses the id from ~str~`
		void parse(const char* str);

		// Returns this stirng converted to ASSII
		// ~buf~ size must br greater than or equal to STRING_ID32_BUF_LEN or the 
		// returned string will be truncated
		const char* to_string(char* buf, u32 len) const;
	};

	// Hashed string.
	struct StringId64
	{
		u64 _id;

		StringId64() :_id(0) {}
		StringId64(u64 idx) : _id(idx) {}
		explicit StringId64(const char* str);
		explicit StringId64(const char* str, u32 len);

		void hash(const char* str, u32 len);

		//Parses the id from ~str~`
		void parse(const char* str);

		// Returns this stirng converted to ASSII
		// ~buf~ size must br greater than or equal to STRING_ID64_BUF_LEN or the 
		// returned string will be truncated
		const char* to_string(char* buf, u32 len) const;
	};

}
