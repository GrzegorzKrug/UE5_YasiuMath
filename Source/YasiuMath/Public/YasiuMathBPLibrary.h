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
     * @param out 
     * @param X 
     * @param Y 
     * @param spreadDistance 
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trygonometry")
    void SpreadPointsOnTangentByXY( TArray<FVector2D>& out, const double X, const double Y, const double spreadDistance );

    /**
     * Spread points on tangent line to arc located by angle and radius of given arc.
     * Points are moved away from tangent point by spreadDistance
     * @tparam T 
     * @param angle 
     * @param radius 
     * @param spreadDistance 
     * @return 
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trygonometry")
    void SpreadPointsOnTangentByAngleRadius( TArray<FVector2D>& out, double angle, double radius, double spreadDistance );


    /**
     * Find radius of circle for both tangent lines that intersect.
     * Tangents on circle are defined by angle Alfa and Beta.
     * Tangent line distance to meeting point from circle intersection is `symmetricWidth`.
     * Angle is in radians.
     \f[
         R=\frac{\sin(\alpha)*A-\sin(\beta)*B}{\cos(\beta)-\cos(\alpha)}
     \f]
     * @param alfa angle of first tangent
     * @param beta  angle of second tangent
     * @param symmetricWidth width from tangent point to intersect
     */
    // UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trigonometry")
    UFUNCTION(BlueprintCallable)
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
     * @param alfa 
     * @param beta 
     * @param widthA 
     * @param widthB 
     * @return 
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Trygonometry")
    FVector2D FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric( double alfa, double beta, double widthA, double widthB );

    /**
     * Find points that create convex hull in 2D Space
     * @param result result array with point indexes
     * @param polygonPoints Points in 2D Space
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Convex")
    void ConvexHull2D( TArray<int>& result, const TArray<FVector2D>& polygonPoints );

    /** @brief Normalize angle to its period, to be in range <0, period>
     *  @warning Does not support negative periods
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|Convex")
    double ClipAngleToCycle( double angle, double period );
};
