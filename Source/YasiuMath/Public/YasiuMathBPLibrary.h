#pragma once


#include "CoreMinimal.h"
#include "Templates/Tuple.h"

#include "YasiuMathBPLibrary.generated.h"


UCLASS()
class YASIUMATH_API UYasiuMathFunctionLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    // UFUNCTION(BlueprintCallable, Category = "Math|Custom")
    // FVector GetPoint( const FVector& StartPoint, const FVector& EndPoint, float Alpha );

    /**
     * 
     * @param out 
     * @param X 
     * @param Y 
     * @param spreadDistance 
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu")
    void SpreadPointsOnArcByXY( TArray<FVector2D>& out, const double X, const double Y, const double spreadDistance );

    /**
     * 
     * @param out 
     * @param angle 
     * @param radius 
     * @param spreadDistance 
     */
    UFUNCTION(BlueprintCallable, Category = "Math|Yasiu")
    void SpreadPointsOnArcByAngleRadius( TArray<FVector2D>& out, double angle, double radius, double spreadDistance );
};
