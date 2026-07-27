#include "YasiuMathBP/BPFunLib_Algebra.h"


#include "YasiuMathLib.h"


float UYasiuMathFL_Algebra::RemapFloat(
    const float Value,
    float MinIn,
    float MaxIn,
    float MinOut,
    float MaxOut,
    bool ClampOut
)
{
    return YasiuMath::Algebra::Remap<float>(Value, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
}

double UYasiuMathFL_Algebra::RemapDouble(
    const double Value,
    double MinIn,
    double MaxIn,
    double MinOut,
    double MaxOut,
    bool ClampOut
)
{
    return YasiuMath::Algebra::Remap<double>(Value, MinIn, MaxIn, MinOut, MaxOut, ClampOut);
}
