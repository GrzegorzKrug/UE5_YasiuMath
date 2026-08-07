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
            const Vec3<T>& TargetPosition,
            const Vec3<T>& TargetVelocity,
            const T BulletSpeed,
            Vec3<T>& InterceptLocation,
            T& ImpactTime
        )
        {
            T a = TargetVelocity.Dot(TargetVelocity) - BulletSpeed * BulletSpeed;
            T b = 2.0f * TargetPosition.Dot(TargetVelocity);
            T c = TargetPosition.Dot(TargetPosition);


            if ( fabs(a) < 1e-6f ) {
                // Bullet speed ~= target speed
                if ( fabs(b) < 1e-6f )
                    return false; // No solution
                ImpactTime = -c / b;
                if ( ImpactTime < 0 )
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
                ImpactTime = (t1 > 0 && t2 > 0) ? fminf(t1, t2) : (t1 > 0 ? t1 : t2);
                if ( ImpactTime < 0 )
                    return false; // Both solutions negative
            }
            InterceptLocation = TargetPosition + TargetVelocity * ImpactTime;
            return true;
        }

        template<typename T>
        bool InterceptMissile_Dynamic(
            ProjectileDynamicState<T> Target,
            const InterceptorParams<T>& Interceptor,
            const T MaxQueryTime,
            const T StepTime,
            const T AccurateStepTime,
            Vec3<T>& HitLocation,
            T& HitTime
        )
        {
            bool calculationValid = false;
            const auto HalfStepTime = StepTime / 2;
            const auto HalfAccurateStepTime = AccurateStepTime / 2;

            Vec3<T> InitialOffset = Interceptor.Position;
            Target.Position -= InitialOffset;

            T Velocity = Interceptor.InitialSpeed;
            T Range = 0;

            if ( StepTime < YasiuMath::Constants::EPSILON ) {
                /* Too Small Eps */
                // throw std::runtime_error("Delta time in Intercept is too small");
                return false;
            }

            unsigned int StepsMax = static_cast<unsigned int>(floor(MaxQueryTime / StepTime));

            /* 1-Indexed for time calculation */
            unsigned int Steps = 0;
            auto PrevMissile = Target;
            T PrevRange = 0;
            T PrevVelocity = Velocity;

            for ( Steps = 1; Steps < (StepsMax + 1); Steps++ ) {
                /* Missile update */
                Target.DiscreteStep(StepTime);

                /* Interceptor state update */
                const auto OldVel = Velocity;
                Velocity += Interceptor.Acceleration * StepTime;


                if ( Interceptor.AirResistance > 0 ) {
                    /* Velocity is always positive */
                    const auto TrapezVelocity = (OldVel + Velocity) / 2;
                    const auto drag = TrapezVelocity * TrapezVelocity * Interceptor.AirResistance * StepTime;
                    Velocity -= drag;
                    if ( Velocity <= 0 ) {
                        return false;
                    }
                }

                if ( Velocity > Interceptor.MaxSpeed && Interceptor.MaxSpeed > 0 ) {
                    Velocity = Interceptor.MaxSpeed;
                }

                /* Must use velocity affected by drag */
                Range += (Velocity + OldVel) * HalfStepTime;

                if ( Target.Position.Length() <= Range ) {
                    calculationValid = true;
                    /* Reached estimation */
                    break;
                }

                PrevMissile = Target;
                PrevRange = Range;
                PrevVelocity = Velocity;
            }

            /* Phase 2 backward + Accurate estimation */
            if ( AccurateStepTime > 0 && AccurateStepTime < StepTime ) {
                Steps -= 1;
                Target = PrevMissile;
                Range = PrevRange;
                Velocity = PrevVelocity;

                /* SUBSTEP LOOP Duplicate */
                unsigned int AccSteps = 1;
                for ( AccSteps = 1; AccSteps < 100000; AccSteps++ ) {
                    /* Missile update */
                    Target.DiscreteStep(AccurateStepTime);

                    /* Interceptor state update */
                    const auto OldVel = Velocity;
                    Velocity += Interceptor.Acceleration * AccurateStepTime;


                    if ( Interceptor.AirResistance > 0 ) {
                        /* Velocity is always positive */
                        const auto TrapezVelocity = (OldVel + Velocity) / 2;
                        const auto drag = TrapezVelocity * TrapezVelocity * Interceptor.AirResistance * AccurateStepTime;
                        Velocity -= drag;
                        if ( Velocity <= 0 ) {
                            return false;
                        }
                    }

                    if ( Velocity > Interceptor.MaxSpeed && Interceptor.MaxSpeed > 0 ) {
                        Velocity = Interceptor.MaxSpeed;
                    }

                    /* Dynamic update must be iterative */
                    Range += (Velocity + OldVel) * HalfAccurateStepTime;

                    if ( Target.Position.Length() <= Range ) {
                        calculationValid = true;
                        /* Reached estimation */
                        break;
                    }
                }
                HitTime = Steps * StepTime + AccSteps * AccurateStepTime;
            }
            else {
                HitTime = Steps * StepTime;
            }

            HitLocation = Target.Position + InitialOffset;
            return calculationValid;
        }
    }
};
