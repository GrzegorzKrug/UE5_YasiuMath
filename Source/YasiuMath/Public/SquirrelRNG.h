/*
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

#pragma once
#include  "CoreMinimal.h"

#include  "SquirrelRNG.generated.h"


/**
 * @brief Random number generator. Seedable. Gives better random values than epic generator.
 */
UCLASS(BlueprintType)
class YASIUMATH_API USquirrel13_RNG : public UObject {
public:
    GENERATED_BODY()

    USquirrel13_RNG() {};

    USquirrel13_RNG( unsigned int seed )
        : m_seed(seed), init_seed(seed) {};

    USquirrel13_RNG( int position, unsigned int seed )
        : m_position(position), m_seed(seed), init_position(position), init_seed(seed) {};

    USquirrel13_RNG( int position, unsigned int seed, unsigned int variant )
        : m_position(position), m_seed(seed), m_variant(variant), init_position(position), init_seed(seed) {};

protected:
    UPROPERTY()
    int m_position{0};

    UPROPERTY()
    unsigned int m_seed{1};

    UPROPERTY()
    int m_variant{0};

    UPROPERTY()
    unsigned int init_position = 0;

    // UPROPERTY(BlueprintReadWrite)
    int32 init_seed = 0;

public:
    /**
     * This function provides access to change private variables for C++ object which is not accessible in Blueprints.
     * @brief Modify internal values to custom.
     */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void InitBP( int seed, int position = 0 );

    /** @internal Return current uint32_t without changing position */
    uint32_t get_current_random() const;

    /** @internal Move position by 1 and return uint32_t */
    uint32_t get_random();

    /** @brief Move position by 1 and return random value as integer in range <min, max> */
    UFUNCTION(BlueprintCallable, Category="RNG")
    int GetNextInt( int min, int max );

    /** @brief Return random value scaled to integer range <min, max> at current position */
    UFUNCTION(BlueprintCallable, Category="RNG")
    int GetCurrentInt( int min, int max ) const;

    /**
     * @brief Move position by 1 and return double in range <0, 1>
     */
    UFUNCTION(BlueprintCallable, Category="RNG")
    double GetNextDouble();

    /** @brief Return last random double in range <0, 1> */
    UFUNCTION(BlueprintCallable, Category="RNG")
    double GetCurrentDouble() const;

    /** @brief Set generator position */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetPosition( int new_position );
    
    /** @brief Offset generator position */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void IncrementPosition( int offset=1 );

    /** @brief Set seed noise seed */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetSeed( int new_seed );
    
    /** @brief Internal noise variants (it is not initialized), default=0, max=3 */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetNoiseVariant( int newVariant=0 );

    /** @brief Reset to internal initial values */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void ResetSeedPos();

    static uint32_t RNG_0( int position, unsigned int seed );

    static uint32_t RNG_1( int position, unsigned int seed );

    static uint32_t RNG_2( int position, unsigned int seed );

    static uint32_t RNG_3( int position, unsigned int seed );

    /** @brief Serialization of RNG Object */
    virtual void Serialize( FArchive& Ar ) override;
};
