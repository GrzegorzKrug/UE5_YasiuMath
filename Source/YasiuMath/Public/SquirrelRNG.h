/*
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once


#include "Components/ActorComponent.h"


#include  "SquirrelRNG.generated.h"


/**
 * @brief Random number generator. Seedable. Gives better random values than epic generator.
 * 
 * Supports serialization for saving/loading
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced)
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

    virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

    /** @brief Serialization of RNG Object */
    virtual void Serialize( FArchive& Ar ) override;


protected:
    UPROPERTY(Replicated)
    int m_position{0};

    UPROPERTY(Replicated)
    unsigned int m_seed{1};

    /** @brief variable related to selected noise variant */
    UPROPERTY(Replicated)
    int m_variant{0};

    UPROPERTY(Replicated)
    unsigned int init_position = 0;

    // UPROPERTY(BlueprintReadWrite)
    UPROPERTY(Replicated)
    int32 init_seed = 0;


public:
    /**
     * This function provides access to change private variables for C++ object which is not accessible in Blueprints.
     * @brief Modify internal values to custom.
     */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void InitBP( int seed, int position = 0 );

    /** @internal Return current uint32_t without changing position */
    uint32_t get_current_random() const;

    /** @internal Move position by 1 and return uint32_t */
    uint32_t get_next_random();

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

    /** @brief Internal noise variants (it is not initialized), default=0, max=5
     * Invalid variants will use default noise.
     */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetNoiseVariant( int newVariant = 0 );

    /** @brief Reset to internal initial values */
    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void ResetSeedPos();


protected:
    /** @brief Noise variant */
    static uint32_t RNG_0( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32_t RNG_1( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32_t RNG_2( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32_t RNG_3( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32_t RNG_4( int position, unsigned int seed );

    /** @brief Noise variant */
    static uint32_t RNG_5( int position, unsigned int seed );
};


/** @brief Component with attached \ref USquirrel13_RNG object
 * 
 * Component has Network replication support */
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent), DefaultToInstanced)
class YASIUMATH_API USquirrel13_RNGComponent : public UActorComponent {
    GENERATED_BODY()


public:
    USquirrel13_RNGComponent();

    virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

    UPROPERTY(BlueprintReadWrite, Replicated, Category="Yasiu|RNG")
    TObjectPtr<USquirrel13_RNG> RNG;

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
