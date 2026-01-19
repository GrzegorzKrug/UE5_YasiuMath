#pragma once

#include <vector>
#include "Misc/AutomationTest.h"
#include "Serialization/BufferArchive.h"

#include "YasiuMathBPLibrary.h"
#include "SquirrelRNG.h"
#include "PCG_RNG.h"

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    RNG_Test,
    "Plugins.Yasiu.Math.RNG.Base1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool RNG_Test::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001;
    auto* RNG = NewObject<USquirrel13_RNG>();

    int seed = 123;
    RNG->SetPosition(5);
    RNG->SetSeed(seed);
    double ret = RNG->GetNextDouble();
    TestNearlyEqual(TEXT("Is same 1?"), ret, RNG->GetCurrentDouble(), YASIU_EPS);

    int ret2{};
    ret2 = RNG->GetNextInt(0, 5000);
    TestEqual(TEXT("Is same 2?"), ret2, RNG->GetCurrentInt(0, 5000));
    ret2 = RNG->GetNextInt(-900, 500);
    TestEqual(TEXT("Is same 3?"), ret2, RNG->GetCurrentInt(-900, 500));

    RNG->SetPosition(450);
    RNG->SetSeed(184);
    RNG->SetNoiseVariant(1);

    FBufferArchive arch{};
    arch.SetIsLoading(false);
    arch.SetIsSaving(true);
    RNG->Serialize(arch);
    // FArchive saved = arch;
    double valAfterSave = RNG->GetNextDouble();

    RNG->SetPosition(450);
    RNG->SetSeed(184);
    RNG->SetNoiseVariant(1);
    TestNearlyEqual(TEXT("Is same before saving ?"), valAfterSave, RNG->GetNextDouble(), YASIU_EPS);

    RNG->GetNextDouble();
    TestNotEqual(TEXT("Should be different 1"), valAfterSave, RNG->GetNextDouble());
    RNG->SetPosition(12);
    RNG->SetSeed(13);
    RNG->SetNoiseVariant(3);

    TestNotEqual(TEXT("Should be different 2"), valAfterSave, RNG->GetCurrentDouble());
    TestNotEqual(TEXT("Should be different 3"), valAfterSave, RNG->GetNextDouble());

    auto arch2 = FMemoryReader(arch);
    arch2.SetIsLoading(true);
    arch2.SetIsSaving(false);

    auto RNG_2 = NewObject<USquirrel13_RNG>();
    RNG_2->Serialize(arch2);
    TestNearlyEqual(TEXT("Is same after loading 1?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);
    // TestNearlyEqual(TEXT("Is same after loading 2?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);
    // TestNearlyEqual(TEXT("Is same after loading 3?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);
    // TestNearlyEqual(TEXT("Is same after loading 4?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);
    // TestNearlyEqual(TEXT("Is same after loading 5?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);
    // TestNearlyEqual(TEXT("Is same after loading 6?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);

    /* Reset */
    RNG_2->SetPosition(450);
    RNG_2->SetSeed(184);
    RNG_2->SetNoiseVariant(1);
    TestNearlyEqual(TEXT("Is same after reset?"), valAfterSave, RNG_2->GetNextDouble(), YASIU_EPS);

    return true;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    RNG_Test2,
    "Plugins.Yasiu.Math.RNG.Base2",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* TESTING VALUES */
bool RNG_Test2::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001;
    auto* RNG = NewObject<USquirrel13_RNG>();

    const int seed = 66321;
    const int pos = 4312;

    RNG->InitBP(seed, pos);

    double VerifyVal1 = RNG->get_next_random();
    TestNearlyEqual(TEXT("Is same number7?"), RNG->get_current_random(), VerifyVal1, YASIU_EPS);

    RNG->ResetSeedPos();
    TestNearlyEqual(TEXT("Is same number3?"), RNG->get_next_random(), VerifyVal1, YASIU_EPS);

    RNG->OffsetPosition(-1);
    TestNearlyEqual(TEXT("Is same number5?"), RNG->get_next_random(), VerifyVal1, YASIU_EPS);

    RNG->ResetSeedPos();
    auto VerifyVal2 = RNG->GetNextDouble();
    // TestNearlyEqual(TEXT("Is same number1?"), RNG->GetNextDouble(), VerifyVal, YASIU_EPS);
    TestNearlyEqual(TEXT("Is same number4?"), RNG->GetCurrentDouble(), VerifyVal2, YASIU_EPS);
    RNG->OffsetPosition(-1);
    TestNearlyEqual(TEXT("Is same number5?"), RNG->GetNextDouble(), VerifyVal2, YASIU_EPS);

    RNG->InitBP(442424, 15151);
    double VerifyInt = RNG->GetNextInt(-5000, 5000);
    RNG->ResetSeedPos();
    TestNearlyEqual(TEXT("Is same INT number1?"), RNG->GetNextInt(-5000, 5000), VerifyInt, YASIU_EPS);
    TestNearlyEqual(TEXT("Is same INT number2?"), RNG->GetCurrentInt(-5000, 5000), VerifyInt, YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    RNG_BucketTest1,
    "Plugins.Yasiu.Math.RNG.Buckets.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


inline bool RNG_BucketTest1::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001;
    auto* RNG = NewObject<USquirrel13_RNG>();


    /* Hardcoded lazy case */
    constexpr int bucketsNum = 10;

    /* Accuracy of noise */
    std::vector<std::pair<int, float>> QualityParams = {{10000, 0.03}, {1000, 0.1}, {100, 0.3}};

    unsigned int Buckets[bucketsNum];

    /* Pairs: SEED / Pos */
    std::vector<std::pair<int, int>> PosSeedVector = {
        {-5001, 901},
        {-515, 151},
        {-3, 198},
        {1, 1},
        {3, 3},
        {15, 15},
        {124, 1515},
        {144, 15},
        {150, -125},
        {12344, 14515},
        {33044, 1665},
        {66321, 5161},
        {2137, 69000},
        {27, -15}
    };

    for ( auto qualityPair : QualityParams ) {
        const unsigned int bucketSize = qualityPair.first;
        const float AllowanceMargin = qualityPair.second;
        const unsigned int bucketMinSize = bucketSize * (1. - AllowanceMargin);
        const unsigned int bucketMaxSize = bucketSize * (1. + AllowanceMargin);
        const unsigned int randomNumbers = bucketSize * bucketsNum;

        for ( auto PosSeedPair : PosSeedVector ) {
            const int seed = PosSeedPair.first;
            const int pos = PosSeedPair.second;

            for ( int var = 0; var <= 5; var++ ) {
                std::fill(Buckets, Buckets + 10, 0);
                RNG->InitBP(seed, pos);
                RNG->SetNoiseVariant(var);

                /* Generate random data */
                for ( unsigned int i = 0; i < randomNumbers; i++ ) {
                    auto val = RNG->GetNextDouble();
                    if ( val < 0.1 ) {
                        Buckets[0] += 1;
                    }
                    else if ( val < 0.2 ) {
                        Buckets[1] += 1;
                    }
                    else if ( val < 0.3 ) {
                        Buckets[2] += 1;
                    }
                    else if ( val < 0.4 ) {
                        Buckets[3] += 1;
                    }
                    else if ( val < 0.5 ) {
                        Buckets[4] += 1;
                    }
                    else if ( val < 0.6 ) {
                        Buckets[5] += 1;
                    }
                    else if ( val < 0.7 ) {
                        Buckets[6] += 1;
                    }
                    else if ( val < 0.8 ) {
                        Buckets[7] += 1;
                    }
                    else if ( val < 0.9 ) {
                        Buckets[8] += 1;
                    }
                    else {
                        Buckets[9] += 1;
                    }
                }

                /* Verify results */
                for ( int i = 0; i < bucketsNum; i++ ) {
                    auto val = Buckets[i];
                    auto testStr = FString::Format(
                        TEXT("Bucket{0}: Margins: {1}, {2}, but was: {3} (seed:{4}, pos:{5}, var:{6})"),
                        {i, bucketMinSize, bucketMaxSize, val, seed, pos, var}
                    );
                    TestTrue(testStr, ((val >= bucketMinSize) && (val <= bucketMaxSize)));
                }
            }
        }
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    RNG_BucketTest2,
    "Plugins.Yasiu.Math.RNG.Buckets.2",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


inline bool RNG_BucketTest2::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001;
    auto* RNG = NewObject<UPCG32_RNG>();


    /* Hardcoded lazy case */
    constexpr int bucketsNum = 10;

    /* Accuracy of noise */
    std::vector<std::pair<int, float>> QualityParams = {{10000, 0.03}, {1000, 0.1}, {100, 0.3}};

    unsigned int Buckets[bucketsNum];

    /* Pairs: state / stream */
    std::vector<std::pair<int, int>> PosSeedVector = {
        {-5001, 901},
        {-515, 151},
        {-3, 198},
        {1, 1},
        {3, 3},
        {15, 15},
        {124, 1515},
        {144, 15},
        {150, -125},
        {12324, 14515},
        {33044, 1665},
        {66321, 5161},
        {2137, 69000},
        {27, -15}
    };

    for ( auto qualityPair : QualityParams ) {
        const unsigned int bucketSize = qualityPair.first;
        const float AllowanceMargin = qualityPair.second;
        const unsigned int bucketMinSize = bucketSize * (1. - AllowanceMargin);
        const unsigned int bucketMaxSize = bucketSize * (1. + AllowanceMargin);
        const unsigned int randomNumbers = bucketSize * bucketsNum;

        for ( auto PosSeedPair : PosSeedVector ) {
            const int seed = PosSeedPair.first;
            const int pos = PosSeedPair.second;

            std::fill(Buckets, Buckets + 10, 0);
            RNG->InitBP(seed, pos);

            /* Generate random data */
            for ( unsigned int i = 0; i < randomNumbers; i++ ) {
                auto val = RNG->GetNextDouble();
                if ( val < 0.1 ) {
                    Buckets[0] += 1;
                }
                else if ( val < 0.2 ) {
                    Buckets[1] += 1;
                }
                else if ( val < 0.3 ) {
                    Buckets[2] += 1;
                }
                else if ( val < 0.4 ) {
                    Buckets[3] += 1;
                }
                else if ( val < 0.5 ) {
                    Buckets[4] += 1;
                }
                else if ( val < 0.6 ) {
                    Buckets[5] += 1;
                }
                else if ( val < 0.7 ) {
                    Buckets[6] += 1;
                }
                else if ( val < 0.8 ) {
                    Buckets[7] += 1;
                }
                else if ( val < 0.9 ) {
                    Buckets[8] += 1;
                }
                else {
                    Buckets[9] += 1;
                }
            }

            /* Verify results */
            for ( int i = 0; i < bucketsNum; i++ ) {
                auto val = Buckets[i];
                auto testStr = FString::Format(
                    TEXT("Bucket{0}: Margins: {1}, {2}, but was: {3} (seed:{4}, pos:{5} "),
                    {i, bucketMinSize, bucketMaxSize, val, seed, pos}
                );
                TestTrue(testStr, ((val >= bucketMinSize) && (val <= bucketMaxSize)));
            }
        }
    }

    return true;
}
