/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

#pragma once


#include "YasiuMathBP/YasiuMathBPLibrary.h"


#include "BPFunLib_ConvexHull.generated.h"


/**
* @brief BP Function library used with ConvexHull
 */
UCLASS(BlueprintType)
class UYasiuMathFL_ConvexHull : public UYasiuMathFunctionLibrary {
    GENERATED_BODY()


public:
    /**
     * @brief Find points that create convex hull in 2D Space
     * @param result Result array of input point indexes
     * @param polygonPoints Array of points in 2D Space
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu|ConvexHull")
    void ConvexHull2D( TArray<int>& result, const TArray<FVector2D>& polygonPoints );
};
