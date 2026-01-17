/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
*/

#include "SquirrelRNG.h"


void USquirrel13_RNG::InitBP( int seed, int position )
{
    m_seed = seed;
    init_seed = seed;
    m_position = position;
    init_position = position;
}

uint32_t USquirrel13_RNG::get_current_random() const
{
    uint32_t randomValue = 2025;

    if ( m_variant == 3 ) {
        randomValue = USquirrel13_RNG::RNG_3(m_position, m_seed);
    }
    else if ( m_variant == 2 ) {
        randomValue = USquirrel13_RNG::RNG_2(m_position, m_seed);
    }
    else if ( m_variant == 1 ) {
        randomValue = USquirrel13_RNG::RNG_1(m_position, m_seed);
    }
    else {
        randomValue = USquirrel13_RNG::RNG_0(m_position, m_seed);
    }
    return randomValue;
}

uint32_t USquirrel13_RNG::get_next_random()
{
    m_position++;
    return get_current_random();
}

int USquirrel13_RNG::GetNextInt( const int min, const int max )
{
    m_position++;
    return GetCurrentInt(min, max);
}

int USquirrel13_RNG::GetCurrentInt( const int min, const int max ) const
{
    const int diff = max - min;
    return FMath::RoundToInt32(GetCurrentDouble() * diff) + min;
}

double USquirrel13_RNG::GetNextDouble()
{
    m_position++;
    return GetCurrentDouble();
}

double USquirrel13_RNG::GetCurrentDouble() const
{
    return static_cast<double>(get_current_random()) / UINT32_MAX;
};

void USquirrel13_RNG::SetPosition( int new_position )
{
    m_position = new_position;
}

void USquirrel13_RNG::IncrementPosition( int offset )
{
    m_position += offset;
};

void USquirrel13_RNG::SetSeed( int new_seed )
{
    m_seed = new_seed;
}

void USquirrel13_RNG::SetNoiseVariant( int new_variant )
{
    m_variant = new_variant;
    if ( m_variant > 3 ) {
        /* Todo: Warning Log ?*/
    }
}

void USquirrel13_RNG::ResetSeedPos()
{
    m_seed = init_seed;
    m_position = init_position;
}


uint32_t USquirrel13_RNG::RNG_0( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB5297A4D;
    constexpr unsigned int BIT_NOISE2 = 0x68E41DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1B54C4E9;

    uint32_t mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
};

uint32_t USquirrel13_RNG::RNG_1( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB4297B4C;
    constexpr unsigned int BIT_NOISE2 = 0x67E13DA5;
    constexpr unsigned int BIT_NOISE3 = 0x1B55C4E8;

    uint32_t mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

uint32_t USquirrel13_RNG::RNG_2( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB3297B5D;
    constexpr unsigned int BIT_NOISE2 = 0x67E42DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1B47C4E9;

    uint32_t mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

uint32_t USquirrel13_RNG::RNG_3( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xC4397B83;
    constexpr unsigned int BIT_NOISE2 = 0x57E31D4A;
    constexpr unsigned int BIT_NOISE3 = 0x1C33DFD1;

    uint32_t mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

void USquirrel13_RNG::Serialize( FArchive& Ar )
{
    UObject::Serialize(Ar);
    Ar << init_seed; // Non-Uproperty
}
