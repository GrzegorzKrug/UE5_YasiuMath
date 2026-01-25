/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

#include "PCG_RNG.h"

uint32_t UPCG32_RNG::next()
{
    uint64_t oldstate = state;
    state = oldstate * 6364136223846793005ULL + (stream | 1);

    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;

    // last = (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    last = (xorshifted >> rot) | (xorshifted << ((32 - rot) & 31));
    return last;
}


void UPCG32_RNG::InitBP( int64 stateIn, int64 streamIn )
{
    SetState(stateIn);
    SetStream(streamIn);
}

double UPCG32_RNG::GetNextDouble()
{
    return static_cast<double>(next()) / UINT32_MAX;
}

double UPCG32_RNG::GetCurrentDouble() const
{
    return static_cast<double>(last) / UINT32_MAX;
}

int UPCG32_RNG::GetNextInt( int A, int B )
{
    next();
    return GetCurrentInt(A, B);
}

int UPCG32_RNG::GetCurrentInt( int A, int B ) const
{
    auto diff = B - A;
    return static_cast<int>(GetCurrentDouble() * diff + A);
}


void UPCG32_RNG::Serialize( FArchive& Ar )
{
    UObject::Serialize(Ar);
    Ar << state;
    Ar << stream;
    Ar << last;
}
