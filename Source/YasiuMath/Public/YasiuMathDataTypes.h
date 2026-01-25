/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once

#include <cmath>
#include <stdexcept>
#include "YasiuConstants.h"

/// \file


/** @brief Categorized name spaces */
namespace YasiuMath {
    /** @brief Helper types used in algorithms */
    namespace Types {
        /** @brief Value used for number stability check */
        constexpr double EPSILON = 1e-7;

        /**
         * @brief Pair-like type, Helper for point description in ConvexHull algorithm
         * 
         * XY are point offset from (0,0) and this can be converted to abstract angle, starting from X+ axis around origin point
         * @warning Angle is not give in Deg/Rad!
         * @warning Angle is only used to identify order of point for Convex Hull
         */
        template<typename T>
        struct Point {
            /** X */
            T x = 0;
            /** Y */
            T y = 0;

            Point( T x, T y )
                : x(x), y(y) {}

            Point( std::pair<T, T> point )
                : x(point.first), y(point.second) {}

            /** @brief Get Angle from X/Y */
            T fastAngle()
            {
                if ( x == 0.f ) {
                    return y;
                }
                else {
                    return static_cast<T>(atan2(y, x));
                }
            }

            T fastAngleModded()
            {
                // return fastAngle();
                return static_cast<T>(fmod(fastAngle(), static_cast<T>(YasiuNums::Y_PI)));
            }
        };


        /**
         * @brief Helper object, for point description in algorithm.
         * Index and 2 values
         */
        template<typename U, typename T>
        struct IndexedPair {
            /** Index */
            U index = 0;
            /** First Value */
            T first = 0;
            /** Second Value */
            T second = 0;

            IndexedPair() = default;

            IndexedPair( U index, T first, T second )
                : index(index), first(first), second(second) {};

            Point<T> operator-( const IndexedPair<U, T>& other ) const
            {
                return Point<T>{first - other.first, second - other.second};
            }

            IndexedPair<U, T> subtract( const IndexedPair<U, T>& other ) const
            {
                return IndexedPair<U, T>{index, first - other.first, second - other.second};
            }

            bool operator==( const IndexedPair<U, T>& other ) const
            {
                return index == other.index;
            }
        };


        /**
         * @brief Helper Struct. Point description with index and angle
         * 
         * It has angle related to center (0,0).
         */
        template<typename T>
        struct PointAngle {
            int index = 0;
            T angle = 0;

            PointAngle( const int& index, T angle )
                : index(index), angle(angle) {}

            PointAngle( const IndexedPair<int, T>& point )
            {
                index = point.index;
                angle = atan2(point.second, point.first);
            }

            PointAngle( const int& ind, const T& x, const T& y )
                : index(ind)
            {
                angle = atan2(y, x);
            }

            bool operator<( const PointAngle<T>& other ) const
            {
                return angle < other.angle;
            }

            bool operator>( const PointAngle<T>& other ) const
            {
                return angle > other.angle;
            }
        };


        /** @brief Structure reflecting vector of size 3, Templated Type
         *
         * Has all basic operators like **+ - * /**
         * Has dot and cross product functions.
         */
        template<typename T>
        struct Vec3 {
            /** X */
            T X = 0;
            /** Y */
            T Y = 0;
            /** Z */
            T Z = 0;

            Vec3() {};

            Vec3( T in )
                : X(in), Y(in), Z(in) {};

            Vec3( T xin, T yin, T zin )
                : X(xin), Y(yin), Z(zin) {};

            template<typename U>
            Vec3( const Vec3<U>& other )
                : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)), Z(static_cast<T>(other.Z)) {}

            // template<typename U>
            // Vec3( const Vec3<U>& other )
            // : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)), Z(static_cast<T>(other.Z)) {}

            // Addition
            Vec3 operator+( const Vec3& other ) const
            {
                return Vec3(X + other.X, Y + other.Y, Z + other.Z);
            }

            Vec3& operator+=( const Vec3& other )
            {
                X += other.X;
                Y += other.Y;
                Z += other.Z;
                return *this;
            }

            // Subtraction
            Vec3 operator-( const Vec3& other ) const
            {
                return Vec3(X - other.X, Y - other.Y, Z - other.Z);
            }

            Vec3& operator-=( const Vec3& other )
            {
                X -= other.X;
                Y -= other.Y;
                Z -= other.Z;
                return *this;
            }

            // Scalar multiplication
            Vec3 operator*( T scalar ) const
            {
                return Vec3(X * scalar, Y * scalar, Z * scalar);
            }

            Vec3 operator*( Vec3 other ) const
            {
                return Vec3(X * other.X, Y * other.Y, Z * other.Z);
            }

            Vec3& operator*=( T scalar )
            {
                X *= scalar;
                Y *= scalar;
                Z *= scalar;
                return *this;
            }

            // Scalar division
            Vec3 operator/( T scalar ) const
            {
                return Vec3(X / scalar, Y / scalar, Z / scalar);
            }

            Vec3& operator/=( T scalar )
            {
                X /= scalar;
                Y /= scalar;
                Z /= scalar;
                return *this;
            }

            Vec3 operator/( const Vec3& other ) const
            {
                return Vec3(X / other.X, Y / other.Y, Z / other.Z);
            }

            Vec3& operator/=( const Vec3& other )
            {
                X /= other.X;
                Y /= other.Y;
                Z /= other.Z;
                return *this;
            }

            /** @brief All elements must match */
            bool operator==( const Vec3& other ) const
            {
                return X == other.X && Y == other.Y && Z == other.Z;
            }

            /** @brief OnlY 1 can be different */
            bool operator!=( const Vec3& other ) const
            {
                return X != other.X || Y != other.Y || Z != other.Z;
            }

            /** @brief Compare magnitudes */
            bool operator>( const T value ) const
            {
                return Length() > value;
            }

            /** @brief Compare magnitudes */
            bool operator<( const T value ) const
            {
                return Length() < value;
            }

            /** @brief Compare magnitudes */
            bool operator>( const Vec3& other ) const
            {
                return LengthSquared() > other.LengthSquared();
            }

            /** @brief Compare magnitudes */
            bool operator<( const Vec3& other ) const
            {
                return LengthSquared() < other.LengthSquared();
            }

            /** @brief Dot product of 2 vectors */
            T Dot( const Vec3& other ) const
            {
                return X * other.X + Y * other.Y + Z * other.Z;
            }

            /** @brief Cross product of 2 vectors */
            Vec3 Cross( const Vec3& other ) const
            {
                return Vec3(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);
            }

            /** @brief Length^2 */
            T LengthSquared() const
            {
                return X * X + Y * Y + Z * Z;
            }

            T Length() const
            {
                return static_cast<T>(std::sqrt(LengthSquared()));
            }

            /** @brief Vector with absolute values */
            Vec3 Abs() const
            {
                return Vec3{fabs(X), fabs(Y), fabs(Z)};
            }

            /** @brief Sum inner elements */
            T Sum() const
            {
                return X + Y + Z;
            }

            /** @brief Sum of absolute values of components */
            T AbsSum() const
            {
                return fabs(X) + fabs(Y) + fabs(Z);
            }

            /** @brief Return copy of normalized vector */
            Vec3 Normalize() const
            {
                // Vec3 other = *this;
                // other.NormalizeInPlace();
                return Vec3(*this).NormalizeInPlace();
            }

            /** @brief Normalize this vector */
            Vec3& NormalizeInPlace()
            {
                T len = Length();
                if ( len != 0 ) {
                    X /= len;
                    Y /= len;
                    Z /= len;
                }
                return *this;
            }

            /** @brief Check if vector has any element other than 0, uses \ref EPSILON */
            bool IsNearly0( const T EPS = EPSILON ) const
            {
                return AbsSum() < EPS;
            }
        };


        /** @brief Ballistic state. Can move in time with \ref Step function
         *
         * Self-sufficient to predict next state with \ref Step and \ref MultiStep
         * @warning AirFrictionCoeff must be within <0, 1> range, otherwise model will not be correct or could crash.
         * 
         */
        template<typename T>
        struct ProjectileDynamicState {
            virtual ~ProjectileDynamicState() {};

            /** @brief Current position */
            Vec3<T> Position = 0;

            /** @brief Movement velocity vector */
            Vec3<T> Velocity = 0;

            /** @brief Acceleration or Deceleration or combined with Gravity Forces */
            Vec3<T> ThrustVector = 0;

            /** @brief Movement resistance opposite to current velocity vector. Must be in <0, 1> range
             *
             * $$Drag = Velocity^2 * Coeff$$
             */
            T AirFrictionCoeff = 0;

            /** @brief Maximal speed(length) of velocity vector */
            T MaxSpeed = 1000;

            /** @brief Step function, calling \ref Step in loop until QueryTime is reached with DeltaTime step
             *
             * Loop count is **N = QueryTime / DeltaTime**
             * 
             * @param QueryTime Time to step into future
             * @param DeltaTime Time for single step, suggested range <0.1, 1>
             * @note Last step has very small DeltaTime depending on math error
             */
            virtual void MultiStep( T QueryTime, T DeltaTime )
            {
                const int N = QueryTime / DeltaTime;
                for ( int i = 0; i < N; i++ ) {
                    Step(DeltaTime);
                }

                T missingTime = QueryTime - (DeltaTime * N);
                if ( missingTime > EPSILON ) {
                    Step(missingTime);
                }
            }

            /** @brief Main function used to modify state in time
             * @param deltaTime Time distance to move at single step
             * 
             */
            virtual void Step( T deltaTime )
            {
                /* Acceleration */
                if ( !ThrustVector.IsNearly0() ) {
                    Velocity += ThrustVector * deltaTime;
                }

                /* Friction as last step to damp all forces */
                if ( AirFrictionCoeff > 0 ) {
                    /* Preserve sign! not loose it with square */
                    Vec3<T> drag = (Velocity * Velocity.Abs()) * AirFrictionCoeff * deltaTime;
                    Velocity -= drag;
                }


                /* Speed limit */
                // if ( !ThrustVector.IsNearly0() ) {
                /* Not need for checking Velocity!=0 safety/optimization */
                if ( Velocity.Length() > MaxSpeed ) {
                    Velocity = Velocity.Normalize() * MaxSpeed;
                }
                // }

                Position += Velocity * deltaTime;
            };

            /**
             * @brief Predicts ballistic state at time, Ignores Air Resistance and MaxSpeed.
             * Complexity: O(1).
             *
             * @param ProjectileState Starting state, that will be modified and returned
             * @param QueryTime Time To Predict
             * @return Flag telling if result is Accurate, or approximation, see note
             *
             * @note Return flag notify only if **AirDrag** or **MaxSpeed** could influence results.
             * @note If AirResistance is 0 and max speed was not reached it return flag will be **true**
             * @note Otherwise flag is **false** indicating estimation has error (possibly large on longer periods)
             * @note Use \ref Predict for accurate state
             */
            static bool PredictQuick( ProjectileDynamicState& ProjectileState, T QueryTime )
            {
                /* Static because must return bool ( can not return Copied value )
                 * Formula:
                 * | V + At | = V_max
                 * V_max is scalar, max speed
                 * V - Velocity vec
                 * A - Acceleration/thrust vec
                 * t - time
                 * 
                 * 0 = t^2*A^2 + 2t*VA + V^2-V_max^2
                 * Now-> do components sum x+y+z
                 */
                // PredictionState = *this;

                // OutState = *this;
                if ( ProjectileState.ThrustVector.IsNearly0() ) {
                    ProjectileState.Position = ProjectileState.Position + ProjectileState.Velocity * QueryTime;
                    return true;
                }

                T Xa = ProjectileState.ThrustVector.LengthSquared();
                T Xb = 2 * (ProjectileState.Velocity * ProjectileState.ThrustVector).Sum();
                T Xc = ProjectileState.Velocity.LengthSquared() - ProjectileState.MaxSpeed * ProjectileState.MaxSpeed;

                T delta = Xb * Xb - 4 * Xa * Xc;
                T TimeToMaxSpeed = -1;
                if ( delta < 0 ) {
                    /* Technically should never occur
                     * Accel=0 is computed before
                     */
                    throw std::runtime_error("Delta negative. Can't return false. Solution is not Valid at all.");
                    // return false;
                }
                else if ( fabs(delta) <= EPSILON ) {
                    /* 1 solution ~ 0 */
                    TimeToMaxSpeed = -Xb / (2 * Xa);
                }
                else {
                    /* 2 Solutions */
                    T x1 = (-Xb - std::sqrt(delta)) / (2 * Xa);
                    T x2 = (-Xb + std::sqrt(delta)) / (2 * Xa);

                    if ( x1 > 0 ) {
                        TimeToMaxSpeed = x1;
                    }
                    else if ( x2 > 0 ) {
                        TimeToMaxSpeed = x2;
                    }
                    if ( x1 > 0 && x2 > 0 && x1 < x2 ) {
                        TimeToMaxSpeed = x1;
                    }
                }

                if ( TimeToMaxSpeed < 0 ) {
                    /* Wrong params? */
                    return false;
                }
                if ( QueryTime <= TimeToMaxSpeed ) {
                    return true;
                }


                /* Estimated, No Speed Clipping */
                return false;
            }

            /** @brief Same as \ref MultiStep but does not modify object and returns copy of it at predicted time
             *
             * Accuracy depends on DeltaTime. Big delta is faster but with bigger numeric error.
             * 
             * @param QueryTime Predict position at this time
             * @param DeltaTime Time for single step, suggested range <0.1, 1>
             * @return New State at query time
             */
            ProjectileDynamicState Predict( T QueryTime, T DeltaTime ) const
            {
                ProjectileDynamicState object = *this;
                object.MultiStep(QueryTime, DeltaTime);
                return object;
            }
        };


        /** @brief Params used for ballistics predictions
         * 
         */
        struct InterceptorParams {
            /** @brief */
            Vec3<float> Position{0};

            /** @brief Speed at which object can start moving */
            float InitialSpeed{0};

            /** @brief Acceleration in any direction*/
            float Acceleration{0};

            /** @brief Max speed object can reach with acceleration, 0 or less is ingored */
            float MaxSpeed = {0};

            /** @brief Keep in <0, 1> range! */
            float AirResistance = {0};
        };
    }
}
