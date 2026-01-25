#pragma once

#include <vector>
#include "Misc/AutomationTest.h"
#include "YasiuMathLib.h"
#include "CoreMinimal.h"
#include "YasiuMathBPLibrary.h"

/*
 * 3 Plugins in game project
 * RuntimeTest EditorTests FunctionalTestingEditor
 */


// Define a simple automation test
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    InterceptTest1,
    "Plugins.Yasiu.Math.Intercept.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool InterceptTest1::RunTest( const FString& Parameters )

{
    YasiuMath::Vec3<float> ValidRes{0, 0, 10};
    YasiuMath::Vec3<float> result{};
    YasiuMath::Vec3<float> PlaneLocation = {0, 10, 10};
    YasiuMath::Vec3<float> PlaneMoveVec{0, -10, 0};
    float BulletSpeed = 10;
    bool ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result1"), ret);
    TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);

    /* INTANT INTERCEPT */
    ValidRes = {0, 10, 10};
    result = {};
    PlaneLocation = {0, 10, 10};
    BulletSpeed = 10000000.f;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result2"), ret);
    TestNearlyEqual(TEXT("Intercept zone close2"), (result - ValidRes).Length(), 0);

    ValidRes = {0, 10, 10};
    result = {};
    PlaneLocation = {0, 10, 10};
    BulletSpeed = 0.1;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestFalse(TEXT("Invalid result1"), ret);
    BulletSpeed = 0.2;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestFalse(TEXT("Invalid result2"), ret);
    BulletSpeed = 0.3;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestFalse(TEXT("Invalid result3"), ret);
    PlaneMoveVec = {0, 10, 0};
    BulletSpeed = 10;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestFalse(TEXT("Invalid result3"), ret);


    PlaneLocation = {0, 10, 5};
    PlaneMoveVec = {0, -2, 3};
    BulletSpeed = 6;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result4"), ret);

    PlaneLocation = {0, 100, 5};
    PlaneMoveVec = {0, 5, 10};
    BulletSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result5"), ret);

    PlaneLocation = {0, 100, -500};
    PlaneMoveVec = {0, 5, 10};
    BulletSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result6"), ret);

    BulletSpeed = 11;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result7"), ret);

    PlaneLocation = {0, 100, 500};
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestFalse(TEXT("Invalid result7"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close3"), (result - ValidRes).length(), 0);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BulletState_Test1,
    "Plugins.Yasiu.Math.Bullet.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool BulletState_Test1::RunTest( const FString& Parameters )
{
    YasiuMath::ProjectileDynamicState<double> Bullet{};

    YasiuMath::ProjectileDynamicState<double> state;
    bool valid = Bullet.PredictQuick(state, 10.);
    Bullet.Step(1.);

    return true;
}
