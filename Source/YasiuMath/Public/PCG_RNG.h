/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once

#include "CoreMinimal.h"

#include "PCG_RNG.generated.h"


/**
 * @brief State based RNG, good for MonteCarlo
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced)
class UPCG32_RNG : public UObject {
    GENERATED_BODY()


public:
    uint32_t next();

    /** @brief Set state and stream in PCG */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void InitBP( int64 stateIn, int64 streamIn );

    /** @brief Set only state in PCG */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetState( int64 stateIn ) { state = static_cast<uint64_t>(stateIn); };

    /** @brief Set stream variable, used for generating next states */
    UFUNCTION(BlueprintCallable, Category="RNG")
    void SetStream( int64 streamIn ) { stream = static_cast<uint64_t>(streamIn); };

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

    /** @brief Function used to save object
     *  @private 
     * Will save state, stream and last random number as part of object
     */
    virtual void Serialize( FArchive& Ar ) override;


protected:
    uint64_t state = 0x853c49e6748fea9bULL;
    uint64_t stream = 0xda3e39cb94b95bdbULL; // must be odd
    uint32_t last = -1;
};
