/*
*/

#pragma once

#include "core/types.h"

namespace crown
{   

    struct Allocator
    {
        Allocator()
        {

        }

        virtual ~Allocator()
        {

        }

        Allocator(const Allocator&) = delete;
        Allocator& operator=(const Allocator&) = delete;

        // Allocates ~size~ bytes of memory aligned to the specified
        // ~align~ byte and returns a pointer to the first allocated byte.
        virtual void* allocate(u32 size, u32 align = DEFAULT_ALIGN) = 0;

        //Deallocates a previously allocated block of memory pointed by ~data~
        virtual void deallocate(void* data) = 0;

        //Return the size of the memory lock pointed by ~ptr~ or SIZE_NOT_TRACKED
        //if the allocator does not support memory tracking
        //~ptr~ must be a pointer retureed by Allocator::allocate()
        virtual u32 allocated_size(const void* ptr) = 0;

        // Returns the total number of bytes allocated
        virtual u32 total_allocated() = 0;

        //Default memory alignment in bytes
        static const u32 DEFAULT_ALIGN = 8;
        static const u32 SIZE_NOT_TRACKED = 0xffffffffu;

    };

}
//nanmespace crown