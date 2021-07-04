#include "config.h"
#include "core/containers/array.inl"
#include "core/containers/pair.inl"
#include "core/memory/memory.inl"
#include "core/memory/temp_allocator.inl"
// #include "core/murmur.h"
// #include "core/strings/string.inl"
// #include "core/strings/string_id.inl"
// #include "core/strings/string_stream.inl"
// #include "core/strings/string_view.inl"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#undef CE_ASSERT
#undef CE_ENSURE
#undef CE_FATAL
#define ENSURE(condition)                       \
    do                                          \
    {                                           \
        if (!(condition))                       \
        {                                       \
            printf("FAILED: '%s' in %s:%d\n"    \
                , #condition                    \
                , __FILE__                      \
                , __LINE__                      \
                );                              \
            exit(EXIT_FAILURE);                 \
        }                                       \
    }                                           \
    while (0)

namespace crown
{

	static void test_default_allocator()// The origin name is test_memory
	{
		Allocator& a = default_allocator();

		void* p = a.allocate(32);
		ENSURE(a.allocated_size(p)>=32);
		a.deallocate(p);
	}

	//Nest one is the unittest for temp_allocator

	static void test_new_delete() {
#if 0
		class MyClass
		{
		public:
			MyClass() {}
			~MyClass() {}
		};

		class MyClassAllocatorAware
		{
		public:
			ALLOCATOR_AWARE;

			MyClassAllocatorAware() {}
			~MyClassAllocatorAware() {}

		private:
			Allocator& _allocator;
		};

		Allocator& a = default_allocator();

		MyClass* obj1 = CE_NEW(a, MyClass);
		ENSURE(a.allocated_size(obj1) >= sizeof(MyClass));
		CE_DELETE(a, obj1);

		MyClassAllocatorAware* obj2 = CE_NEW(a, MyClassAllocatorAware)(a);
		ENSURE(a.allocated_size(obj2) >= sizeof(MyClassAllocatorAware));
		CE_DELETE(a, obj2);
#endif
	}

	static void test_array()
	{
		Allocator& a = default_allocator();

		//basic
		{
			Array<int> v(a);

			ENSURE(array::empty(v) == true);
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 0);

			array::push_back(v, 1);
			ENSURE(array::empty(v) == false);
			ENSURE(array::size(v) == 1);
			ENSURE(v[0] == 1);

			array::push_back(v, 2);
			ENSURE(array::empty(v) == false);
			ENSURE(array::size(v) == 2);
			ENSURE(v[1] == 2);
		}

		// set_capacity()
		{
			Array<int> v(a);

			array::set_capacity(v, 10);
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 10);

			array::push_back(v, 1);
			array::set_capacity(v, 10);
			ENSURE(array::size(v) == 1);
			ENSURE(array::capacity(v) == 10);

			array::set_capacity(v, 5);
			ENSURE(array::size(v) == 1);
			ENSURE(array::capacity(v) == 5);

			array::push_back(v, 2);
			array::push_back(v, 3);
			array::push_back(v, 4);
			ENSURE(array::size(v) == 4);
			ENSURE(array::capacity(v) == 5);

			array::set_capacity(v, 2);
			ENSURE(array::size(v) == 2);
			ENSURE(array::capacity(v) == 2);

			array::set_capacity(v, 0);			// set capacity to 0 don`t realloc the memory
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 2);

		}

		//resize()
		{
			Array<int> v(a);

			array::resize(v, 2);
			ENSURE(array::size(v) == 2);
			ENSURE(array::capacity(v) == 2);

			array::resize(v, 4);
			ENSURE(array::size(v) == 4);
			ENSURE(array::capacity(v) == 4);


			array::resize(v, 3);
			ENSURE(array::size(v) == 3);
			ENSURE(array::capacity(v) == 4);
		}

		//grow()
		{
			Array<int>v(a);

			array::grow(v, 0);
			ENSURE(array::capacity(v) == 1);

			array::grow(v, 0);
			ENSURE(array::capacity(v) == 3);

			array::grow(v, 0);
			ENSURE(array::capacity(v) == 5);

			array::grow(v, 0);
			ENSURE(array::capacity(v) == 7);

			array::grow(v, 20);
			ENSURE(array::capacity(v) == 20);
			
			array::grow(v, 0);
			ENSURE(array::capacity(v) == 41);

		}


		// reserve()
		{
			Array<int> v(a);

			array::reserve(v, 10);
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 10);

			array::reserve(v, 5);
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 10);

			array::reserve(v, 15);
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 21);
		}

		// shrink_to_fit()
		{
			Array<int> v(a);

			array::reserve(v, 10);
			array::push_back(v, 1);
			array::push_back(v, 2);
			ENSURE(array::size(v) == 2);
			ENSURE(array::capacity(v) == 10);

			array::shrink_to_fit(v);
			ENSURE(array::size(v) == 2);
			ENSURE(array::capacity(v) == 2);
		}

		// push() / clear()
		{
			u32 size;
			Array<int> v(a);
			int items[] = { 1,2,3,4,5 };

			size = array::push(v, items, countof(items));
			ENSURE(size == 5);
			ENSURE(array::size(v) == 5);
			ENSURE(array::capacity(v) == 5);
			ENSURE(v[0] == 1);
			ENSURE(v[1] == 2);
			ENSURE(v[2] == 3);
			ENSURE(v[3] == 4);
			ENSURE(v[4] == 5);

			array::clear(v);
			ENSURE(array::size(v) == 0);
			ENSURE(array::capacity(v) == 5);

		}


		//begin() / end()
		{
			const int* cit;
			int* it;
			Array<int> v(a);
			int items[] = { 1,1,1,1,1 };

			array::push(v, items, countof(items));

			for (cit = array::begin(v); cit != array::end(v); ++cit) {
				ENSURE(*cit == 1);
			}

			for (it = array::begin(v); it != array::end(v); ++it) {
				ENSURE(*it == 1);
			}
		}

		// front()/ back(0
		{
			Array<int> v(a);
			int items[] = { 1,2,3,4,5 };

			array::push(v, items, countof(items));

			ENSURE(array::front(v) == 1);
			ENSURE(array::back(v) == 5 );
			
			array::pop_back(v);
			ENSURE(array::back(v) == 4);
		}

		// ctor / copt ctor
		{
			Array<int> v1(a);
			int items[] = { 1,2,3,4,5 };
			array::push(v1, items, countof(items));

			Array<int> v2(v1);
			ENSURE(array::size(v2) == 5);
			ENSURE(array::capacity(v2) == 5);
			ENSURE(v2[0] == 1);
			ENSURE(v2[1] == 2);
			ENSURE(v2[2] == 3);
			ENSURE(v2[3] == 4);
			ENSURE(v2[4] == 5);

			Array<int> v3 = v1;
			ENSURE(array::size(v3) == 5);
			ENSURE(array::capacity(v3) == 5);
			ENSURE(v3[0] == 1);
			ENSURE(v3[1] == 2);
			ENSURE(v3[2] == 3);
			ENSURE(v3[3] == 4);
			ENSURE(v3[4] == 5);

		}

	}


#define RUN_TEST(name)    \
	do{					  \
		name();			  \
	}					  \
	while(0)

	int main_unit_tests()
	{
		memory_globals::init();
		RUN_TEST(test_default_allocator);
		RUN_TEST(test_new_delete);
		memory_globals::shutdown();
		return EXIT_SUCCESS;
	}
}

int main(int argc, char* argv[])
{
	return crown::main_unit_tests();
}