/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */


#include "SquirrelRNG.h"


// #include "YasiuMathLogs.h"


#include "Net/UnrealNetwork.h"


void USquirrel13::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
    UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USquirrel13, m_position);
    DOREPLIFETIME(USquirrel13, m_seed);
    DOREPLIFETIME(USquirrel13, m_variant);
    DOREPLIFETIME(USquirrel13, init_position);
    DOREPLIFETIME(USquirrel13, init_seed);
}

void USquirrel13::InitBP( int seed, int position )
{
    m_seed = seed;
    init_seed = seed;
    m_position = position;
    init_position = position;
}


int USquirrel13::GetNextInt( const int min, const int max )
{
    m_position++;
    return GetCurrentInt(min, max);
}

int USquirrel13::GetCurrentInt( const int min, const int max ) const
{
    const int diff = max - min;
    return FMath::RoundToInt(GetCurrentDouble() * diff) + min;
}

double USquirrel13::GetNextDouble()
{
    m_position++;
    return GetCurrentDouble();
}

double USquirrel13::GetCurrentDouble() const
{
    return static_cast<double>(get_current_random()) / UINT32_MAX;
}

int USquirrel13::GetPosition() const
{
    return m_position;
};

void USquirrel13::SetPosition( int new_position )
{
    m_position = new_position;
}

void USquirrel13::OffsetPosition( int offset )

{
    m_position += offset;
}

int32 USquirrel13::GetSeed() const
{
    return m_seed;
};

void USquirrel13::SetSeed( int new_seed )
{
    m_seed = new_seed;
}

void USquirrel13::SetNoiseVariant( int new_variant )
{
    m_variant = new_variant;
}

void USquirrel13::ResetSeedPos()
{
    m_seed = init_seed;
    m_position = init_position;
}


uint32 USquirrel13::get_current_random() const
{
    if ( m_variant == 5 ) {
        return USquirrel13::RNG_5(m_position, m_seed);
    }
    else if ( m_variant == 4 ) {
        return USquirrel13::RNG_4(m_position, m_seed);
    }
    else if ( m_variant == 3 ) {
        return USquirrel13::RNG_3(m_position, m_seed);
    }
    else if ( m_variant == 2 ) {
        return USquirrel13::RNG_2(m_position, m_seed);
    }
    else if ( m_variant == 1 ) {
        return USquirrel13::RNG_1(m_position, m_seed);
    }
    else {
        return USquirrel13::RNG_0(m_position, m_seed);
    }
}

uint32 USquirrel13::get_next_random()
{
    m_position++;
    return get_current_random();
}

uint32 USquirrel13::RNG_0( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB5297A42;
    constexpr unsigned int BIT_NOISE2 = 0x5FE41DA4;
    constexpr unsigned int BIT_NOISE3 = 0x1B54C4E9;

    uint32 mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
};

uint32 USquirrel13::RNG_1( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB4297B26;
    constexpr unsigned int BIT_NOISE2 = 0x67E23DA5;
    constexpr unsigned int BIT_NOISE3 = 0xB25AC4E8;

    uint32 mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

uint32 USquirrel13::RNG_2( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xB3297B5D;
    constexpr unsigned int BIT_NOISE2 = 0x67E42D4A;
    constexpr unsigned int BIT_NOISE3 = 0x1B47C4E9;

    uint32 mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

uint32 USquirrel13::RNG_3( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xC4397B83;
    constexpr unsigned int BIT_NOISE2 = 0x57E31D4A;
    constexpr unsigned int BIT_NOISE3 = 0x1C33DFD1;

    uint32 mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

uint32 USquirrel13::RNG_4( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0x24397B83;
    constexpr unsigned int BIT_NOISE2 = 0x57E11D27;
    constexpr unsigned int BIT_NOISE3 = 0x1C4AD2A3;

    uint32 mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

uint32 USquirrel13::RNG_5( int position, unsigned int seed )
{
    constexpr unsigned int BIT_NOISE1 = 0xC1397B83;
    constexpr unsigned int BIT_NOISE2 = 0x57E31D4A;
    constexpr unsigned int BIT_NOISE3 = 0x1C33DFD1;
    // constexpr unsigned int BIT_NOISE1 = 0x18397BA83;
    // constexpr unsigned int BIT_NOISE2 = 0x171331D4A;
    // constexpr unsigned int BIT_NOISE3 = 0x5C218DFDF;


    uint32 mangled{(unsigned int)position};
    mangled *= BIT_NOISE1;
    mangled += seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE3;
    mangled ^= (mangled >> 8);

    return mangled;
}

void USquirrel13::Serialize( FArchive& Ar )
{
    UObject::Serialize(Ar);
    // Ar << init_seed; // Non-Uproperty
    // if ( Ar.IsLoading() ) {
    //     YASIUMATHLOG_MULTI(Verbose, "Loading seed: init_seed: {0}, m_seed: {1}", init_seed, m_seed);
    // }
    // else {
    //     YASIUMATHLOG_MULTI(Verbose, "Saving seed: init_seed: {0}, m_seed: {1}", init_seed, m_seed);
    // }
}

USquirrel13_RNGComponent::USquirrel13_RNGComponent()
{
    RNG = CreateDefaultSubobject<USquirrel13>(TEXT("Squirrel_RNG"));
}

void USquirrel13_RNGComponent::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USquirrel13_RNGComponent, RNG);
}

void USquirrel13_RNGComponent::InitBP( int seed, int pos )
{
    RNG->InitBP(seed, pos);
}

int32 USquirrel13_RNGComponent::GetSeed() const
{
    return RNG->GetSeed();
}

void USquirrel13_RNGComponent::SetSeed( int64 Seed )
{
    RNG->SetSeed(Seed);
}

int USquirrel13_RNGComponent::GetPosition() const
{
    return RNG->GetPosition();
}

void USquirrel13_RNGComponent::SetPosition( int Pos )
{
    RNG->SetPosition(Pos);
}

int USquirrel13_RNGComponent::GetCurrentInt( int min, int max ) const
{
    return RNG->GetCurrentInt(min, max);
}

int USquirrel13_RNGComponent::GetNextInt( int min, int max )
{
    return RNG->GetNextInt(min, max);
}

double USquirrel13_RNGComponent::GetCurrentDouble() const
{
    return RNG->GetCurrentDouble();
}

double USquirrel13_RNGComponent::GetNextDouble()
{
    return RNG->GetNextDouble();
}
