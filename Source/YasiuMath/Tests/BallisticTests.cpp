#pragma once


/* Testing only current and future builds */
#include "Runtime/Launch/Resources/Version.h"
#if WITH_DEV_AUTOMATION_TESTS && ENGINE_MAJOR_VERSION == 5  &&  ENGINE_MINOR_VERSION >= 6


#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"


#include "YasiuMathLib.h"


#include <vector>

/*
 * 3 Plugins in game project
 * RuntimeTest EditorTests FunctionalTestingEditor
 */

using namespace YasiuMath::Types;
using namespace YasiuMath::Ballistics;


#define TestVec3NearlyEqual(Message, A, B, EPS) \
{ \
    TestNearlyEqual(FString(Message) + TEXT("(X)"), A.X, B.X, EPS); \
    TestNearlyEqual(FString(Message) + TEXT("(Y)"), A.Y, B.Y, EPS); \
    TestNearlyEqual(FString(Message) + TEXT("(Z)"), A.Z, B.Z, EPS); \
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
    YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Proj_F, Int_F, 0.f, 0.1f, Out_F);
    YasiuMath::Ballistics::InterceptMissile_Dynamic<double>(Proj_D, Int_D, 0, 0.1, Out_D);

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
    bool ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestTrue(TEXT("Valid result1"), ret);
    TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);

    /* Instant INTERCEPT */
    ValidRes = {0, 10, 10};
    result = {};
    PlaneLocation = {0, 10, 10};
    BulletSpeed = 10000000.f;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestTrue(TEXT("Valid result2"), ret);
    TestNearlyEqual(TEXT("Intercept zone close2"), (result - ValidRes).Length(), 0);

    ValidRes = {0, 10, 10};
    result = {};
    PlaneLocation = {0, 10, 10};
    BulletSpeed = 0.1;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestFalse(TEXT("Invalid result1"), ret);

    BulletSpeed = 0.2;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestFalse(TEXT("Invalid result2"), ret);

    BulletSpeed = 0.3;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestFalse(TEXT("Invalid result3"), ret);

    PlaneMoveVec = {0, 10, 0};
    BulletSpeed = 10;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestFalse(TEXT("Invalid result3b"), ret);

    /* <-> */
    PlaneLocation = {0, 10, 5};
    PlaneMoveVec = {0, -2, 3};
    BulletSpeed = 6;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestTrue(TEXT("Valid result4"), ret);

    PlaneLocation = {0, 100, 5};
    PlaneMoveVec = {0, 5, 10};
    BulletSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestTrue(TEXT("Valid result5"), ret);

    PlaneLocation = {0, 100, -500};
    PlaneMoveVec = {0, 5, 10};
    BulletSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestTrue(TEXT("Valid result6"), ret);

    BulletSpeed = 11;
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
    TestTrue(TEXT("Valid result7"), ret);

    PlaneLocation = {0, 100, 500};
    ret = YasiuMath::Ballistics::InterceptMissile_Linear(PlaneLocation, PlaneMoveVec, BulletSpeed, result);
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
    bool ret = YasiuMath::Ballistics::InterceptMissile_Dynamic(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result1"), ret);
    TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);

    ValidRes = {0, 9, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 10000000.f;

    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result2"), ret);
    TestNearlyEqual(TEXT("Intercept zone close2"), (result - ValidRes).Length(), 0);
    TestVec3NearlyEqual(TEXT("Intercept zone close2b"), result, ValidRes, YASIU_EPS);

    ValidRes = {0, 10, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 0.1f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result1"), ret);

    Missile.InitialSpeed = 0.2f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result2"), ret);

    Missile.InitialSpeed = 0.3f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result3"), ret);

    Target.Velocity = {0, 10, 0};
    Missile.InitialSpeed = 10.f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result3b"), ret);

    /* <-> */
    Target.Position = {0, 10, 5};
    Target.Velocity = {0, -2, 3};
    Missile.InitialSpeed = 6;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result4"), ret);

    Target.Position = {0, 100, 5};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result5"), ret);

    Target.Position = {0, 100, -500};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result6"), ret);

    Missile.InitialSpeed = 11;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result7"), ret);

    Target.Position = {0, 100, 500};
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
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
    bool ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result1"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close1"), (result - ValidRes).Length(), 0);
    // Vector3Test(TEXT("Intercept zone close1b"), result, ValidRes, YASIU_EPS);

    ValidRes = {0, 9, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 100000.f;

    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result2"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close2"), (result - ValidRes).Length(), 0);
    // Vector3Test(TEXT("Intercept zone close2b"), result, ValidRes, YASIU_EPS);

    ValidRes = {0, 10, 10};
    result = {};
    Target.Position = {0, 10, 10};
    Missile.InitialSpeed = 0.1f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result1"), ret);

    Missile.InitialSpeed = 0.2f;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result2"), ret);

    Missile.InitialSpeed = 0.3f;
    Target.AirFrictionCoeff = 0;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result3"), ret);

    Target.AirFrictionCoeff = 0;
    Target.Position = {0, 10, 10};
    Target.Velocity = {0, 10, 0};
    Missile.InitialSpeed = 10.f;
    Missile.Acceleration = 0;
    Missile.AirResistance = 0.0001;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result3b"), ret);

    /* <-> */
    Target.Position = {0, 10, 5};
    Target.Velocity = {0, -2, 3};
    Missile.InitialSpeed = 6;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result4"), ret);

    Target.AirFrictionCoeff = 0.001;
    Target.Position = {0, 100, 5};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result5"), ret);

    Target.AirFrictionCoeff = 0;
    Target.Position = {0, 100, -500};
    Target.Velocity = {0, 5, 10};
    Missile.InitialSpeed = 12;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result6"), ret);

    Missile.InitialSpeed = 11;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Valid result7"), ret);

    Target.Position = {0, 100, 500};
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestFalse(TEXT("Invalid result7"), ret);
    // TestNearlyEqual(TEXT("Intercept zone close3"), (result - ValidRes).length(), 0);

    Target.AirFrictionCoeff = 0;
    Missile.InitialSpeed = 0.3f;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
    TestTrue(TEXT("Chase result1"), ret);

    Target.AirFrictionCoeff = 0.001;
    Target.Position = {0, 10, 10};
    Target.Velocity = {0, 10, 0};
    Missile.InitialSpeed = 10.f;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result);
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

    TestVec3NearlyEqual(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position3"), out2.Position, out1.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position4"), out1.Position, out4.Position, YASIU_EPS);

    // TestNearlyEqual(TEXT("Close Position"), out1.Position.X, out2.Position.X, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity4"), out1.Velocity, out4.Velocity, YASIU_EPS);

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
    TestVec3NearlyEqual(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position4"), out1.Position, out4.Position, YASIU_EPS);
    // Vector3Test(TEXT("Close Position5"), out1.Position, out5.Position, YASIU_EPS); /* Can not test, too big step */

    TestNearlyEqual(TEXT("Max Velocity"), out1.Velocity.Length(), 500., YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity4"), out1.Velocity, out4.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity5"), out1.Velocity, out5.Velocity, YASIU_EPS);

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
    TestVec3NearlyEqual(TEXT("Close Position1"), out1.Position, out2.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    // Vector3Test(TEXT("Close Position3"), out2.Position, out1.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position4"), out1.Position, out4.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position5"), out1.Position, out5.Position, YASIU_EPS);


    TestNearlyEqual(TEXT("End Velocity1"), out1.Velocity.Length(), 1010., YASIU_EPS);
    TestNearlyEqual(TEXT("End Velocity2"), out2.Velocity.Length(), 1010., YASIU_EPS);

    TestVec3NearlyEqual(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity4"), out1.Velocity, out4.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Position5"), out1.Position, out5.Position, YASIU_EPS);

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
    TestVec3NearlyEqual(TEXT("Close Position2"), out1.Position, out3.Position, YASIU_EPS);
    // Vector3Test(TEXT("Close Position3"), out2.Position, out1.Position, YASIU_EPS);

    // TestNearlyEqual(TEXT("Close Position"), out1.Position.X, out2.Position.X, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity1"), out1.Velocity, out2.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Close Velocity2"), out1.Velocity, out3.Velocity, YASIU_EPS);
    // Vector3Test(TEXT("Close Velocity3"), out2.Velocity, out1.Velocity, YASIU_EPS);

    return true;
}

/* Check if model gives same results in predictor and interceptor */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation1,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.1",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


bool ModelCalculation1::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-4;
    // YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Ship;

    Ship.Position = {0, 10, 0};
    Ship.Velocity = {0, 1, 0};
    Ship.Acceleration = {0, 0, 0};
    const auto ValidState1 = Ship.PredictQuick(1);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value0a"), Ship.Velocity, Vec3(0.f,1.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value0b"), Ship.Position, Vec3(0.f,11.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value0c"), Ship.Velocity, ValidState1.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value0d"), Ship.Position, ValidState1.Position, YASIU_EPS);


    Ship.Position = {0, 10, 0};
    Ship.Velocity = 0;
    Ship.Acceleration = {0, 1, 0};
    Ship.MaxSpeed = 13;

    const auto ValidState2 = Ship.PredictQuick(1);
    const auto ValidState3 = Ship.PredictQuick(2);
    const auto ValidState4 = Ship.PredictQuick(3);
    const auto ValidState5 = Ship.PredictQuick(4);
    const auto ValidState6 = Ship.PredictQuick(5);

    /* Trapezoid estimation of acceleration */
    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value1"), Ship.Velocity, Vec3(0.f,1.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1b"), Ship.Position, Vec3(0.f,10.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1c"), Ship.Velocity, ValidState2.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1d"), Ship.Position, ValidState2.Position, YASIU_EPS);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value2"), Ship.Velocity, Vec3(0.f,2.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value2b"), Ship.Position, Vec3(0.f,12.0f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value2c"), Ship.Velocity, ValidState3.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value2d"), Ship.Position, ValidState3.Position, YASIU_EPS);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value3"), Ship.Velocity, Vec3(0.f,3.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value3b"), Ship.Position, Vec3(0.f,14.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value3c"), Ship.Velocity, ValidState4.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value3d"), Ship.Position, ValidState4.Position, YASIU_EPS);


    /* Max Speed check */
    Ship.Position = {0, 0, 0};
    Ship.Velocity = {0, 12, 0};
    Ship.Acceleration = {0, 2, 0};
    Ship.MaxSpeed = 15;

    const auto ValidMaxState1 = Ship.PredictQuick(1);
    const auto ValidMaxState2 = Ship.PredictQuick(2);
    const auto ValidMaxState3 = Ship.PredictQuick(3);
    const auto ValidMaxState4 = Ship.PredictQuick(4);
    const auto ValidMaxState5 = Ship.PredictQuick(5);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value4"), Ship.Position, Vec3(0.f,13.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value4b"), Ship.Velocity, Vec3(0.f,14.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value4c"), Ship.Position, ValidMaxState1.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value4d"), Ship.Velocity, ValidMaxState1.Velocity, YASIU_EPS);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value5"), Ship.Position, Vec3(0.f,27.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5b"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    /* Speed Clamping issue, proportional to step */
    // TestVec3NearlyEqual(TEXT("Compare Value5c"), Ship.Position, ValidMaxState2.Position, YASIU_EPS); 
    TestVec3NearlyEqual(TEXT("Compare Value5d"), Ship.Velocity, ValidMaxState2.Velocity, YASIU_EPS);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value6"), Ship.Position, Vec3(0.f,42.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6b"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    /* Speed Clamping issue, proportional to step */
    // TestVec3NearlyEqual(TEXT("Compare Value6c"), Ship.Position, ValidMaxState3.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6d"), Ship.Velocity, ValidMaxState3.Velocity, YASIU_EPS);

    Ship.DiscreteStep(1);
    TestVec3NearlyEqual(TEXT("Compare Value7"), Ship.Position, Vec3(0.f,57.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value7b"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    /* Speed Clamping issue, proportional to step */
    // TestVec3NearlyEqual(TEXT("Compare Value7c"), Ship.Position, ValidMaxState4.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value7d"), Ship.Velocity, ValidMaxState4.Velocity, YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation2,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.2",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* DuplicatedTest1 but small Steps in discrete */
bool ModelCalculation2::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-3;
    // YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Ship;

    Ship.Position = {0, 10, 0};
    Ship.Velocity = {0, 1, 0};
    Ship.Acceleration = {0, 0, 0};
    const auto ValidState1 = Ship.PredictQuick(1);

    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value0a"), Ship.Velocity, Vec3(0.f,1.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value0b"), Ship.Position, Vec3(0.f,11.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value0c"), Ship.Velocity, ValidState1.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value0d"), Ship.Position, ValidState1.Position, YASIU_EPS);

    Ship.Position = {0, 10, 0};
    Ship.Velocity = 0;
    Ship.Acceleration = {0, 1, 0};
    Ship.MaxSpeed = 13;

    const auto ValidState2 = Ship.PredictQuick(1);
    const auto ValidState3 = Ship.PredictQuick(2);
    const auto ValidState4 = Ship.PredictQuick(3);
    const auto ValidState5 = Ship.PredictQuick(4);
    const auto ValidState6 = Ship.PredictQuick(5);

    /* Trapezoid estimation of acceleration */
    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value1"), Ship.Velocity, Vec3(0.f,1.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1b"), Ship.Position, Vec3(0.f,10.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1c"), Ship.Velocity, ValidState2.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1d"), Ship.Position, ValidState2.Position, YASIU_EPS);

    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value2"), Ship.Velocity, Vec3(0.f,2.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value2b"), Ship.Position, Vec3(0.f,12.0f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value2c"), Ship.Velocity, ValidState3.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value2d"), Ship.Position, ValidState3.Position, YASIU_EPS);

    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value3"), Ship.Velocity, Vec3(0.f,3.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value3b"), Ship.Position, Vec3(0.f,14.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value3c"), Ship.Velocity, ValidState4.Velocity, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value3d"), Ship.Position, ValidState4.Position, YASIU_EPS);

    /* Max Speed check */
    Ship.Position = {0, 0, 0};
    Ship.Velocity = {0, 12, 0};
    Ship.Acceleration = {0, 2, 0};
    Ship.MaxSpeed = 15;

    const auto ValidMaxState1 = Ship.PredictQuick(1);
    const auto ValidMaxState2 = Ship.PredictQuick(2);
    const auto ValidMaxState3 = Ship.PredictQuick(3);
    const auto ValidMaxState4 = Ship.PredictQuick(4);
    const auto ValidMaxState5 = Ship.PredictQuick(5);

    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value4"), Ship.Position, Vec3(0.f,13.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value4b"), Ship.Velocity, Vec3(0.f,14.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value4c"), Ship.Position, ValidMaxState1.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value4d"), Ship.Velocity, ValidMaxState1.Velocity, YASIU_EPS);

    auto MicroStepState = Ship;
    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value5"), Ship.Position, Vec3(0.f,27.75f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5b"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5c"), Ship.Position, ValidMaxState2.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5d"), Ship.Velocity, ValidMaxState2.Velocity, YASIU_EPS);

    for ( int i = 0; i < 100; i++ ) { MicroStepState.DiscreteStep(0.01f); }
    TestVec3NearlyEqual(TEXT("Compare Value5e"), MicroStepState.Position, Vec3(0.f,27.75f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5f"), MicroStepState.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5g"), MicroStepState.Position, ValidMaxState2.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5h"), MicroStepState.Velocity, ValidMaxState2.Velocity, YASIU_EPS);

    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value6"), Ship.Position, Vec3(0.f,42.75f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6b"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6c"), Ship.Position, ValidMaxState3.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6d"), Ship.Velocity, ValidMaxState3.Velocity, YASIU_EPS);

    for ( int i = 0; i < 100; i++ ) { MicroStepState.DiscreteStep(0.01f); }
    TestVec3NearlyEqual(TEXT("Compare Value6e"), Ship.Position, Vec3(0.f,42.75f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6f"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6c"), Ship.Position, ValidMaxState3.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value6h"), Ship.Velocity, ValidMaxState3.Velocity, YASIU_EPS);

    /* TODO : handle validmax comparison */
    for ( int i = 0; i < 10; i++ ) { Ship.DiscreteStep(0.1f); }
    TestVec3NearlyEqual(TEXT("Compare Value7"), Ship.Position, Vec3(0.f,57.75f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value7b"), Ship.Velocity, Vec3(0.f,15.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value7c"), Ship.Position, ValidMaxState4.Position, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value7d"), Ship.Velocity, ValidMaxState4.Velocity, YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation5,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.5",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Quick Predict accelration values check */
bool ModelCalculation5::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-4;
    // YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Ship;
    Ship.Position = 0;
    Ship.Velocity = 0;
    Ship.Acceleration = {0, 1, 0};
    Ship.MaxSpeed = 10;

    const auto RetState1 = Ship.PredictQuick(1);
    const auto RetState5 = Ship.PredictQuick(5);
    const auto RetState9 = Ship.PredictQuick(9);
    const auto RetState10 = Ship.PredictQuick(10);
    const auto RetState11 = Ship.PredictQuick(11);
    const auto RetState15 = Ship.PredictQuick(15);

    TestVec3NearlyEqual(TEXT("Compare Value1"), RetState1.Position, Vec3(0.f,0.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value1b"), RetState1.Velocity, Vec3(0.f,1.f,0.f), YASIU_EPS);

    TestVec3NearlyEqual(TEXT("Compare Value5"), RetState5.Position, Vec3(0.f,12.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value5b"), RetState5.Velocity, Vec3(0.f,5.f,0.f), YASIU_EPS);

    TestVec3NearlyEqual(TEXT("Compare Value9"), RetState9.Position, Vec3(0.f,40.5f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value9b"), RetState9.Velocity, Vec3(0.f,9.f,0.f), YASIU_EPS);

    TestVec3NearlyEqual(TEXT("Compare Value10"), RetState10.Position, Vec3(0.f,50.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value10b"), RetState10.Velocity, Vec3(0.f,10.0f,0.f), YASIU_EPS);

    TestVec3NearlyEqual(TEXT("Compare Value11"), RetState11.Position, Vec3(0.f,60.f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value11b"), RetState11.Velocity, Vec3(0.f,10.0f,0.f), YASIU_EPS);

    TestVec3NearlyEqual(TEXT("Compare Value15"), RetState15.Position, Vec3(0.f,100.0f,0.f), YASIU_EPS);
    TestVec3NearlyEqual(TEXT("Compare Value15b"), RetState15.Velocity, Vec3(0.f,10.0f,0.f), YASIU_EPS);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation3,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.3",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Intercept with predictors check linear checks */
bool ModelCalculation3::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 0.0001;

    ProjectileDynamicState<float> Target;
    Vec3<float> result{};
    bool ret = 0;

    float PredictTime = 0;

    YasiuMath::Ballistics::ProjectileDynamicState<float> Ship;

    Ship.Position = {0, 10, 0};
    Ship.Velocity = {0, -5, 0};
    ret = YasiuMath::Ballistics::InterceptMissile_Linear<float>(Ship.Position, Ship.Velocity, 10, result, PredictTime);
    const auto ValidRes1 = Ship.PredictQuick(PredictTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1"), result, ValidRes1.Position, YASIU_EPS);
    TestTrue(TEXT("Ret1"), ret);

    Ship.Position = {100, 100, 0};
    Ship.Velocity = {0, 4, 1};
    ret = YasiuMath::Ballistics::InterceptMissile_Linear<float>(Ship.Position, Ship.Velocity, 10, result, PredictTime);
    const auto ValidRes2 = Ship.PredictQuick(PredictTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2"), result, ValidRes2.Position, YASIU_EPS);
    TestTrue(TEXT("Ret2"), ret);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation4,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.4",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Intercept Dynamic checks */
bool ModelCalculation4::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-3;
    const float MaxQueryTime = 100;
    const float DT = 0.1;


    YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Target;
    Vec3<float> result{};
    float InterceptTime = 0;
    bool ret = 0;

    Target.AirFrictionCoeff = 0;
    Missile.AirResistance = 0;

    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Missile.InitialSpeed = 10;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result, InterceptTime);
    const auto ValidRes1 = Target.PredictQuick(InterceptTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1"), result, ValidRes1.Position, YASIU_EPS);
    TestTrue(TEXT("Ret1"), ret);


    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0, 0.1};
    Missile.InitialSpeed = 10;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result, InterceptTime);
    const auto ValidRes2 = Target.PredictQuick(InterceptTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2"), result, ValidRes2.Position, YASIU_EPS);
    TestTrue(TEXT("Ret2"), ret);


    Target.Position = {200, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0.2, 0.4};
    Target.MaxSpeed = 300;
    Missile.InitialSpeed = 30;
    Missile.Acceleration = 1;
    Missile.MaxSpeed = 350;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result, InterceptTime);
    const auto ValidRes3 = Target.PredictQuick(InterceptTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3"), result, ValidRes3.Position, YASIU_EPS);
    TestTrue(TEXT("Ret3"), ret);


    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation6,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.6",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Air resistance calculations */
bool ModelCalculation6::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-3;
    const float MaxQueryTime = 100;
    const float DT = 0.1;


    YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Target;
    Vec3<float> result{};
    float InterceptTime = 0;
    bool ret = 0;

    Target.AirFrictionCoeff = 0.0015;
    Missile.AirResistance = 0.001;

    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Missile.InitialSpeed = 10;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result, InterceptTime);
    const auto ValidRes1 = Target.Predict(InterceptTime, 0.1);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1"), result, ValidRes1.Position, YASIU_EPS);
    TestTrue(TEXT("Ret1"), ret);


    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0, 0.1};
    Missile.InitialSpeed = 10;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result, InterceptTime);
    const auto ValidRes2 = Target.Predict(InterceptTime, 0.1);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2"), result, ValidRes2.Position, YASIU_EPS);
    TestTrue(TEXT("Ret2"), ret);


    Target.Position = {200, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0.2, 0.4};
    Target.MaxSpeed = 300;
    Missile.InitialSpeed = 30;
    Missile.Acceleration = 1;
    Missile.MaxSpeed = 350;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT, result, InterceptTime);
    const auto ValidRes3 = Target.Predict(InterceptTime, 0.1);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3"), result, ValidRes3.Position, YASIU_EPS);
    TestTrue(TEXT("Ret3"), ret);


    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation7,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.7",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Intercept small and big step */
bool ModelCalculation7::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-2;
    const float MaxQueryTime = 100;

    const float DT20 = 20;
    const float DT10 = 10;
    const float DT1 = 1;
    const float DT01 = 0.1;
    const float DTA = 0.001;
    const float ResultStepTime = 0.01;

    YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Target;
    Vec3<float> Res01{};
    Vec3<float> Res1{};
    Vec3<float> Res10{};
    Vec3<float> Res20{};

    // float IntTime = 0;
    bool ret = 0;

    Target.AirFrictionCoeff = 0;
    Missile.AirResistance = 0;

    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Missile.InitialSpeed = 10;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT01, DTA, Res01 );
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT1, DTA, Res1 );
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT10, DTA, Res10);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT20, DTA, Res20);
    // const auto ValidRes1 = Target.Predict(IntTime, ResultStepTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1a "), Res01, Res1, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1b "), Res1, Res10, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1c "), Res10, Res20, YASIU_EPS);
    TestTrue(TEXT("Ret1"), ret);
    
    
    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0, 0.1};
    Missile.InitialSpeed = 10;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT01, DTA, Res01);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT1, DTA, Res1);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT10, DTA, Res10);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT20, DTA, Res20);
    // const auto ValidRes2 = Target.Predict(IntTime, ResultStepTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2a "), Res01, Res1, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2b "), Res1, Res10, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2c "), Res10, Res20, YASIU_EPS);
    TestTrue(TEXT("Ret2"), ret);


    Target.Position = {200, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0.2, 0.4};
    Target.MaxSpeed = 300;
    Missile.InitialSpeed = 30;
    Missile.Acceleration = 1;
    Missile.MaxSpeed = 350;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT01, DTA, Res01);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT1, DTA, Res1);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT10, DTA, Res10);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT20, DTA, Res20);
    // const auto ValidRes3 = Target.Predict(IntTime, ResultStepTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3a "), Res01, Res1, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3b "), Res1, Res10, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3c "), Res10, Res20, YASIU_EPS);
    TestTrue(TEXT("Ret3"), ret);


    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    ModelCalculation8,
    "Plugins.Yasiu.Math.Ballistic.ModelCheck.8",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)


/* Test 7 With ari resistance */
bool ModelCalculation8::RunTest( const FString& Parameters )
{
    const float YASIU_EPS = 1e-1;
    const float MaxQueryTime = 100;

    const float DT20 = 20;
    const float DT10 = 10;
    const float DT1 = 1;
    const float DT01 = 0.1;
    const float DTA = 0.01;
    const float ResultStepTime = 0.1; /* Air differences */

    YasiuMath::Ballistics::InterceptorParams<float> Missile;
    ProjectileDynamicState<float> Target;
    Vec3<float> Res01{};
    Vec3<float> Res1{};
    Vec3<float> Res10{};
    Vec3<float> Res20{};

    float IntTime = 0;
    bool ret = 0;

    Target.AirFrictionCoeff = 0.0015;
    Missile.AirResistance = 0.001;

    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Missile.InitialSpeed = 10;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT01, DTA, Res01, IntTime);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT1, DTA, Res1, IntTime);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT10, DTA, Res10, IntTime);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT20, DTA, Res20, IntTime);
    // const auto ValidRes1 = Target.Predict(IntTime, ResultStepTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1a "), Res01, Res1, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1b "), Res1, Res10, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy1c "), Res10, Res20, YASIU_EPS);
    TestTrue(TEXT("Ret1"), ret);


    Target.Position = {100, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0, 0.1};
    Missile.InitialSpeed = 10;
    Missile.Acceleration = 1;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT01, DTA, Res01);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT1, DTA, Res1);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT10, DTA, Res10);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT20, DTA, Res20, IntTime);
    // const auto ValidRes2 = Target.Predict(IntTime, ResultStepTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2a "), Res01, Res1, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2b "), Res1, Res10, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy2c "), Res10, Res20, YASIU_EPS);
    TestTrue(TEXT("Ret2"), ret);


    Target.Position = {200, 100, 0};
    Target.Velocity = {0, 4, 1};
    Target.Acceleration = {0, 0.2, 0.4};
    Target.MaxSpeed = 300;
    Missile.InitialSpeed = 30;
    Missile.Acceleration = 1;
    Missile.MaxSpeed = 350;
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT01, DTA, Res01);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT1, DTA, Res1);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT10, DTA, Res10);
    ret = YasiuMath::Ballistics::InterceptMissile_Dynamic<float>(Target, Missile, MaxQueryTime, DT20, DTA, Res20, IntTime);
    // const auto ValidRes3 = Target.Predict(IntTime, ResultStepTime);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3a "), Res01, Res1, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3b "), Res1, Res10, YASIU_EPS);
    TestVec3NearlyEqual(TEXT("TestInterceptAccuracy3c "), Res10, Res20, YASIU_EPS);
    TestTrue(TEXT("Ret3"), ret);


    return true;
}

// #endif
#endif
