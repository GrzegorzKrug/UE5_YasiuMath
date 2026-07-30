/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#include "YasiuMathBP/BPFunLib_Algebra.h"


#include "YasiuMathLib.h"
// #include "YasiuMathUnrealLib.h"


float UYasiuMathFL_Algebra::RemapFloat( const float Value, float MinIn, float MaxIn, float MinOut, float MaxOut, bool ClampOut )
{
    return YasiuMath::Algebra::Remap<float>(Value, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
}

double UYasiuMathFL_Algebra::RemapDouble(
    const double Value,
    double MinIn,
    double MaxIn,
    double MinOut,
    double MaxOut,
    bool ClampOut
)
{
    return YasiuMath::Algebra::Remap<double>(Value, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
}

FVector UYasiuMathFL_Algebra::RemapVector(
    const FVector& Value,
    double MinIn,
    double MaxIn,
    double MinOut,
    double MaxOut,
    bool ClampOut
)
{
    auto const X = YasiuMath::Algebra::Remap<float>(Value.X, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
    auto const Y = YasiuMath::Algebra::Remap<float>(Value.Y, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
    auto const Z = YasiuMath::Algebra::Remap<float>(Value.Z, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
    return {X, Y, Z};
}
