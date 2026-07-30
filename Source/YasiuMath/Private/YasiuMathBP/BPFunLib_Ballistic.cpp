/* 
* Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */


#include "YasiuMathBP/BPFunLib_Ballistic.h"

YasiuMath::Ballistics::ProjectileDynamicState<double> FBallisticObject::ToDynamicObject() const
{
    YasiuMath::Ballistics::ProjectileDynamicState<double> Out;
    Out.Position.X = Position.X;
    Out.Position.Y = Position.Y;
    Out.Position.Z = Position.Z;

    Out.Velocity.X = Velocity.X;
    Out.Velocity.Y = Velocity.Y;
    Out.Velocity.Z = Velocity.Z;

    Out.Acceleration.X = Acceleration.X;
    Out.Acceleration.Y = Acceleration.Y;
    Out.Acceleration.Z = Acceleration.Z;

    Out.AirFrictionCoeff = AirFrictionCoeff;

    Out.MaxSpeed = MaxSpeed;

    return Out;
}

FBallisticObject& FBallisticObject::FromDynamic( const YasiuMath::Ballistics::ProjectileDynamicState<double>& Ob )
{
    Position.X = Ob.Position.X;
    Position.Y = Ob.Position.Y;
    Position.Z = Ob.Position.Z;

    Velocity.X = Ob.Velocity.X;
    Velocity.Y = Ob.Velocity.Y;
    Velocity.Z = Ob.Velocity.Z;

    Acceleration.X = Ob.Acceleration.X;
    Acceleration.Y = Ob.Acceleration.Y;
    Acceleration.Z = Ob.Acceleration.Z;
    AirFrictionCoeff = Ob.AirFrictionCoeff;

    MaxSpeed = Ob.MaxSpeed;

    return *this;
}

YasiuMath::Ballistics::InterceptorParams<double> FBallisticInterceptor::ToInterceptor() const
{
    YasiuMath::Ballistics::InterceptorParams<double> Out;
    Out.Position.X = Position.X;
    Out.Position.Y = Position.Y;
    Out.Position.Z = Position.Z;

    Out.InitialSpeed = InitialSpeed;
    Out.Acceleration = Acceleration;
    Out.MaxSpeed = MaxSpeed;
    Out.AirResistance = AirResistance;

    return Out;
}


void FBallisticInterceptor::FromInterceptor( const YasiuMath::Ballistics::InterceptorParams<double>& Ob )
{
    Position.X = Ob.Position.X;
    Position.Y = Ob.Position.Y;
    Position.Z = Ob.Position.Z;

    InitialSpeed = Ob.InitialSpeed;
    Acceleration = Ob.Acceleration;
    MaxSpeed = Ob.MaxSpeed;
    AirResistance = Ob.AirResistance;
}


FBallisticObject UYasiuMathFL_Ballistic::DiscreteStep( const FBallisticObject& Ob, const float DeltaStep )
{
    auto Proj = Ob.ToDynamicObject();
    Proj.DiscreteStep(DeltaStep);
    return FBallisticObject().FromDynamic(Proj);
}

FBallisticObject UYasiuMathFL_Ballistic::AutoStep( const FBallisticObject& Ob, const float PredictTime, const float DeltaStep )
{
    auto Proj = Ob.ToDynamicObject();
    Proj.AutoStep(PredictTime, DeltaStep);
    return FBallisticObject().FromDynamic(Proj);
}

bool UYasiuMathFL_Ballistic::Intercept_Linear(
    const FVector& TargetPosition,
    const FVector& TargetVelocity,
    double InterceptSpeed,
    FVector& OutLocation
)
{
    YasiuMath::Types::Vec3<double> InterpLoc;
    const auto ret = YasiuMath::Ballistics::InterceptMissile_Linear(
        InterpLoc,
        YasiuMath::Types::Vec3<double>(TargetPosition.X, TargetPosition.Y, TargetPosition.Z),
        YasiuMath::Types::Vec3<double>(TargetVelocity.X, TargetVelocity.Y, TargetVelocity.Z),
        InterceptSpeed
    );

    OutLocation.X = InterpLoc.X;
    OutLocation.Y = InterpLoc.Y;
    OutLocation.Z = InterpLoc.Z;
    return ret;
}

bool UYasiuMathFL_Ballistic::Intercept_Dynamic(
    const FBallisticObject& Target,
    const FBallisticInterceptor Interceptor,
    FVector& OutLocation,
    float MaxQueryTime,
    float DeltaStep
)
{
    YasiuMath::Types::Vec3<double> InterpLoc;
    const auto tg = Target.ToDynamicObject();
    const auto inc = Interceptor.ToInterceptor();
    
     const auto ret = YasiuMath::Ballistics::InterceptMissile_Dynamic(
        InterpLoc,
        tg,
        inc,
        MaxQueryTime,
        DeltaStep
    );

    OutLocation.X = InterpLoc.X;
    OutLocation.Y = InterpLoc.Y;
    OutLocation.Z = InterpLoc.Z;
    return ret;
    
}
