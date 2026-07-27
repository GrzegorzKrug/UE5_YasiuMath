#pragma once


#include "YasiuMathBP//YasiuMathBPLibrary.h"


#include "BPFunLib_Angle.generated.h"


/**
* @brief BP Function library containing algebra functions
 */
UCLASS(BlueprintType)
class UYasiuMathFL_Angle : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    /** @brief Normalize angle to its period, to be in range <0, period>
     *  @warning Does not support negative periods
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Angle")
    double ClipAngleToCycle( double angle, double period );

    /**
     * @brief Rotate Box and calculate new bounding box along same axes to fix old box inside.213123
     *
     * Helps find suboptimal bounding box for rotated meshes
     * @note Function can take both Dimensions and Box Extent.
     * @param BoxSize Dimension of Bounding box
     * @param Rotation Rotation to calculate
     * @return Returns size in same units as passed **BoxSize**
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Angle")
    FVector RotateBoundingBox( const FVector& BoxSize, const FQuat& Rotation );
};
