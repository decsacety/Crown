/*
*/

#pragma once

#include"core/types.h"

namespace crown
{
	u32 murmur32(const void* key, u32 len, u32 seed);
	u64 murmur64(const void* key, u32 len, u64 seed);
}