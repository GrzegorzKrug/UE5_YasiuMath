/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */
/// \file

#pragma once

// #include "CoreMinimal.h"
#include "Math/MathFwd.h"


namespace YasiuMath {
    namespace AngleUtils {
        /**
        * Rotate Box and calculate new bounding box along same axes to fit old box inside.
         * Function can take both BoxDimensions and Box Extent.
         * @param Box
         * @param Rotation 
         * @return 
         */
        FVector YASIUMATH_API RotateBoundingBox( const FVector& Box, const FQuat& Rotation );

        // int YASIUMATH_API TemporarySum( const int A, const int B );
        //
        // int YASIUMATH_API FasterSum( const int A, const int B );

        // std::vector<double> RotateBoundingBox( const std::vector<double>& BoxSize, const std::vector<double>& Rotation );
    }
}
