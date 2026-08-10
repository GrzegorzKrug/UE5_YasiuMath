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
USTRUCT(BlueprintType, Category="YasiuMath|Ballistics")
struct FBallisticObject {
    GENERATED_BODY()

    /** @brief Current position */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    FVector Position;

    /** @brief Movement velocity vector */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    FVector Velocity;

    /** @brief Acceleration or Deceleration or combined with Gravity Forces */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    FVector Acceleration;

    /** @brief Maximal speed(length) of velocity vector */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    float MaxSpeed = 0;

    /** @brief Movement resistance opposite to current velocity vector. Must be in <0, 1> range. More in details...
     *
     * $$Drag = Velocity^2 * Coeff$$
     */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    float AirFrictionCoeff = 0;

    YasiuMath::Ballistics::ProjectileDynamicState<double> ToDynamicObject() const;

    FBallisticObject& FromDynamic( const YasiuMath::Ballistics::ProjectileDynamicState<double>& Ob );
};


/**
 * @brief Type for passing parameters, see details how use...
 * 
 * @note Check \ref UYasiuMathFL_Ballistics for usage
 */
USTRUCT(BlueprintType, Category="YasiuMath|Ballistics")
struct FBallisticInterceptor {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    FVector Position;

    /** @brief Speed at which object starts moving, should move towards correct location! 
     * 
     * Objects moving backwards will yield wrong result.
     */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    double InitialSpeed = 0;

    /** @brief Acceleration in any direction */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    double Acceleration = 0;

    /** @brief Max speed object can reach with acceleration, 0 or less is ignored */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
    double MaxSpeed = 0;

    /** @brief Keep in <0, 1> range! */
    UPROPERTY(BlueprintReadWrite, Category="YasiuMath")
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
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="YasiuMath|Ballistics")
    static FBallisticObject DiscreteStep( const FBallisticObject& Ob, const float DeltaStep );

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
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="YasiuMath|Ballistics")
    static FBallisticObject AutoStep( const FBallisticObject& Ob, const float PredictTime, const float DeltaStep );

    /** @brief Prediction function for linear objects.
     * Works only for objects with constant speeds, can not accelerate or be affected by air resistance
     * 
     * @param TargetPosition Target input position
     * @param TargetVelocity Target input velocity
     * @param InterceptSpeed Interceptor speed in any direction
     * @param HitLocation Output intercept location
     * @param HitTime
     * @return Flag indicating if output location is valid.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="YasiuMath|Ballistics")
    static bool Intercept_Linear(
        const FVector& TargetPosition,
        const FVector& TargetVelocity,
        double InterceptSpeed,
        FVector& HitLocation,
        double& HitTime
    );

    /** @brief Prediction function for objects with variable speed and resistance.
     * 
     * @param Target Struct with dynamic parameters of target
     * @param Interceptor Struct with dynamic parameters of interceptor
     * @param MaxQueryTime Time range used to find solution.
     * @param StepTime Single calculation time. Keep in small range <0.1, 1> (for accuracy, but can use higher values).
     * @param AccurateStepTime Accurate step time to reduce overshoot error. 0=Ignore
     * @param HitLocation Output intercept location
     * @param HitTime Estimated time to hit 
     * @return Flag indicating if output location is valid.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="YasiuMath|Ballistics")
    static bool Intercept_Dynamic(
        const FBallisticObject& Target,
        const FBallisticInterceptor Interceptor,
        double MaxQueryTime,
        double StepTime,
        double AccurateStepTime,
        FVector& HitLocation,
        double& HitTime
    );
};
