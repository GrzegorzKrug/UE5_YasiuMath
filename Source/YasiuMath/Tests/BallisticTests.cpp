#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"


#include "YasiuMathLib.h"
#include "YasiuMathBP/BPFunLib_Ballistic.h"

#include <vector>

/*
 * 3 Plugins in game project
 * RuntimeTest EditorTests FunctionalTestingEditor
 */

using namespace YasiuMath::Types;
using namespace YasiuMath::Ballistics;


#define Vector3Test(TEXT, A, B, EPS) \
{ \
    TestNearlyEqual(TEXT, A.X, B.X, EPS); \
    TestNearlyEqual(TEXT, A.Y, B.Y, EPS); \
    TestNearlyEqual(TEXT, A.Z, B.Z, EPS); \
};


IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BalisticLibGeneral1,
    "Plugins.Yasiu.Math.Ballistic.General.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool BalisticLibGeneral1::RunTest( const FString& Parameters )
{
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    InterceptTest0,
    "Plugins.Yasiu.Math.Ballistic.Intercept.0",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Types checking */
bool InterceptTest0::RunTest( const FString& Parameters )
{
    ProjectileDynamicState<float> Proj_F;
    ProjectileDynamicState<double> Proj_D;
    InterceptorParams<float> Int_F;
    InterceptorParams<double> Int_D;

    Vec3<float> Out_F;
    Vec3<double> Out_D;

    /* Correct */
    YasiuMath::Ballistics::InterceptMissile_Dynamic(Out_F, Proj_F, Int_F, 0, 0.1);
    YasiuMath::Ballistics::InterceptMissile_Dynamic(Out_D, Proj_D, Int_D, 0, 0.1);

    return true;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    InterceptTest1,
    "Plugins.Yasiu.Math.Ballistic.Intercept.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool InterceptTest1::RunTest( const FString& Parameters )

{
    Vec3<float> ValidRes{0, 0, 10};
    Vec3<float> result{};
    Vec3<float> PlaneLocation = {0, 10, 10};
    Vec3<float> PlaneMoveVec{0, -10, 0};
    float BulletSpeed = 10;
    bool ret = YasiuMath::Ballistics::InterceptMissile_Linear(result, PlaneLocation, PlaneMoveVec, BulletSpeed);
    TestTrue(TEXT("Valid result1"), ret);
    TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);

    /* Instant INTERCEPT */
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
    TestFalse(TEXT("Invalid result3b"), ret);

    /* <-> */
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
    InterceptTest2,
    "Plugins.Yasiu.Math.Ballistic.Intercept.2",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool InterceptTest2::RunTest( const FString& Parameters )

{
    YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Target;

    Vec3<float> ValidRes{0, 0, 10};
    Vec3<float> result{};
    const float YASIU_EPS = 0.0001;
    const float MaxQueryTime = 100;
    const float DT = 0.1;

    Target.Position = {0, 10, 10};
    Target.Velocity = {0, -10, 0};

    Missile.InitialSpeed = 10;
    bool ret = YasiuMath::Ballistics::InterceptMissile_Dynamic(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result1"), ret);
    TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);

    ValidRes = {0, 9, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 10000000.f;

    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result2"), ret);
    TestNearlyEqual(TEXT("Intercept zone close2"), (result - ValidRes).Length(), 0);
    Vector3Test(TEXT("Intercept zone close2b"), result, ValidRes, YASIU_EPS);

    ValidRes = {0, 10, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 0.1f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result1"), ret);

    Missile.InitialSpeed = 0.2f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result2"), ret);

    Missile.InitialSpeed = 0.3f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result3"), ret);

    Target.Velocity = {0, 10, 0};
    Missile.InitialSpeed = 10.f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result3b"), ret);

    /* <-> */
    Target.Position = {0, 10, 5};
    Target.Velocity = {0, -2, 3};
    Missile.InitialSpeed = 6;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result4"), ret);

    Target.Position = {0, 100, 5};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result5"), ret);

    Target.Position = {0, 100, -500};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result6"), ret);

    Missile.InitialSpeed = 11;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result7"), ret);

    Target.Position = {0, 100, 500};
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result7"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close3"), (result - ValidRes).length(), 0);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    InterceptTest3,
    "Plugins.Yasiu.Math.Ballistic.Intercept.3",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool InterceptTest3::RunTest( const FString& Parameters )

{
    YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Target;

    Vec3<float> ValidRes{0, 0, 10};
    Vec3<float> result{};
    const float YASIU_EPS = 0.0001;
    const float MaxQueryTime = 100;
    const float DT = 0.1;

    Target.Position = {0, 10, 10};
    Target.Velocity = {0, -10, 0};
    Target.AirFrictionCoeff = 1e-4;
    Missile.AirResistance = 1e-6;

    Missile.InitialSpeed = 10;
    bool ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result1"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);
    // Vector3Test(TEXT("Intercept zone close1b"), result, ValidRes, YASIU_EPS);

    ValidRes = {0, 9, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 100000.f;

    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result2"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close2"), (result - ValidRes).Length(), 0);
    // Vector3Test(TEXT("Intercept zone close2b"), result, ValidRes, YASIU_EPS);

    ValidRes = {0, 10, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 0.1f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result1"), ret);

    Missile.InitialSpeed = 0.2f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result2"), ret);

    Missile.InitialSpeed = 0.3f;
    Target.AirFrictionCoeff = 0;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result3"), ret);

    Target.AirFrictionCoeff = 0;
    Target.Position = {0, 10, 10};
    Target.Velocity = {0, 10, 0};
    Missile.InitialSpeed = 10.f;
    Missile.Acceleration = 0;
    Missile.AirResistance = 0.0001;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result3b"), ret);

    /* <-> */
    Target.Position = {0, 10, 5};
    Target.Velocity = {0, -2, 3};
    Missile.InitialSpeed = 6;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result4"), ret);

    Target.AirFrictionCoeff = 0.001;
    Target.Position = {0, 100, 5};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result5"), ret);

    Target.AirFrictionCoeff = 0;
    Target.Position = {0, 100, -500};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result6"), ret);

    Missile.InitialSpeed = 11;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Valid result7"), ret);

    Target.Position = {0, 100, 500};
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestFalse(TEXT("Invalid result7"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close3"), (result - ValidRes).length(), 0);

    Target.AirFrictionCoeff = 0;
    Missile.InitialSpeed = 0.3f;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Chase result1"), ret);

    Target.AirFrictionCoeff = 0.001;
    Target.Position = {0, 10, 10};
    Target.Velocity = {0, 10, 0};
    Missile.InitialSpeed = 10.f;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(result, Target, Missile, MaxQueryTime, DT);
    TestTrue(TEXT("Chase result2"), ret);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BulletState_Test1,
    "Plugins.Yasiu.Math.Ballistic.BulletState.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool BulletState_Test1::RunTest( const FString& Parameters )
{
    YasiuMath::Ballistics::ProjectileDynamicState<double> Bullet{};

    YasiuMath::Ballistics::ProjectileDynamicState<double> state;
    auto newState = Bullet.PredictQuick(10.);
    Bullet.DiscreteStep(1.);
    newState.DiscreteStep(1);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BulletState_Test2,
    "Plugins.Yasiu.Math.Ballistic.BulletState.2",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Compare predict with quick predict */
bool BulletState_Test2::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001f;

    ProjectileDynamicState<double> Bullet1{};
    Bullet1.MaxSpeed = 300;
    Bullet1.AirFrictionCoeff = 0;
    Bullet1.Velocity = {10, 0, 0};
    Bullet1.Acceleration = {0, 0, 0};
    auto Bullet2 = Bullet1;
    auto Bullet3 = Bullet1;
    auto Bullet4 = Bullet1;

    const double Step = 5000;
    const auto out1 = Bullet1.PredictQuick(Step);
    Bullet2.DiscreteStep(Step);
    const auto out2 = Bullet2;
    Bullet3.AutoStep(Step, 0.1);
    const auto out3 = Bullet3;
    const auto out4 = Bullet4.Predict(Step, 0.1);

    Vector3Test(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position3"), out2.Position, out1.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position4"), out1.Position, out4.Position, YASIU_EPS);

    // TestNearlyEqual(TEXT("Close Position"), out1.Position.X, out2.Position.X, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity4"), out1.Velocity, out4.Velocity, YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BulletState_Test3,
    "Plugins.Yasiu.Math.Ballistic.BulletState.3",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Big acceleration compare */
bool BulletState_Test3::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001f;

    const double TIME = 100;
    ProjectileDynamicState<double> Bullet1{};
    Bullet1.MaxSpeed = 500;
    Bullet1.AirFrictionCoeff = 0;
    Bullet1.Velocity = {10, 0, 0};
    Bullet1.Acceleration = {10, 0, 0};
    auto Bullet2 = Bullet1;
    auto Bullet3 = Bullet1;
    auto Bullet4 = Bullet1;
    auto Bullet5 = Bullet1;

    /* DISCRETE CALCULATION */
    double DT = 0.01;
    const int N = TIME / DT;
    for ( int i = 0; i < N; i++ ) {
        Bullet1.DiscreteStep(DT);
    }
    const double missingTime = TIME - (DT * N);
    if ( missingTime > 0 ) {
        Bullet1.DiscreteStep(missingTime);
    }
    // Bullet1.Step(TIME);
    const auto out1 = Bullet1;

    const auto out2 = Bullet2.PredictQuick(TIME);
    Bullet3.AutoStep(TIME, 0.1);
    const auto out3 = Bullet3;
    const auto out4 = Bullet4.Predict(TIME, 0.1);

    Bullet5.DiscreteStep(TIME);
    const auto out5 = Bullet5;

    TestNearlyEqual(TEXT("Close Position RAW1"), out1.Position.X, 37995., YASIU_EPS);
    TestNearlyEqual(TEXT("Close Position RAW2"), out2.Position.X, 37995., YASIU_EPS);
    Vector3Test(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position4"), out1.Position, out4.Position, YASIU_EPS);
    // Vector3Test(TEXT("Close Position5"), out1.Position, out5.Position, YASIU_EPS); /* Can not test, too big step */

    TestNearlyEqual(TEXT("Max Velocity"), out1.Velocity.Length(), 500., YASIU_EPS);
    Vector3Test(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity4"), out1.Velocity, out4.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity5"), out1.Velocity, out5.Velocity, YASIU_EPS);

    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BulletState_Test4,
    "Plugins.Yasiu.Math.Ballistic.BulletState.4",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Big acceleration compare */
bool BulletState_Test4::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001f;

    const double TIME = 100;
    ProjectileDynamicState<double> Bullet1{};
    Bullet1.MaxSpeed = 0;
    Bullet1.AirFrictionCoeff = 0;
    Bullet1.Velocity = {10, 0, 0};
    Bullet1.Acceleration = {10, 0, 0};
    auto Bullet2 = Bullet1;
    auto Bullet3 = Bullet1;
    auto Bullet4 = Bullet1;
    auto Bullet5 = Bullet1;

    /* DISCRETE CALCULATION */
    double DT = 0.01;
    const int N = TIME / DT;
    for ( int i = 0; i < N; i++ ) {
        Bullet1.DiscreteStep(DT);
    }
    const double missingTime = TIME - (DT * N);
    if ( missingTime > 0 ) {
        Bullet1.DiscreteStep(missingTime);
    }

    const auto out1 = Bullet1;
    const auto out2 = Bullet2.PredictQuick(TIME);

    Bullet3.AutoStep(TIME, 0.1);
    const auto out3 = Bullet3;
    const auto out4 = Bullet4.Predict(TIME, 0.1);

    Bullet5.DiscreteStep(TIME);
    const auto out5 = Bullet5;

    TestNearlyEqual(TEXT("Close Position RAW1"), out1.Position.X, 51000, YASIU_EPS);
    TestNearlyEqual(TEXT("Close Position RAW2"), out2.Position.X, 51000, YASIU_EPS);
    Vector3Test(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    // Vector3Test(TEXT("Close Position3"), out2.Position, out1.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position4"), out1.Position, out4.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position5"), out1.Position, out5.Position, YASIU_EPS);

    TestNearlyEqual(TEXT("End Velocity1"), out1.Velocity.Length(), 1010., YASIU_EPS);
    TestNearlyEqual(TEXT("End Velocity2"), out2.Velocity.Length(), 1010., YASIU_EPS);

    Vector3Test(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity4"), out1.Velocity, out4.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Position5"), out1.Position, out5.Position, YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    BulletState_Test5,
    "Plugins.Yasiu.Math.Ballistic.BulletState.5",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* AIR DRAG */
bool BulletState_Test5::RunTest( const FString& Parameters )
{
    constexpr double YASIU_EPS = 0.0001f;

    ProjectileDynamicState<double> Bullet1{};
    Bullet1.MaxSpeed = 500;
    Bullet1.AirFrictionCoeff = 0.01;
    Bullet1.Velocity = {10, 0, 0};
    Bullet1.Acceleration = {1000, 0, 0};

    auto Bullet2 = Bullet1;
    auto Bullet3 = Bullet1;
    auto Bullet4 = Bullet1;

    const double Step = 5000;
    // const auto out1 = Bullet1.PredictQuick(Step);
    const auto out1 = Bullet1.Predict(Step, 0.1);
    // Bullet2.Step(Step);
    // const auto out2 = Bullet2;
    Bullet3.AutoStep(Step, 0.1);
    const auto out3 = Bullet3;

    // Vector3Test(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    Vector3Test(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    // Vector3Test(TEXT("Close Position3"), out2.Position, out1.Position, YASIU_EPS);

    // TestNearlyEqual(TEXT("Close Position"), out1.Position.X, out2.Position.X, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    Vector3Test(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);

    return true;
}
