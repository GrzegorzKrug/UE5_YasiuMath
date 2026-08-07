/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */


#include "PCG_RNG.h"


#include "Net/UnrealNetwork.h"


void UPCG_RNG32::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
    UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UPCG_RNG32, state);
    DOREPLIFETIME(UPCG_RNG32, stream);
}

void UPCG_RNG32::InitBP( int64 StateIn, int64 StreamIn )
{
    SetState(StateIn);
    SetNoiseStream(StreamIn);
}

int64 UPCG_RNG32::GetState() const
{
    return static_cast<int64>(state);
}

void UPCG_RNG32::SetState( int64 stateIn )
{
    state = static_cast<uint64>(stateIn);
}

void UPCG_RNG32::SetNoiseStream( int64 streamIn )
{
    stream = static_cast<uint64>(streamIn);
    if ( !(stream % 2) ) {
        stream |= 1;
    }
}

double UPCG_RNG32::GetNextDouble()
{
    return static_cast<double>(NextNumber()) / UINT32_MAX;
}

double UPCG_RNG32::GetCurrentDouble() const
{
    return static_cast<double>(CurrentNumber()) / UINT32_MAX;
}

int UPCG_RNG32::GetNextInt( int A, int B )
{
    NextNumber();
    return GetCurrentInt(A, B);
}

int UPCG_RNG32::GetCurrentInt( int A, int B ) const
{
    const int diff = B - A;
    return FMath::RoundToInt(GetCurrentDouble() * diff) + A;
}

uint32 UPCG_RNG32::CurrentNumber() const
{
    const uint64 OldState = state;
    uint32 xorshifted = ((OldState >> 18u) ^ OldState) >> 27u;
    uint32 rot = OldState >> 59u;

    // last = (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    return (xorshifted >> rot) | (xorshifted << ((32 - rot) & 31));
}

uint32 UPCG_RNG32::NextNumber()
{
    const uint64 OldState = state;
    state = OldState * 6364136223846793005ULL + (stream | 1);

    return CurrentNumber();
}

UPCG32_RNGComponent::UPCG32_RNGComponent()
{
    RNG = CreateDefaultSubobject<UPCG_RNG32>(TEXT("PCG_RNG"));
}

void UPCG32_RNGComponent::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UPCG32_RNGComponent, RNG);
}

void UPCG32_RNGComponent::InitBP( int64 State, int64 Noise )
{
    RNG->InitBP(State, Noise);
}

int64 UPCG32_RNGComponent::GetState() const
{
    return RNG->GetState();
}

void UPCG32_RNGComponent::SetState( int64 State )
{
    RNG->SetState(State);
}

void UPCG32_RNGComponent::SetStreamNoise( int64 Noise )
{
    RNG->SetNoiseStream(Noise);
}

int UPCG32_RNGComponent::GetCurrentInt( int min, int max ) const
{
    return RNG->GetCurrentInt(min, max);
}

int UPCG32_RNGComponent::GetNextInt( int min, int max )
{
    return RNG->GetNextInt(min, max);
}

double UPCG32_RNGComponent::GetCurrentDouble() const
{
    return RNG->GetCurrentDouble();
}

double UPCG32_RNGComponent::GetNextDouble()
{
    return RNG->GetNextDouble();
}
