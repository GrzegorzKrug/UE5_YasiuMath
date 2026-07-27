#include "YasiuMathBP/BPFunLib_Angle.h"


#include "YasiuMathLib.h"
#include "YasiuMathUnrealLib.h"


double UYasiuMathFL_Angle::ClipAngleToCycle( double angle, double period )
{
    return YasiuMath::AngleUtils::NormalizeAngleToPeriod(angle, period);
}

FVector UYasiuMathFL_Angle::RotateBoundingBox( const FVector& BoxSize, const FQuat& Rotation )
{
    return YasiuMath::AngleUtils::RotateBoundingBox(BoxSize, Rotation);
}
