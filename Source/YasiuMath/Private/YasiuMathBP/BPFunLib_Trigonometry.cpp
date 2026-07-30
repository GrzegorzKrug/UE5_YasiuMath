/*
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
*/


#include "YasiuMathBP/BPFunLib_Trigonometry.h"


#include "YasiuMathLib.h"


void UYasiuMathFL_Trigonometry::SpreadPointsOnTangentByXY(
    TArray<FVector2D>& out,
    const double X,
    const double Y,
    const double spreadDistance
)
{
    std::vector<std::pair<double, double>> temp = YasiuMath::Trigonometry::SpreadPointsOnTangentByXY<
        double>(X, Y, spreadDistance);
    for ( std::pair<double, double> el : temp ) {
        out.Add(FVector2D{el.first, el.second});
    }
}


void UYasiuMathFL_Trigonometry::SpreadPointsOnTangentByAngleRadius(
    TArray<FVector2D>& points,
    const double angle,
    const double radius,
    const double spreadDistance
)
{
    const auto res = YasiuMath::Trigonometry::SpreadPointsOnTangentByAngleRadius<double>(angle, radius, spreadDistance);
    points.Add(FVector2D(res[0].first, res[0].second));
    points.Add(FVector2D(res[1].first, res[1].second));
}


double UYasiuMathFL_Trigonometry::FindMinimalRadiusForIntersectingTangentsOnArc(
    const double alfa,
    const double beta,
    const double symmetricWidth
)
{
    return YasiuMath::Trigonometry::FindMinimalRadiusForIntersectingTangentsOnArc<double>(alfa, beta, symmetricWidth);
}

FVector2D UYasiuMathFL_Trigonometry::FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric(
    const double alfa,
    const double beta,
    const double widthA,
    const double widthB
)
{
    auto temp = YasiuMath::Trigonometry::FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric<>(alfa, beta, widthA, widthB);
    return FVector2D{temp.first, temp.second};
}
