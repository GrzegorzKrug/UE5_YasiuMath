/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#pragma once


#include "YasiuMathBP/YasiuMathBPLibrary.h"
#include "YasiuMathLib.h"


#include "BPFunLib_Ballistic.generated.h"


/**
 * @brief Type for simulating movement, see details how use...
 * 
 * Type reflecting \ref YasiuMath::Ballistics::ProjectileDynamicState
 * 
 * @note Check \ref UYasiuMathFL_Ballistics for usage
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
 * @brief Type for passing parameters, see details how use...
 * 
 * @note Check \ref UYasiuMathFL_Ballistics for usage
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
* @brief Function library used for projectile prediction and missile interception calculations
 */
UCLASS(BlueprintType)
class UYasiuMathFL_Ballistics : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    /** @brief Single iterative step 
     * 
     * @param Ob Struct with ballistic parameters
     * @param DeltaStep Single calculation time. Keep in small range <0.01, 1> (for accuracy) 
     * @return New Object state
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    FBallisticObject DiscreteStep( const FBallisticObject& Ob, const float DeltaStep );

    /** @brief Predict multiple steps in loop. Optimized for AirDrag=0, see details...
     * 
     * 
     * @param Ob Struct with ballistic parameters
     * @param PredictTime TimeRange to predict
     * @param DeltaStep Single calculation time. Keep in small range <0.01, 1> (for accuracy). 
     * Used only when calculating air drag.
     * 
     * @return New Object state
     * 
     * @note Normally function uses \ref DiscreteStep, but switchest to fast calculation when AirDrag is 0, effectively being **O(1)**
     * @note \ref YasiuMath::Ballistics::ProjectileDynamicState::AutoStep
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    FBallisticObject AutoStep( const FBallisticObject& Ob, const float PredictTime, const float DeltaStep );

    /** @brief Prediction function for linear objects.
     * Works only for objects with constant speeds, can not accelerate or be affected by air resistance
     * 
     * @param TargetPosition Target input position
     * @param TargetVelocity Target input velocity
     * @param InterceptSpeed Interceptor speed in any direction
     * @param OutLocation Output intercept location
     * @return Flag indicating if output location is valid.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    bool Intercept_Linear(
        const FVector& TargetPosition,
        const FVector& TargetVelocity,
        double InterceptSpeed,
        FVector& OutLocation
    );

    /** @brief Prediction function for objects with variable speed and resistance
     * @param Target Struct with dynamic parameters of target
     * @param Interceptor Struct with dynamic parameters of interceptor
     * @param OutLocation Output intercept location
     * @param MaxQueryTime Time range used to find solution.
     * @param DeltaStep Single calculation time. Keep in small range <0.01, 1> (for accuracy). 
     * @return Flag indicating if output location is valid.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Math|Yasiu|Ballistic")
    bool Intercept_Dynamic(
        const FBallisticObject& Target,
        const FBallisticInterceptor Interceptor,
        FVector& OutLocation,
        float MaxQueryTime = 10,
        float DeltaStep = 0.1
    );
};
