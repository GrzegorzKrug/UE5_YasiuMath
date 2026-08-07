/* 
 * Copyright (c) 2026 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once


#include "Constants.h"


#include <cmath>
#include <utility>


/// \file


/** @brief Categorized name spaces */
namespace YasiuMath {
    /** @brief Generic Types used in library */
    namespace Types {
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
                return static_cast<T>(fmod(fastAngle(), static_cast<T>(YasiuMath::Constants::Y_PI)));
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
         * Vector implements arithmetic operators **
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
            constexpr Vec3( const Vec3<U>& other )
                : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)), Z(static_cast<T>(other.Z)) {}


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

            /** @brief Default Pythagoras Length */
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

            /** @brief Check if vector has any element other than 0, uses \ref YasiuMath::Constants::EPSILON */
            bool IsNearly0( const T EPS = YasiuMath::Constants::EPSILON ) const
            {
                return AbsSum() < EPS;
            }
        };
    }
}
