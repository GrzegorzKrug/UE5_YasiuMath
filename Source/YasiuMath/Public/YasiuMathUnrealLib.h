/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */
/* File for used in C++ but requires unreal variables or objects. */
/// \file

#pragma once

// #include "CoreMinimal.h"
#include "Math/MathFwd.h"


namespace YasiuMath {
    namespace AngleUtils {
        /**
         * @brief Rotate Box and calculate new bounding box along same axes to fit old box inside.
         * 
         * Helps find suboptimal bounding box for rotated meshes
         * @note Function can take both Dimensions and Box Extent.
         * @param Box Dimension of Bounding box
         * @param Rotation Rotation to calculate
         * @return Returns size in same units as passed **Box**
         */
        FVector YASIUMATH_API RotateBoundingBox( const FVector& Box, const FQuat& Rotation );

        // int YASIUMATH_API TemporarySum( const int A, const int B );

        // int YASIUMATH_API FasterSum( const int A, const int B );

        // std::vector<double> RotateBoundingBox( const std::vector<double>& DebugBoxSize, const std::vector<double>& Rotation );
    }
}
