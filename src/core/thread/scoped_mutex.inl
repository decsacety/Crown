/*
*/

#pragma once

#include"core/thread/mutex.h"

namespace crown
{

    //Automaticaly locks a mutex when created and unlocks when destroyed

    struct ScopedMutex
    {
        Mutex& _mutex;

        ScopedMutex(Mutex& m) : _mutex(m)
        {
            _mutex.lock();
        }

        ~ScopedMutex()
        {
            _mutex.unlock();
        }

        ScopedMutex(const ScopedMutex&) = delete;
        ScopedMutex& operator=(const ScopedMutex&) = delete;
        
    };
    
}// namespace crown