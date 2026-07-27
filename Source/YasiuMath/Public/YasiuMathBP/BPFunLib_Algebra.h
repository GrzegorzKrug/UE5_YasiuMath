#pragma once


#include "YasiuMathBP//YasiuMathBPLibrary.h"


#include "BPFunLib_Algebra.generated.h"


/**
* @brief BP Function library 
 */
UCLASS(BlueprintType)
class UYasiuMathFL_Algebra : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    /**
     * 
     * @brief Remap float value to given range with optional clamping
     * @param Value Input value
     * @param MinIn Remap from Low
     * @param MaxIn Remap from High
     * @param MinOut Remap to Low
     * @param MaxOut Remap to High
     * @param ClampOut Optional Clamping to Output limits
     * @return 
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Math|Yasiu|Algebra", meta=(DisplayName="Remap"))
    float RemapFloat( const float Value, float MinIn, float MaxIn, float MinOut, float MaxOut, bool ClampOut = true );

    /**
     * 
     * @brief Remap float value to given range with optional clamping
     * @param Value Input value
     * @param MinIn Remap from Low
     * @param MaxIn Remap from High
     * @param MinOut Remap to Low
     * @param MaxOut Remap to High
     * @param ClampOut Optional Clamping to Output limits
     * @return 
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Math|Yasiu|Algebra", meta=(DisplayName="Remap"))
    double RemapDouble( const double Value, double MinIn, double MaxIn, double MinOut, double MaxOut, bool ClampOut = true );
};
