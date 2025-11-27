/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */
#pragma once


#include "CoreMinimal.h"
#include "Templates/Tuple.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "YasiuMathBPLibrary.generated.h"


/**
 * BP Function library having all important functions exposed for BP Editor
 */
UCLASS(BlueprintType)
class UYasiuMathFunctionLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * 
     * @param X Position X
     * @param Y Position Y
     * @param spreadDistance Distance to spread in one direction from tangent point
     * @returns **TArray** of 2 points
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trigonometry")
    void SpreadPointsOnTangentByXY( TArray<FVector2D>& out, const double X, const double Y, const double spreadDistance );

    /**
     * Spread points on tangent line to arc located by angle and radius of given arc.
     * Points are moved away from tangent point by spreadDistance
     * @param angle Tangent location defined by angle
     * @param radius Radius of arc
     * @param spreadDistance Distance to spread in one direction from tangent point
     * @returns **TArray** of 2 points
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trigonometry")
    void SpreadPointsOnTangentByAngleRadius( TArray<FVector2D>& out, double angle, double radius, double spreadDistance );


    /**
     * Find radius of circle for both tangent lines that intersect.
     * Tangents on circle are defined by angle Alfa and Beta.
     * Tangent line distance to meeting point from circle intersection is `symmetricWidth`.
     * Angle is in radians.
     \f[
         R=\frac{\sin(\alpha)*A-\sin(\beta)*B}{\cos(\beta)-\cos(\alpha)}
     \f]
     * @param alfa Tangent point location defined by angle
     * @param beta Tangent point location defined by angle
     * @param symmetricWidth Width from tangent point to intersect
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trigonometry")
    double FindMinimalRadiusForIntersectingTangentsOnArc( double alfa, double beta, double symmetricWidth );

    /**
     * Find radius of circle for both tangent line that intersect. Asymmetry comes from 2 different widths.
     * Tangents on circle are defined by angle Alfa and Beta.
     * Tangent line distance to meeting point from circle intersection is `symmetricWidth`.
     * Angle is in radians.
     * First radius is bigger.
     \f[
         R=\frac{\sin(\alpha)*A-\sin(\beta)*B}{\cos(\beta)-\cos(\alpha)}
     \f]
     * @param alfa Tangent point location defined by angle
     * @param beta Tangent point location defined by angle
     * @param widthA Distance from intersection for point A
     * @param widthB Distance from intersection for point B
     * @return 
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trigonometry")
    FVector2D FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric( double alfa, double beta, double widthA, double widthB );

    /**
     * Find points that create convex hull in 2D Space
     * @param result Result array of input point indexes
     * @param polygonPoints Array of points in 2D Space
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Convex")
    void ConvexHull2D( TArray<int>& result, const TArray<FVector2D>& polygonPoints );

    /** @brief Normalize angle to its period, to be in range <0, period>
     *  @warning Does not support negative periods
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Convex")
    double ClipAngleToCycle( double angle, double period );
};
