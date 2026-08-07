/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#include "YasiuMathBP/BPFunLib_Rotation.h"


#include "YasiuMathLib.h"
#include "YasiuMathUnrealLib.h"


double UYasiuMathFL_Rotation::ClipAngleToCycle( double angle, double period )
{
    return YasiuMath::Rotation::NormalizeAngleToPeriod(angle, period);
}

FVector UYasiuMathFL_Rotation::RotateBoundingBox( const FVector& BoxSize, const FQuat& Rotation )
{
    return YasiuMath::Rotation::RotateBoundingBox(BoxSize, Rotation);
}

FRotator UYasiuMathFL_Rotation::RotateCamera(
    const FRotator& CurrentRotator,
    float YawChange,
    float PitchChange,
    const float PitchLimit
)
{
    FRotator Out = CurrentRotator;
    YasiuMath::Rotation::RotateCameraWithoutRoll(Out, YawChange, PitchChange, PitchLimit);
    return Out;
}
