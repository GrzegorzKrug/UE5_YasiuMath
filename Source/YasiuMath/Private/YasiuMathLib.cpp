/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */


#include "YasiuMathLib.h"
#include "YasiuMathDataTypes.h"

using namespace YasiuMath::Types;

bool YasiuMath::Ballistics::InterceptMissile_Linear(
    Vec3<float>& InterceptLocation,
    const Vec3<float>& MissilePosition,
    const Vec3<float>& MissileVelocity,
    const float BulletSpeed
)
{
    float a = MissileVelocity.Dot(MissileVelocity) - BulletSpeed * BulletSpeed;
    float b = 2.0f * MissilePosition.Dot(MissileVelocity);
    float c = MissilePosition.Dot(MissilePosition);

    float t;

    if ( fabs(a) < 1e-6f ) {
        // Bullet speed ~= target speed
        if ( fabs(b) < 1e-6f )
            return false; // No solution
        t = -c / b;
        if ( t < 0 )
            return false;
    }
    else {
        float discriminant = b * b - 4.0f * a * c;
        if ( discriminant < 0 )
            return false; // No real solution
        float sqrtD = sqrtf(discriminant);
        float t1 = (-b + sqrtD) / (2.0f * a);
        float t2 = (-b - sqrtD) / (2.0f * a);

        // Choose smallest positive t
        t = (t1 > 0 && t2 > 0) ? fminf(t1, t2) : (t1 > 0 ? t1 : t2);
        if ( t < 0 )
            return false; // Both solutions negative
    }

    InterceptLocation = MissilePosition + MissileVelocity * t;
    return true;
}

bool YasiuMath::Ballistics::InterceptMissile_Dynamic(
    Vec3<float>& PredictedLocation,
    double& EstimatedTime,
    ProjectileDynamicState<double> Missile,
    const InterceptorParams& Interceptor,
    const double QueryTime,
    const double DeltaTime
)
{
    bool calculationValid = false;
    // Vec3<double>
    Vec3<double> InitialOffset = Interceptor.Position;
    // Bullet.BulletPosition = PositionOffset;
    Missile.Position -= InitialOffset;

    double Velocity = Interceptor.InitialSpeed;
    double Range = 0;

    if ( DeltaTime < EPSILON ) {
        /* Too Small Eps */
        throw std::runtime_error("Delta time in Intercept is too small");
    }

    unsigned int StepsN = static_cast<unsigned int>(floor(QueryTime / DeltaTime));

    /* 1-Indexed for time calculation */
    for ( unsigned int i = 1; i < StepsN + 1; i++ ) {
        /* Missile update */
        Missile.Step(DeltaTime);

        /* Interceptor state update */
        Velocity += Interceptor.Acceleration * DeltaTime;

        if ( Interceptor.AirResistance > 0 ) {
            /* Velocity is always positive */
            auto drag = Velocity * Velocity * Interceptor.AirResistance * DeltaTime;
            Velocity -= drag;
        }

        if ( Velocity > Interceptor.MaxSpeed && Interceptor.MaxSpeed > 0 ) {
            Velocity = Interceptor.MaxSpeed;
        }

        /* Dynamic update must be iterative */
        Range += Velocity * DeltaTime;

        if ( Missile.Position.Length() <= Range ) {
            EstimatedTime = i * DeltaTime;
            calculationValid = true;
            /* Reached estimation */
            break;
        }
    }

    PredictedLocation = Missile.Position + InitialOffset;
    return calculationValid;
}
