/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

#pragma once

#include "YasiuMathLib.h"


using namespace YasiuMath::Types;


namespace YasiuMath {
    namespace Ballistics {
        template<typename T>
        bool InterceptMissile_Linear(
            Vec3<T>& InterceptLocation,
            const Vec3<T>& MissilePosition,
            const Vec3<T>& MissileVelocity,
            const double BulletSpeed
        )
        {
            T a = MissileVelocity.Dot(MissileVelocity) - BulletSpeed * BulletSpeed;
            T b = 2.0f * MissilePosition.Dot(MissileVelocity);
            T c = MissilePosition.Dot(MissilePosition);

            T t;

            if ( fabs(a) < 1e-6f ) {
                // Bullet speed ~= target speed
                if ( fabs(b) < 1e-6f )
                    return false; // No solution
                t = -c / b;
                if ( t < 0 )
                    return false;
            }
            else {
                T discriminant = b * b - 4.0f * a * c;
                if ( discriminant < 0 )
                    return false; // No real solution
                T sqrtD = sqrtf(discriminant);
                T t1 = (-b + sqrtD) / (2.0f * a);
                T t2 = (-b - sqrtD) / (2.0f * a);

                // Choose smallest positive t
                t = (t1 > 0 && t2 > 0) ? fminf(t1, t2) : (t1 > 0 ? t1 : t2);
                if ( t < 0 )
                    return false; // Both solutions negative
            }

            InterceptLocation = MissilePosition + MissileVelocity * t;
            return true;
        }

        template<typename T>
        bool InterceptMissile_Dynamic(
            Vec3<T>& PredictedLocation,
            ProjectileDynamicState<T> Missile,
            const InterceptorParams<T>& Interceptor,
            const double MaxQueryTime,
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

            if ( DeltaTime < YasiuMath::Constants::EPSILON ) {
                /* Too Small Eps */
                throw std::runtime_error("Delta time in Intercept is too small");
            }

            unsigned int StepsN = static_cast<unsigned int>(floor(MaxQueryTime / DeltaTime));

            /* 1-Indexed for time calculation */
            // auto QueryTime = 0;
            for ( unsigned int i = 1; i < StepsN + 1; i++ ) {
                /* Missile update */
                Missile.DiscreteStep(DeltaTime);

                /* Interceptor state update */
                const auto oldVel = Velocity;
                Velocity += Interceptor.Acceleration * DeltaTime;

                if ( Interceptor.AirResistance > 0 ) {
                    /* Velocity is always positive */
                    const auto drag = Velocity * Velocity * Interceptor.AirResistance * DeltaTime;
                    Velocity -= drag;
                    if ( Velocity <= 0 ) {
                        return calculationValid;
                    }
                }

                if ( Velocity > Interceptor.MaxSpeed && Interceptor.MaxSpeed > 0 ) {
                    Velocity = Interceptor.MaxSpeed;
                }

                /* Dynamic update must be iterative */
                Range += (Velocity + oldVel) * 0.5 * DeltaTime;

                if ( Missile.Position.Length() <= Range ) {
                    // QueryTime = i * DeltaTime;
                    calculationValid = true;
                    /* Reached estimation */
                    break;
                }
            }

            /* TODO : fix overshoot calculation */
            PredictedLocation = Missile.Position + InitialOffset;
            return calculationValid;
        }
    }
};
