/* 
* Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#include "YasiuMathBP/BPFunLib_ConvexHull.h"


#include "YasiuMathLib.h"
#include <memory>


void UYasiuMathFL_ConvexHull::ConvexHull2D( TArray<int>& result, const TArray<FVector2D>& polygonPoints )
{
    auto vect = std::make_shared<std::vector<std::pair<double, double>>>();
    vect->reserve(polygonPoints.Num());
    for ( int i = 0; i < polygonPoints.Num(); i++ ) {
        vect->push_back(std::pair<double, double>{polygonPoints[i].X, polygonPoints[i].Y});
    }
    std::vector<int> res = YasiuMath::ConvexHull::ConvexHull2D(*vect);

    result.Empty();
    for ( auto r : res ) {
        result.Add(r);
    }
}
