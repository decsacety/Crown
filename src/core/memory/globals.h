/*
*/

#pragma once

#include"core/memory/types.h"

namespace crown
{

    Allocator& default_allocator();
    Allocator& default_scratch_allocator();

    namespace memory_globals
    {
        //Constructs the initial default allocators
        // Has to be called before anything else during the engine startup
        void init(void);

        //Destroys the allocators created with memory_globals::init()
        //Should be the last call of the program.
        void shutdowm(void);
    }// namespace memory_globals

}//namespace crown