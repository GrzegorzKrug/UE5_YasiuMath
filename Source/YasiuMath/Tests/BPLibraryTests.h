#include "Misc/AutomationTest.h"
#include "YasiuMathBPLibrary.h"

/*
 * 3 Plugins in game project
 * RuntimeTest EditorTests FunctionalTestingEditor
 */


// Define a simple automation test
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BasicCallTests,
    "Plugins.Yasiu.Math.Base",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool BasicCallTests::RunTest( const FString& Parameters )
{
    TArray<int> resultsInt{};
    TArray<FVector2D> resultsVector2{};

    UYasiuMathFunctionLibrary* MathLib = NewObject<UYasiuMathFunctionLibrary>();
    MathLib->ClipAngleToCycle(5, 5);

    MathLib->SpreadPointsOnTangentByXY(resultsVector2, 5, 5, 10);
    MathLib->SpreadPointsOnTangentByAngleRadius(resultsVector2, 50, 15, 5);
    MathLib->FindMinimalRadiusForIntersectingTangentsOnArc(4, 10, 5);
    MathLib->FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric(10, 25, 5, 10);

    TArray<FVector2D> points{{2, 4}, {5, 4}, {3, 4}, {10, 10}};
    MathLib->ConvexHull2D(resultsInt, points);


    MathLib->ClipAngleToCycle(350, 10);

    FVector Box{50, 50, 50};
    FQuat rotation{};
    MathLib->RotateBoundingBox(Box, rotation);

    return true; // Return true if test completed successfully
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    MathBoxRotation,
    "Plugins.Yasiu.Math.Box",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool MathBoxRotation::RunTest( const FString& Parameters )
{
    constexpr float YASIU_EPS = 0.0001f;
    UYasiuMathFunctionLibrary* MathLib = NewObject<UYasiuMathFunctionLibrary>();

    FVector Box{100, 100, 100};
    FRotator rotator{};
    rotator.Yaw = 45;
    float size = Box.X;

    float newSize = FMath::Sqrt(size * size + size * size);

    FVector result = MathLib->RotateBoundingBox(Box, rotator.Quaternion());
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.X), newSize, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Y), newSize, YASIU_EPS);

    rotator.Yaw = -45;
    result = MathLib->RotateBoundingBox(Box, rotator.Quaternion());
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.X), newSize, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Y), newSize, YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    MathBoxPillarRotation,
    "Plugins.Yasiu.Math.Box2",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool MathBoxPillarRotation::RunTest( const FString& Parameters )
{
    constexpr float YASIU_EPS = 0.0001f;
    UYasiuMathFunctionLibrary* MathLib = NewObject<UYasiuMathFunctionLibrary>();

    FVector Box{65, 150, 352};
    FRotator rotator{0, 0, 0};
    FVector result;


    rotator.Yaw = 90;
    result = MathLib->RotateBoundingBox(Box, rotator.Quaternion());
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.X), 150, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Y), 65, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Z), 352, YASIU_EPS);

    rotator.Yaw = 0;
    rotator.Pitch = 90;
    result = MathLib->RotateBoundingBox(Box, rotator.Quaternion());
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.X), 352, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Y), 150, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Z), 65, YASIU_EPS);

    rotator.Pitch = 0;
    rotator.Roll = 90;
    result = MathLib->RotateBoundingBox(Box, rotator.Quaternion());
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.X), 65, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Y), 352, YASIU_EPS);
    TestNearlyEqual(TEXT("Is box close"), StaticCast<float>(result.Z), 150, YASIU_EPS);


    return true;
}
