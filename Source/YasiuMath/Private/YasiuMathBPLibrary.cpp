#include "YasiuMathBPLibrary.h"

#include "YasiuMathStaticLib.h"

// std::vector<std::pair<T, T>> SpreadPointsOnArcByXY( const T X, const T Y, const T spreadDistance )
//
// TArray<UYasiuMathFunctionLibrary::GetPoint(const FVector & StartPoint, const FVector & EndPoint, float Alpha)
// {
//     /**/
//     return YasiuMath::Trigonometry::SpreadPointsOnArcByXY<double>()
// }

void UYasiuMathFunctionLibrary::SpreadPointsOnArcByXY(
    TArray<FVector2D>& out,
    const double X,
    const double Y,
    const double spreadDistance
)
{
    std::vector<std::pair<double, double>> temp = YasiuMath::Trigonometry::SpreadPointsOnArcByXY<double>(X, Y, spreadDistance);
    for ( std::pair<double, double> el : temp ) {
        out.Add(FVector2D{el.first, el.second});
    }
}


void UYasiuMathFunctionLibrary::SpreadPointsOnArcByAngleRadius(
    TArray<FVector2D>& out,
    const double angle,
    const double radius,
    const double spreadDistance
)
{
    auto temp = YasiuMath::Trigonometry::SpreadPointsOnArcByAngleRadius<double>(angle, radius, spreadDistance);
    for ( std::pair<double, double> el : temp ) {
        out.Add(FVector2D{el.first, el.second});
    }
}
