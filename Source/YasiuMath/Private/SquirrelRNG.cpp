#include "SquirrelRNG.h"

//Squirrel13_RNG::Squirrel13_RNG(int position = 0, unsigned int seed = 1) :
//	m_position{ position }, m_seed{ seed }
//{
//	//m_position = position; m_seed = seed;
//};

void USquirrel13_RNG::Init( int seed, int position )
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
    } else if ( m_variant == 2 ) {
        randomValue = USquirrel13_RNG::RNG_2(m_position, m_seed);
    } else if ( m_variant == 1 ) {
        randomValue = USquirrel13_RNG::RNG_1(m_position, m_seed);
    } else {
        /// Wrong variant: use default
        randomValue = USquirrel13_RNG::RNG_0(m_position, m_seed);
    }
    return randomValue;
}

uint32_t USquirrel13_RNG::get_random()
{
    m_position++;
    return get_current_random();
}

int USquirrel13_RNG::get_random_int( const int& min, const int& max )
{
    m_position++;
    return get_current_int(min, max);
}

int USquirrel13_RNG::get_current_int( const int& min, const int& max ) const
{
    int diff = max - min;
    return FMath::RoundToInt32(get_current_double() * diff) + min;
}

double USquirrel13_RNG::get_random_double()
{
    m_position++;
    return static_cast<double>(get_random()) / UINT32_MAX;
}

double USquirrel13_RNG::get_current_double() const
{
    return static_cast<double>(get_current_random()) / UINT32_MAX;
};

void USquirrel13_RNG::set_position( int new_position ) { m_position = new_position; };

void USquirrel13_RNG::set_seed( int new_seed ) { m_seed = new_seed; }

void USquirrel13_RNG::noiseVariant( int new_variant ) { m_variant = new_variant; }

void USquirrel13_RNG::resetSeedPos()
{
    m_seed = init_seed;
    m_position = init_position;
}


uint32_t USquirrel13_RNG::RNG_0( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB5297A4D;
    constexpr unsigned int BIT_NOISE2 = 0x68E31DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1B56C4E9;

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
    constexpr unsigned int BIT_NOISE1 = 0xB4297A4C;
    constexpr unsigned int BIT_NOISE2 = 0x67E31DA5;
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
    constexpr unsigned int BIT_NOISE2 = 0x67E31DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1B44C4E9;

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
    constexpr unsigned int BIT_NOISE1 = 0xC4397B28;
    constexpr unsigned int BIT_NOISE2 = 0x57E31DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1C33DDE9;

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
    Ar << init_seed;
    Ar << init_position;

    // if ( Ar.IsSaving() ) {
    // Ar << init_seed;
    // Ar << init_position;
    // } else if ( Ar.IsLoading() ) {
    //     Ar << init_seed;
    //     Ar << init_position;
    // }
}
