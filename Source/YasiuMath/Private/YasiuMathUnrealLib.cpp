/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
*/

#include "YasiuMathUnrealLib.h"
#include "CoreMinimal.h"

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
