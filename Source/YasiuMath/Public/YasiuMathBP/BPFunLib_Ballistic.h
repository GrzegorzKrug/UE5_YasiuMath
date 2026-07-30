/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#pragma once


#include "YasiuMathBP/YasiuMathBPLibrary.h"
#include "YasiuMathLib.h"


#include "BPFunLib_Ballistic.generated.h"


/**
 * @brief Type reflecting \ref YasiuMath::Ballistics::ProjectileDynamicState
 */
USTRUCT(BlueprintType, Category="Math|Yasiu|Ballistic")
struct FBallisticObject {
    GENERATED_BODY()

    /** @brief Current position */
    UPROPERTY(BlueprintType)
    FVector Position;

    /** @brief Movement velocity vector */
    UPROPERTY(BlueprintType)
    FVector Velocity;

    /** @brief Acceleration or Deceleration or combined with Gravity Forces */
    UPROPERTY(BlueprintType)
    FVector Acceleration;

    /** @brief Maximal speed(length) of velocity vector */
    UPROPERTY(BlueprintType)
    float MaxSpeed = 0;

    /** @brief Movement resistance opposite to current velocity vector. Must be in <0, 1> range. More in details...
     *
     * $$Drag = Velocity^2 * Coeff$$
     */
    UPROPERTY(BlueprintType)
    float AirFrictionCoeff = 0;

    YasiuMath::Ballistics::ProjectileDynamicState<double> ToDynamicObject() const;

    FBallisticObject& FromDynamic( const YasiuMath::Ballistics::ProjectileDynamicState<double>& Ob );
};


/**
 * @brief Type reflecting \ref YasiuMath::Ballistics::InterceptorParams
 */
USTRUCT(BlueprintType, Category="Math|Yasiu|Ballistic")
struct FBallisticInterceptor {
    GENERATED_BODY()

    UPROPERTY(BlueprintType)
    FVector Position;

    /** @brief Speed at which object starts moving */
    UPROPERTY(BlueprintType)
    double InitialSpeed = 0;

    /** @brief Acceleration in any direction */
    UPROPERTY(BlueprintType)
    double Acceleration = 0;

    /** @brief Max speed object can reach with acceleration, 0 or less is ignored */
    UPROPERTY(BlueprintType)
    double MaxSpeed = 0;

    /** @brief Keep in <0, 1> range! */
    UPROPERTY(BlueprintType)
    double AirResistance = 0;

    YasiuMath::Ballistics::InterceptorParams<double> ToInterceptor() const;

    void FromInterceptor( const YasiuMath::Ballistics::InterceptorParams<double>& Ob );
};


/**
* @brief BP Function library 
 */
UCLASS(BlueprintType)
class UYasiuMathFL_Ballistic : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    /* Single iterative step */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    FBallisticObject DiscreteStep( const FBallisticObject& Ob, const float DeltaStep );

    /* If air drag is 0 then it uses quick function O(1) */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    FBallisticObject MultiStep( const FBallisticObject& Ob, const float PredictTime, const float DeltaStep );

    /** @brief Prediction works only for objects with constant speeds, can not accelerate or be affected by air resistance */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    bool Intercept_Linear(
        const FVector& TargetPosition,
        const FVector& TargetVelocity,
        double InterceptSpeed,
        FVector& OutLocation
    );

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    bool Intercept_Dynamic(
        const FBallisticObject& Target,
        const FBallisticInterceptor Interceptor,
        FVector& OutLocation,
        float MaxQueryTime = 10,
        float DeltaStep = 0.1
    );
};
