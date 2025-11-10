#include "YasiuMathBPLibrary.h"

#include "YasiuMathStaticLib.h"

// std::vector<std::pair<T, T>> SpreadPointsOnArcByXY( const T X, const T Y, const T spreadDistance )
//
// TArray<UYasiuMathFunctionLibrary::GetPoint(const FVector & StartPoint, const FVector & EndPoint, float Alpha)
// {
//     /**/
//     return YasiuMath::Trigonometry::SpreadPointsOnArcByXY<double>()
// }

void UYasiuMathFunctionLibrary::SpreadPointsOnTangentByXY(
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


void UYasiuMathFunctionLibrary::SpreadPointsOnTangentByAngleRadius(
    TArray<FVector2D>& out,
    const double angle,
    const double radius,
    const double spreadDistance
)
{
    auto temp = YasiuMath::Trigonometry::SpreadPointsOnTangentByAngleRadius<double>(angle, radius, spreadDistance);
    for ( std::pair<double, double> el : temp ) {
        out.Add(FVector2D{el.first, el.second});
    }
}


double UYasiuMathFunctionLibrary::FindMinimalRadiusForIntersectingTangentsOnArc(
    const double alfa,
    const double beta,
    const double symmetricWidth
)
{
    return YasiuMath::Trigonometry::FindMinimalRadiusForIntersectingTangentsOnArc<double>(alfa, beta, symmetricWidth);
}

FVector2D UYasiuMathFunctionLibrary::FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric(
    const double alfa,
    const double beta,
    const double widthA,
    const double widthB
)
{
    auto temp = YasiuMath::Trigonometry::FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric<>(alfa, beta, widthA, widthB);
    return FVector2D{temp.first, temp.second};
}

void UYasiuMathFunctionLibrary::ConvexHull2D( TArray<int>& result, const TArray<FVector2D>& polygonPoints )
{
    auto vect = std::make_shared<std::vector<std::pair<double, double>>>();
    vect->reserve(polygonPoints.Num());
    for ( int i = 0; i < polygonPoints.Num(); i++ ) {
        vect->push_back(std::pair<double, double>{polygonPoints[i].X, polygonPoints[i].Y});
    }
    std::vector<int> res = YasiuMath::ConvexHull::ConvexHull(*vect);

    for ( auto r : res ) {
        result.Add(r);
    }
}
