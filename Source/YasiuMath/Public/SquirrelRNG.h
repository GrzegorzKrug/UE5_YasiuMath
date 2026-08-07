/*
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once


#include "Components/ActorComponent.h"


#include  "SquirrelRNG.generated.h"


/**
 * @brief Random number generator. Seedable.
 * 
 * Gives better random values than epic generator.
 * 
 * - **NetReplicated**: Yes
 * - **GameSave**: Yes
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced)
class YASIUMATH_API USquirrel13 : public UObject {
public:
    GENERATED_BODY()

    USquirrel13() {};

    USquirrel13( unsigned int seed )
        : m_seed(seed), init_seed(seed) {};

    USquirrel13( int position, unsigned int seed )
        : m_position(position), m_seed(seed), init_position(position), init_seed(seed) {};

    USquirrel13( int position, unsigned int seed, unsigned int variant )
        : m_position(position), m_seed(seed), m_variant(variant), init_position(position), init_seed(seed) {};

    virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

    /** @brief Serialization of RNG Object */
    virtual void Serialize( FArchive& Ar ) override;


protected:
    UPROPERTY(Replicated, SaveGame)
    int m_position{0};

    UPROPERTY(Replicated, SaveGame)
    uint32 m_seed{1};

    /** @brief variable related to selected noise variant */
    UPROPERTY(Replicated, SaveGame)
    int m_variant{0};

    UPROPERTY(Replicated)
    uint32 init_position = 0;

    UPROPERTY(Replicated)
    int32 init_seed = 0;


public:
    /**
     * This function provides access to change private variables for C++ object which is not accessible in Blueprints.
     * @brief Modify internal values to custom.
     */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void InitBP( int seed, int position = 0 );

    /** @brief Return random value scaled to integer range <min, max> at current position */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int GetCurrentInt( int min, int max ) const;

    /** @brief Move position by 1 and return random value as integer in range <min, max> */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int GetNextInt( int min, int max );

    /** @brief Return last random double in range <0, 1> */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    double GetCurrentDouble() const;

    /**
     * @brief Move position by 1 and return double in range <0, 1>
     */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    double GetNextDouble();

    /** @brief Get generator position */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int GetPosition() const;

    /** @brief Set generator position */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetPosition( int new_position );

    /** @brief Offset current position, can be positive and negative
     *
     * Position + Offset
     * */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void OffsetPosition( int offset = 1 );

    /** @brief Get generator position */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int32 GetSeed() const;

    /** @brief Set seed noise seed */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetSeed( int new_seed );

    /** @brief Internal noise variants (it is not initialized, but saved), default=0, max=5
     * Invalid variants will use default noise.
     */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetNoiseVariant( int newVariant = 0 );

    /** @brief Reset to internal initial values, InitBP must be used before! */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void ResetSeedPos();


protected:
    /** @internal Return current uint32 without changing position */
    virtual uint32 get_current_random() const;

    /** @internal Move position by 1 and return uint32 */
    uint32 get_next_random();

    /** @brief Noise variant */
    static uint32 RNG_0( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32 RNG_1( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32 RNG_2( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32 RNG_3( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32 RNG_4( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32 RNG_5( int position, unsigned int seed );
};


/** @brief Component with attached \ref USquirrel13 object
 * 
 * - **NetReplicated**: Yes
 * - **GameSave**: Yes
 */
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent), DefaultToInstanced)
class YASIUMATH_API USquirrel13_RNGComponent : public UActorComponent {
    GENERATED_BODY()


public:
    USquirrel13_RNGComponent();

    virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

    UPROPERTY(BlueprintReadWrite, Replicated, Category="Yasiu|RNG")
    TObjectPtr<USquirrel13> RNG;

    /** @brief Initialize values of RNG
     * 
     * @param seed 
     * @param pos 
     */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void InitBP( int seed, int pos = 0 );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int32 GetSeed() const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetSeed( int64 Seed );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int GetPosition() const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetPosition( int Pos );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG", BlueprintPure=false)
    int GetCurrentInt( int min, int max ) const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int GetNextInt( int min, int max );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG", BlueprintPure=false)
    double GetCurrentDouble() const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    double GetNextDouble();
};
