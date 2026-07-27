/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

#include "YasiuMathUnrealLib.h"
#include "CoreMinimal.h"
#include "YasiuMathLib.h"

FVector YasiuMath::AngleUtils::RotateBoundingBox( const FVector& Box, const FQuat& Rotation )
{
    // FVector BoxSize = BoxSize / 2;

    FVector X = Rotation.GetAxisX();
    FVector Y = Rotation.GetAxisY();
    FVector Z = Rotation.GetAxisZ();

    FVector NewBox;
    NewBox.X = FMath::Abs(X.X) * Box.X + FMath::Abs(Y.X) * Box.Y + FMath::Abs(Z.X) * Box.Z;
    NewBox.Y = FMath::Abs(X.Y) * Box.X + FMath::Abs(Y.Y) * Box.Y + FMath::Abs(Z.Y) * Box.Z;
    NewBox.Z = FMath::Abs(X.Z) * Box.X + FMath::Abs(Y.Z) * Box.Y + FMath::Abs(Z.Z) * Box.Z;

    return NewBox;
}

FVector YasiuMath::Algebra::RemapVector(
    const FVector& Input,
    const FVector2d& InputRange,
    const FVector2d& OutputRange,
    const bool ClampOutput
)
{
    const auto OutX = YasiuMath::Algebra::Remap<double>(
        Input.X,
        InputRange.X,
        InputRange.Y,
        OutputRange.X,
        OutputRange.Y,
        ClampOutput
    );
    const auto OutY = YasiuMath::Algebra::Remap<double>(
        Input.Y,
        InputRange.X,
        InputRange.Y,
        OutputRange.X,
        OutputRange.Y,
        ClampOutput
    );
    const auto OutZ = YasiuMath::Algebra::Remap<double>(
        Input.Z,
        InputRange.X,
        InputRange.Y,
        OutputRange.X,
        OutputRange.Y,
        ClampOutput
    );

    return FVector(OutX, OutY, OutZ);
}

FVector3f YasiuMath::Algebra::RemapVector(
    const FVector3f& Input,
    const FVector2f& InputRange,
    const FVector2f& OutputRange,
    const bool ClampOutput
)
{
    const auto OutX = YasiuMath::Algebra::Remap<float>(
        Input.X,
        InputRange.X,
        InputRange.Y,
        OutputRange.X,
        OutputRange.Y,
        ClampOutput
    );
    const auto OutY = YasiuMath::Algebra::Remap<float>(
        Input.Y,
        InputRange.X,
        InputRange.Y,
        OutputRange.X,
        OutputRange.Y,
        ClampOutput
    );
    const auto OutZ = YasiuMath::Algebra::Remap<float>(
        Input.Z,
        InputRange.X,
        InputRange.Y,
        OutputRange.X,
        OutputRange.Y,
        ClampOutput
    );

    return FVector3f(OutX, OutY, OutZ);
}
