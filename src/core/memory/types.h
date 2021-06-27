/*
*/

#pragma once
namespace crown
{
    struct Allocator;

    //Determines if a tyoe is allocator aware.
    template<typename T>
    struct is_allocator_aware
    {
        template<typename C>
        static char test_fun(typename C::allocator_aware *);

        template<typename C>
        static int test_fun(...);

        enum{
            value = (sizeof(test_fun<T)(0) ==sizeof(char))
        };
    };
    
    
}//namespcae crown

#define ALLOCATOR_AWARE         typedef int allocator_aware
#define IS_ALLOCATOR_AWARE(T)      is_allocator_aware<T>::value
#define IS_ALLOCATOR_AWARE_TYPE(T)   Int2Type< IS_ALLOCATOR_AWARE(T) >