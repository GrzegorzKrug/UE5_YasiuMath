/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once


#include "CoreMinimal.h"


#include "Components//ActorComponent.h"


#include "PCG_RNG.generated.h"


/**
 * @brief State based RNG, good for MonteCarlo sampling.
 * 
 * - **NetReplicated**: Yes
 * - **GameSave**: Yes
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced)
class UPCG_RNG32 : public UObject {
    GENERATED_BODY()


public:
    virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

    /** @brief Initial set with 2 variables
     * @param StateIn - model state
     * @param StreamIn - Noise pattern to generate states
     */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void InitBP( int64 StateIn, int64 StreamIn );

    /** @brief Get current state in PCG */
    UFUNCTION(BlueprintCallable, Category="RNG")
    int64 GetState() const;

    /** @brief Set only state in PCG */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetState( int64 stateIn );

    /** @brief Set stream variable, used for generating next states. (must bee odd or will be changed to odd) */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetNoiseStream( int64 streamIn );

    /**
     * @brief Generate next random number
     */
    UFUNCTION(BlueprintCallable, Category="RNG")
    double GetNextDouble();

    /** @brief Return last random double in range <0, 1> */
    UFUNCTION(BlueprintCallable, Category="RNG")
    double GetCurrentDouble() const;

    /** @brief Get random value in range <A, B> */
    UFUNCTION(BlueprintCallable, Category="RNG")
    int GetNextInt( int A, int B );

    /** @brief Get random value in range <A, B> */
    UFUNCTION(BlueprintCallable, Category="RNG")
    int GetCurrentInt( int A, int B ) const;


protected:
    virtual uint32 CurrentNumber() const;

    uint32 NextNumber();

    UPROPERTY(Replicated, SaveGame)
    uint64 state = 0x853c49e6748fea9bULL;

    /** @brief Noise for generation numbers */
    UPROPERTY(Replicated, SaveGame)
    uint64 stream = 0xda3e39cb94b95bdbULL; // must be odd
};


/** @brief Component with attached \ref UPCG_RNG32 object
 * 
 * - **NetReplicated**: Yes
 * - **GameSave**: Yes
 */
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent), DefaultToInstanced)
class YASIUMATH_API UPCG32_RNGComponent : public UActorComponent {
    GENERATED_BODY()


public:
    UPCG32_RNGComponent();

    virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

    UPROPERTY(BlueprintReadWrite, Replicated, Category="Yasiu|RNG")
    TObjectPtr<UPCG_RNG32> RNG;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void InitBP( int64 State, int64 Noise );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int64 GetState() const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetState( int64 State );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    void SetStreamNoise( int64 Noise );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG", BlueprintPure=false)
    int GetCurrentInt( int min, int max ) const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    int GetNextInt( int min, int max );

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG", BlueprintPure=false)
    double GetCurrentDouble() const;

    UFUNCTION(BlueprintCallable, Category="Yasiu|RNG")
    double GetNextDouble();
};
