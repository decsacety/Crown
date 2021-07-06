#include "config.h"
#include "core/containers/array.inl"
#include "core/containers/pair.inl"
#include "core/memory/memory.inl"
#include "core/memory/temp_allocator.inl"
#include "core/murmur.h"
#include "core/strings/string.inl"
#include "core/strings/string_id.inl"
#include "core/strings/string_stream.inl"
#include "core/strings/string_view.inl"

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

	static void test_containers_pair() {
		Allocator& a = default_allocator();

		struct NoAware 
		{
			int _value;

			NoAware() {}
			NoAware(int v) :_value(v) {}
			~NoAware() {}

			int GetValue() const { return _value; }
		};

		struct HasAware 
		{
			ALLOCATOR_AWARE;

			Allocator& _allocator;
			int _value;

			HasAware(Allocator& a) :_allocator(a) {}
			~HasAware() {}

			int GetValue() const { return _value; }
			void SetValue(int v) { _value = v; }
		};

		//Pair<T1, T2,0,0>
		{
			NoAware no1(1);
			NoAware no2(2);
			NoAware no3(3);
			NoAware no4(4);

			PAIR(NoAware, NoAware) pair1(no1, no2);
			ENSURE(pair1.first.GetValue() == 1);
			ENSURE(pair1.second.GetValue() == 2);

			PAIR(NoAware, NoAware) pair2(no3, no4);
			ENSURE(pair2.first.GetValue() == 3);
			ENSURE(pair2.second.GetValue() == 4);

			swap(pair1, pair2);
			ENSURE(pair2.first.GetValue() == 1);
			ENSURE(pair2.second.GetValue() == 2);
			ENSURE(pair1.first.GetValue() == 3);
			ENSURE(pair1.second.GetValue() == 4);
		}

		//Pair<T1, T2, 1,0>
		{
			HasAware has1(a);  has1.SetValue(1);
			NoAware no2(2);
			HasAware has3(a); has3.SetValue(3);
			NoAware no4(4);

			PAIR(HasAware, NoAware) pair1(has1, no2);
			ENSURE(pair1.first.GetValue() == 1);
			ENSURE(pair1.second.GetValue() == 2);

			PAIR(HasAware, NoAware) pair2(has3, no4);
			ENSURE(pair2.first.GetValue() == 3);
			ENSURE(pair2.second.GetValue() == 4);

			swap(pair1, pair2);
			ENSURE(pair1.first.GetValue() == 3);
			ENSURE(pair1.second.GetValue() == 4);
			ENSURE(pair2.first.GetValue() == 1);
			ENSURE(pair2.second.GetValue() == 2);
		}

		//Pair<T1, T2,0,1>
		{
			NoAware no1(1);
			HasAware has2(a); has2.SetValue(2);
			NoAware no3(3);
			HasAware has4(a); has4.SetValue(4);

			PAIR(NoAware, HasAware) pair1(no1, has2);
			ENSURE(pair1.first.GetValue() == 1);
			ENSURE(pair1.second.GetValue() == 2);

			PAIR(NoAware, HasAware) pair2(no3, has4);
			ENSURE(pair2.first.GetValue() == 3);
			ENSURE(pair2.second.GetValue() == 4);

			swap(pair1, pair2);
			ENSURE(pair2.first.GetValue() == 1);
			ENSURE(pair2.second.GetValue() == 2);
			ENSURE(pair1.first.GetValue() == 3);
			ENSURE(pair1.second.GetValue() == 4);
		}

		//Pair<T1, T2,1,1>
		{
			HasAware has1(a); has1.SetValue(1);
			HasAware has2(a); has2.SetValue(2);
			HasAware has3(a); has3.SetValue(3);
			HasAware has4(a); has4.SetValue(4);

			PAIR(HasAware, HasAware) pair1(has1, has2);
			ENSURE(pair1.first.GetValue() == 1);
			ENSURE(pair1.second.GetValue() == 2);

			PAIR(HasAware, HasAware) pair2(has3, has4);
			ENSURE(pair2.first.GetValue() == 3);
			ENSURE(pair2.second.GetValue() == 4);

			swap(pair1, pair2);
			ENSURE(pair2.first.GetValue() == 1);
			ENSURE(pair2.second.GetValue() == 2);
			ENSURE(pair1.first.GetValue() == 3);
			ENSURE(pair1.second.GetValue() == 4);
		}

		// Pair<T1, T2, 1, 0>::Pair(Allocator&a)
		// Pair<T1, T2, 1, 1>::Pair(Allocator&a)
		{
			PAIR(HasAware, NoAware) pair1(a);
			ENSURE(&pair1.first._allocator == &a);

			PAIR(HasAware, HasAware) pair2(a);
			ENSURE(&pair2.first._allocator == &a);
			ENSURE(&pair2.second._allocator == &a);

		}

	}

	static void test_murmur_hash()
	{
		//murmur32()
		{
			u32 hash, seed;

			seed = 0;
			hash = murmur32("abcdefghijk", 11, seed);
			ENSURE(hash == 0x4a7439a6U);

			seed = 0x0BADBEEF;
			hash = murmur32("abcdefghijk", 11, seed);
			ENSURE(hash == 0xca73e643U);
		}

		//murmur64
		{
			u64 hash, seed;

			seed = 0;
			hash = murmur64("abcdefghijk", 11, seed);
			ENSURE(hash == 0xfeff07a18c726536ULL);

			seed = 0x0BADBEEF;
			hash = murmur64("abcdefghijk", 11, seed);
			ENSURE(hash == 0x121f16453e7e7f16ULL);
		}
	}

	static void test_string_inline()
	{
		// snprintf()
		{
			char s[128];
			snprintf(s, 128, "Hello %d %s", 10, "Boy!");
			ENSURE(strcmp(s, "Hello 10 Boy!") == 0);
		}

		// strlen32()
		{
			ENSURE(strlen32("Hello!") == 6);
			ENSURE(strlen32("") == 0);
		}

		//skip_spaces()
		{

		}
	}

	static void test_string_stream()
	{

	}

	static void test_string_view()
	{

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
		RUN_TEST(test_array);
		RUN_TEST(test_containers_pair);
		RUN_TEST(test_murmur_hash);
		RUN_TEST(test_string_inline);
		RUN_TEST(test_string_stream);
		RUN_TEST(test_string_view);
		memory_globals::shutdown();
		return EXIT_SUCCESS;
	}
}

int main(int argc, char* argv[])
{
	return crown::main_unit_tests();
}