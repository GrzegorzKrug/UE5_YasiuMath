#pragma once


#include "YasiuMathBP/YasiuMathBPLibrary.h"
#include "YasiuMathLib.h"


#include "BPFunLib_Ballistic.generated.h"


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

    YasiuMath::Ballistics::InterceptorParams<double> ToDynamicObject() const;

    void FromDynamic( const YasiuMath::Ballistics::InterceptorParams<double>& Ob );
};


/**
* @brief BP Function library 
 */
UCLASS(BlueprintType)
class UYasiuMathFL_Ballistic : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    
    /* If air drag is 0 then it uses quick function O(1) */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Math|Yasiu|Ballistic", meta=(DisplayName="Remap"))
    FBallisticObject PredictPosition( const FBallisticObject& Ob, const float PredictTime, const float DeltaStep );
    
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Math|Yasiu|Ballistic", meta=(DisplayName="Remap"))
    FBallisticObject DiscreteStep( const FBallisticObject& Ob, const float DeltaStep );
    
    /* If air drag is 0 then it uses quick function O(1) */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Math|Yasiu|Ballistic", meta=(DisplayName="Remap"))
    FBallisticObject AutoStep( const FBallisticObject& Ob, const float PredictTime, const float DeltaStep );

    //
    // /**
    //  * 
    //  * @brief Remap float value to given range with optional clamping
    //  * @param Value Input value
    //  * @param MinIn Remap from Low
    //  * @param MaxIn Remap from High
    //  * @param MinOut Remap to Low
    //  * @param MaxOut Remap to High
    //  * @param ClampOut Optional Clamping to Output limits
    //  * @return 
    //  */
    // UFUNCTION(BlueprintCallable, BlueprintPure, Category="Math|Yasiu|Algebra", meta=(DisplayName="Remap"))
    // double RemapDouble( const double Value, double MinIn, double MaxIn, double MinOut, double MaxOut, bool ClampOut = true );
};
