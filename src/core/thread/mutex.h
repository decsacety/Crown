/*
*/

#pragma once

#include"core/types.h"

namespace crown
{

    struct Mutex
    {
        struct Private* _priv;
        CE_ALIGN_DECL(16, u8 _data[64]);

        Mutex();
        ~Mutex();

        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;

        void lock();
        void unlock();
        void* native_handle();
    };

}//namespace crown