/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

/* File for used in C++ but requires unreal variables or objects. */

/// \file

#pragma once


#include "Math/MathFwd.h"


namespace YasiuMath {
    namespace Rotation {
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

        /**
         * @brief Adjust rotator yaw and pitch and keeps roll 0.
         * 
         * @param CurrentRotator 
         * @param YawChange 
         * @param PitchChange 
         * @param PitchLimit Limits pitch value to <-PitchLimit, PitchLimit> 
         */
        void YASIUMATH_API RotateCameraWithoutRoll(
            FRotator& CurrentRotator,
            float YawChange,
            float PitchChange,
            const float PitchLimit = 85
        );
    }


    namespace Algebra {
        /**
        * @brief Remaps Value between **input** range to **output** range with optional clamping.
        * @param Input Input value
        * @param InputRange Input range vector consisting of <Low, High>
        * @param OutputRange Output range vector consisting of <Low, High>
        * @param ClampOutput Flag to clamp output
        * @return Value
        */
        FVector YASIUMATH_API RemapVector(
            const FVector& Input,
            const FVector2d& InputRange,
            const FVector2d& OutputRange,
            const bool ClampOutput
        );

        /**
        * @brief Remaps Value between **input** range to **output** range with optional clamping.
        * @param Input Input value
        * @param InputRange Input range vector consisting of <Low, High>
        * @param OutputRange Output range vector consisting of <Low, High>
        * @param ClampOutput Flag to clamp output
        * @return Value
        */
        FVector3f YASIUMATH_API RemapVector(
            const FVector3f& Input,
            const FVector2f& InputRange,
            const FVector2f& OutputRange,
            const bool ClampOutput
        );
    }
}
