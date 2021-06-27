#include "config.h"
//#include "core/containers/array.inl"
//#include "core/memory/memory.inl"
// #include "core/memory/temp_allocator.inl"
// #include "core/murmur.h"
// #include "core/strings/string.inl"
// #include "core/strings/string_id.inl"
// #include "core/strings/string_stream.inl"
// #include "core/strings/string_view.inl"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

namespace crown{
	int main_unit_tests()
	{
		//memory_globals::init();

		return 1;
	}
}

int main(int argc, char* argv[])
{
	return crown::main_unit_tests();
}