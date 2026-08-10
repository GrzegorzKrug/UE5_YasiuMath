/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#pragma once


#include "YasiuMathBP//YasiuMathBPLibrary.h"


#include "BPFunLib_Rotation.generated.h"


/**
* @brief BP Function library related to rotation and angles
 */
UCLASS(BlueprintType)
class UYasiuMathFL_Rotation : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    /** @brief Normalize angle to range <0, period>.
     * Unit independent.
     * 
     * Normalization removes any excess periods.
     * 
     * Angle can be negative.
     * Period must be > 0
     * 
     * @note Example: 
     * @note Angle = **-12**, Period = **10**
     * @note Result -> **8**
     * 
     */
    UFUNCTION(BlueprintCallable, Category = "YasiuMath|Angle")
    static double ClipAngleToCycle( double angle, double period );

    /**
     * @brief Rotate Box and calculate new bounding box along same axes to fix old box inside.213123
     *
     * Helps find suboptimal bounding box for rotated meshes
     * @note Function can take both Dimensions and Box Extent.
     * @param BoxSize Dimension of Bounding box
     * @param Rotation Rotation to calculate
     * @return Returns size in same units as passed **BoxSize**
     */
    UFUNCTION(BlueprintCallable, Category = "YasiuMath|Angle")
    static FVector RotateBoundingBox( const FVector& BoxSize, const FQuat& Rotation );

    /**
     * @brief Adjust rotator yaw and pitch and keeps roll 0.
     * 
     * @param CurrentRotator 
     * @param YawChange 
     * @param PitchChange 
     * @param PitchLimit Limits pitch value to <-PitchLimit, PitchLimit> 
     * @return New Rotator
     */
    UFUNCTION(BlueprintCallable, Category = "YasiuMath|Angle")
    static FRotator RotateCamera(
        const FRotator& CurrentRotator,
        float YawChange,
        float PitchChange,
        const float PitchLimit = 85
    );
};
