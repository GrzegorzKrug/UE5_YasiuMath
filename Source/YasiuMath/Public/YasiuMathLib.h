/* 
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

/// \file

#pragma once

#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>
// #include <numbers>
// #include <iostream>
// #include <math.h>
// #include <ostream>
#include <algorithm>
#include <unordered_set>

#include "Constants.h"
#include "YasiuMathDataTypes.h"


#if __cplusplus >= 202002L
#define CPP20_OR_LATER 1
#else
#define CPP20_OR_LATER 0
#endif


namespace YasiuMath {
    template<typename T>
    using Vec3 = YasiuMath::Types::Vec3<T>;
    using Vec3f = YasiuMath::Types::Vec3<float>;
    using Vec3d = YasiuMath::Types::Vec3<double>;


    /** @brief Collection of functions working with angles and rotation */
    namespace AngleUtils {
        template<typename T>
        static inline double Degrees2Radians( T degree )
        {
            return static_cast<T>(YasiuMath::Constants::Y_PI * degree / 180.f);
        }

        template<typename T>
        static inline T Radians2Degrees( T radian )
        {
            return static_cast<T>(radian * 180.f / YasiuMath::Constants::Y_PI);
        }

        /**
         * @brief Normalize angle by removing full periods from value, result is in range <0, period>
         * 
         * Angle normalization removes any excessive amount of periods from its value.
         * @warning Does not support negative periods
         */
        template<typename T>
        static T NormalizeAngleToPeriod( T angle, T period = 360.f )
        {
            if ( period < 0 ) { return angle; }
            T temp = static_cast<T>(fmod(angle, period));
            if ( temp < 0 ) {
                temp += period;
            }
            return temp;
        }
    }


    /**
     * @brief Functions that calculate trigonometry problems in 2D / 3D space
     */
    namespace Trigonometry {
        /**
         * Spread points on tangent line to arc located by angle and radius of given arc.
         * Points are moved away from tangent point by spreadDistance
         * @tparam T 
         * @param angle 
         * @param radius 
         * @param spreadDistance 
         * @return 
         */
        template<typename T>
        std::vector<std::pair<T, T>> SpreadPointsOnTangentByAngleRadius( const T angle, const T radius, const T spreadDistance )
        {
            assert(radius > 0);
            assert(spreadDistance > 0);

            std::vector<std::pair<T, T>> result;

            /* Beta = 90 - alfa */
            T beta = static_cast<T>(YasiuMath::Constants::Y_PI / 2. - angle);

            /*
             * sin(B) = opposite / hypotenuse -> dy = sin * hypotenuse
             * cos(B) = adjacent / hypotenuse -> dx = cos * hypotenuse
             */

            // T halfSpread = spreadDistance / 2;
            T dy = sin(beta) * spreadDistance;
            T dx = cos(beta) * spreadDistance;

            T X = cos(angle) * radius;
            T Y = sin(angle) * radius;


            // Projected triangle aligns with axis different than +X +Y
            // Axis for X is positive, for Y is Negative ( angle goes anti-clockwise )
            // Starting from 0 = +X
            result.push_back({X + dx, Y - dy});
            // This is just negated vector
            result.push_back({X - dx, Y + dy});

            // std::tuple<float> test;
            // test
            // test.g

            return result;
        }

        /**
         * @brief Spread points on tangent line to arc.
         * 
         * XY is tangent point location, and arc center is 0,0.
         * Points are moved away from tangent point by spreadDistance
         * @tparam T 
         * @param X 
         * @param Y 
         * @param spreadDistance 
         * @return 
         */
        template<typename T>
        std::vector<std::pair<T, T>> SpreadPointsOnTangentByXY( const T X, const T Y, const T spreadDistance )
        {
            T angle = atan2f(Y, X);
            T radius = sqrtf((X * X) + (Y * Y));
            return SpreadPointsOnTangentByAngleRadius(angle, radius, spreadDistance);
        }

        /**
         * @brief Finds radius of circle for both tangent line that intersect.
         * 
         * Tangents on circle are defined by angle Alfa and Beta. Where point with angle 0 is has value \f$\{X, 0\}\f$ and goes counter-clockwise
         * 
         * Tangent line distance to meeting point from circle intersection is **symmetricWidth**.
         * 
         * Angle is in **radians**.
         * 
         *
         \f[
         R=\frac{\sin(\alpha)*A-\sin(\beta)*B}{\cos(\beta)-\cos(\alpha)}
         \f]
         *		
         * @tparam T 
         * @param alfa 
         * @param beta 
         * @param symmetricWidth 
         * @return 
         */
        template<typename T>
        T FindMinimalRadiusForIntersectingTangentsOnArc( const T alfa, const T beta, const T symmetricWidth )
        {
            /*
             *		sin (alfa) * A  - sin(beta) * b
             * R = ----------------------------------
             *		cos(beta) - cos(alfa)
             */
            T angleDiff = fmod(abs(alfa - beta), 360);
            assert(symmetricWidth > 0);
            assert(angleDiff > 0);
            assert(angleDiff != 180); // Only place when they don't meet in infinite space

            T nominator = symmetricWidth * (sin(alfa) + sin(beta));
            T denominator = cos(beta) - cos(alfa);
            T result = abs(nominator / denominator);

            return result;
        }

        /**
         * @brief Finds radius of 2 circles for both tangent line that intersect.
         * Tangents on circle are defined by angle Alfa and Beta. Where point with angle 0 is has value \f$\{X, 0\}\f$ and goes counter-clockwise
         * 
         * Tangent line distance to meeting point from circle intersection is **symmetricWidth**.
         * 
         * Angle is in **radians**.
         *  
         \f[
         R=\frac{\sin(\alpha)*A-\sin(\beta)*B}{\cos(\beta)-\cos(\alpha)}
         \f]
         *		
         * @tparam T 
         * @param alfa angle of first point
         * @param beta angle of second point
         * @param widthA distance for first point
         * @param widthB distance for second point
         * @return Pair of radius for each circle
         */
        template<typename T>
        std::pair<T, T> FindMinimalRadiusForIntersectingTangentsOnArcAsymmetric(
            const T alfa,
            const T beta,
            const T widthA,
            const T widthB
        )
        {
            std::pair<T, T> result;

            T angleDiff = fmod(abs(alfa - beta), 360); /* Keep angle in 0-360 range */
            assert(widthA > 0);
            assert(widthB > 0);
            assert(angleDiff > 0);
            assert(angleDiff != 180); /// Only place when they don't meet in infinite space

            /* X - formula case */
            T nominator1 = sin(alfa) * widthA + sin(beta) * widthB;
            T denominator1 = cos(alfa) - cos(beta);
            T radius1 = abs(nominator1 / denominator1);

            /* Y - formula case */
            T nominator2 = -(cos(alfa) * widthA + cos(beta) * widthB);
            T denominator2 = sin(alfa) - sin(beta);
            T radius2 = abs(nominator2 / denominator2);

            if ( widthA < widthB ) {
                /* Pack longer radius to first */
                if ( radius1 > radius2 ) {
                    result.first = radius1;
                    result.second = radius2;
                }
                else {
                    result.first = radius2;
                    result.second = radius1;
                }
            }
            else {
                /* Pack shorter radius to first */
                if ( radius1 < radius2 ) {
                    result.first = radius1;
                    result.second = radius2;
                }
                else {
                    result.first = radius2;
                    result.second = radius1;
                }
            }
            return result;
        }
    }


    /**
     * @brief Collection of convex hull functions and helper functions
     */
    namespace ConvexHull {
        /* @cond INTERNAL */
        /* Helper function for convex hull */
        template<typename T>
        inline std::pair<T, T> CalculateVector(
            const std::vector<std::pair<T, T>>& polygonPoints,
            const int& ind1,
            const int& ind2
        )
        {
            // if ( convexStack.size() < 2 ) {
            // 	return {static_cast<T>(0), static_cast<T>(0)};
            // }

            // const int ind1 = convexStack.at(convexStack.size() - 2);
            // const int ind2 = convexStack.at(convexStack.size() - 1);
            // std::cout << "Polygon array size: " << polygonPoints.size() << ", ind1: " << ind1 << ", ind2: " << ind2
            // << std::endl;
            T dX = polygonPoints.at(ind1).first - polygonPoints.at(ind2).first;
            T dY = polygonPoints.at(ind1).second - polygonPoints.at(ind2).second;

            // std::cout << "Last vector: " << dX << " " << dY << " ( " << ind1 << ", " << ind2 << " )" << std::endl;
            return {dX, dY};
        }

        template<typename T>
        static T Cross( const std::pair<T, T>& A, const std::pair<T, T>& B )
        {
            auto temp1 = A.first * B.second; //- B.second * A.first;
            auto temp2 = A.second * B.first;
            return temp1 - temp2;
        }

        /* Function to check backwards if any previous points need to be removed */
        template<typename T>
        static void CheckHullBackwards(
            std::vector<int>& currentConvex,
            const int& checkIndex,
            const std::vector<std::pair<T, T>>& allPoints,
            const bool clockWise = false
        )
        {
            if ( currentConvex.size() < 2 ) { return; }

            T cross;
            if ( clockWise ) {
                std::pair<T, T> vec1 = CalculateVector<T>(allPoints, currentConvex.at(currentConvex.size() - 1), checkIndex);
                std::pair<T, T> vec2 = CalculateVector<T>(
                    allPoints,
                    currentConvex.at(currentConvex.size() - 2),
                    currentConvex.at(currentConvex.size() - 1)
                );
                cross = Cross<T>(vec1, vec2);
            }
            else {
                std::pair<T, T> vec1 = CalculateVector<T>(
                    allPoints,
                    currentConvex.at(currentConvex.size() - 1),
                    currentConvex.at(currentConvex.size() - 2)
                );
                std::pair<T, T> vec2 = CalculateVector<T>(allPoints, checkIndex, currentConvex.at(currentConvex.size() - 1));
                cross = Cross<T>(vec1, vec2);
            }
            // std::cout << "Checking point: " << checkIndex << ", cross: " << cross << std::endl;

            if ( cross <= 0 ) {
                /* Pop invalid, reassign new */
                // std::cout << "  - Removing previous point: " << currentConvex.at(currentConvex.size() - 1) << ", for: " << checkIndex << "\n";
                currentConvex.pop_back();
                // std::cout << "	  Last element now: " << currentConvex.at(currentConvex.size() - 1) << "\n";
                CheckHullBackwards(currentConvex, checkIndex, allPoints, clockWise);
            }
            else {
                // std::cout << " Checked pt: " << checkIndex << "\n";
            }
            return;
        }


        /**
         * @brief Helper function for convex calculation
         */
        template<typename T>
        static void AddPointToConvex(
            std::vector<int>& currentConvex,
            const int& index,
            const std::vector<std::pair<T, T>>& allPoints,
            const bool clockWise = false
        )
        {
            if ( currentConvex.size() <= 1 ) {
                currentConvex.push_back(index);
                return;
            }

            CheckHullBackwards(currentConvex, index, allPoints, clockWise);
            currentConvex.push_back(index);
            // std::cout << " +Hull point added: " << index << "\n";
        };

        /* @endcond */

        /**
         * @brief Find Convex Hull in 2D space from given points.
         * @tparam T numeric type of points
         * @param polygonPoints collection of points X,Y
         * @return Collection of indexes assigned to convex hull in order
         */
        template<typename T>
        static std::vector<int> ConvexHull2D( const std::vector<std::pair<T, T>>& polygonPoints )
        {
            using namespace Types;
            if ( polygonPoints.size() == 0 ) {
                return {};
            }
            else if ( polygonPoints.size() == 1 ) {
                return {0};
            }
            else if ( polygonPoints.size() == 2 ) {
                return {0, 1};
            }

            std::vector<IndexedPair<int, T>> sortedPoints; /* Points sorted in Y Axis */
            sortedPoints.reserve(polygonPoints.size() + 1);
            for ( int i = 0; i < static_cast<int>(polygonPoints.size()); i++ ) {
                sortedPoints.emplace_back(IndexedPair<int, T>(i, polygonPoints.at(i).first, polygonPoints.at(i).second));
            }

            std::sort(
                sortedPoints.begin(),
                sortedPoints.end(),
                [] ( const auto& a, const auto& b ) {
                    /* Ascending Y [y -> Y] */
                    return a.second < b.second;
                }
            );

            IndexedPair<int, T> bottom = sortedPoints.at(0);
            IndexedPair<int, T> top = sortedPoints.at(sortedPoints.size() - 1);
            Point<T> startEndDiff = top - bottom;
            T angleThreshold = startEndDiff.fastAngleModded();

            std::vector<PointAngle<T>> pointsOnLeft;
            std::vector<PointAngle<T>> pointsOnRight;

            /* == Splitting points to left and right == */
            for ( IndexedPair<int, T> point : sortedPoints ) {
                // std::cout << "\t" << point.index << " : " << point.first << ", " << point.second << std::endl;
                if ( point == bottom || point == top ) {
                    continue;
                }

                Point<T> temp = (point - bottom);
                T angle = temp.fastAngleModded();
                // std::cout << "Comparing angle(" << point.index << "): " << angle << " < " << angleThreshold << "\n";
                if ( point.first == top.first && point.first == bottom.second ) {
                    /* Ignore */
                }
                else if ( angle > angleThreshold ) {
                    pointsOnLeft.emplace_back(point.index, angle);
                }
                else {
                    pointsOnRight.emplace_back(point.index, angle);
                }
            }
            // std::cout << "\n";

            std::sort(pointsOnLeft.begin(), pointsOnLeft.end(), std::greater<PointAngle<T>>());
            std::sort(pointsOnRight.begin(), pointsOnRight.end());
            // return {};

            std::unordered_set<int> visitedPoints;
            visitedPoints.reserve(polygonPoints.size());

            /* GOING COUNTERCLOCKWISE FROM TOP */
            std::vector<int> convexLeft;
            std::vector<int> convexRight;
            convexLeft.reserve(polygonPoints.size());
            convexRight.reserve(polygonPoints.size());

            /* Initial 2 points */
            convexLeft.push_back(bottom.index);
            visitedPoints.insert(bottom.index);
            if ( pointsOnLeft.size() > 0 ) {
                convexLeft.push_back(pointsOnLeft.at(0).index);
                visitedPoints.insert(pointsOnLeft.at(0).index);
                Point<T> pt = CalculateVector<T>(polygonPoints, pointsOnLeft.at(0).index, bottom.index);

                // std::cout << "\nLeft Hull\n";
                for ( const PointAngle<T>& nextPoint : pointsOnLeft ) {
                    if ( nextPoint.index == top.index ) {
                        // std::cout << "This is top index, stop loop: " << nextPoint.index << "\n";
                        break;
                    }
#if CPP20_OR_LATER
                    if ( visitedPoints.contains(nextPoint.index) ) {
                        // std::cout << "Skipping visited point: " << nextPoint.index << "\n";
                        continue;
                    }
#else
                    if ( visitedPoints.find(nextPoint.index) != visitedPoints.end() ) {
                        // std::cout << "Skipping visited point: " << nextPoint.index << "\n";
                        continue;
                    }
#endif

                    // std::cout << nextPoint.index << "\n";

                    visitedPoints.insert(nextPoint.index);
                    AddPointToConvex<T>(convexLeft, nextPoint.index, polygonPoints, true);
                }
            }
            AddPointToConvex<T>(convexLeft, top.index, polygonPoints, true);
            // std::cout << "Convex L:\n";
            // for ( auto pt : convexLeft ) {
            // 	// std::cout << ", " << pt;
            // }
            // std::cout << std::endl;

            visitedPoints.clear();
            convexRight.push_back(bottom.index);
            visitedPoints.insert(bottom.index);
            if ( pointsOnRight.size() > 0 ) {
                convexRight.push_back(pointsOnRight.at(0).index);
                visitedPoints.insert(pointsOnRight.at(0).index);

                /* GOING COUNTERCLOCKWISE FROM BOTTOM */
                // std::cout << "\n\nPoints with angles right:\n";
                // std::cout << "\nRight Hull\n";
                for ( const PointAngle<T>& nextPoint : pointsOnRight ) {
                    if ( nextPoint.index == top.index ) {
                        // std::cout << "This is top index, stop loop: " << nextPoint.index << "\n";
                        break;
                    }
                    if ( visitedPoints.find(nextPoint.index) != visitedPoints.end() ) {
                        // std::cout << "Skipping visited point: " << nextPoint.index << "\n";
                        continue;
                    }

                    // std::cout << "= Next PT:" << nextPoint.index << "\n";
                    visitedPoints.insert(nextPoint.index);
                    AddPointToConvex<T>(convexRight, nextPoint.index, polygonPoints);
                }
            }
            AddPointToConvex<T>(convexRight, top.index, polygonPoints);
            // std::cout << "Convex R:\n";
            // for ( auto pt : convexRight ) {
            // 	std::cout << ", " << pt;
            // }
            // std::cout << std::endl;


            /* RESULTS */
            // std::cout << "\nPrinting points:\n";
            // std::cout << "\n";

            // std::cout << "Convex hull:\n";
            // for ( int ind : convexRight ) {
            // std::pair<T, T> point = polygonPoints.at(ind);
            // std::cout << " == " << ind << "\t" << point.first << " _ " << point.second << "\n";
            // }
            // std::cout << "===\n";

            // for ( int ind : convexLeft ) {
            // auto point = polygonPoints.at(ind);
            // std::cout << " == " << ind << "\t" << point.first << " _ " << point.second << "\n";
            // }

            // convexLeft.pop_back();
            // convexRight.pop_back();

            // for ( auto it = convexLeft.end() - 1; it != convexRight.begin(); --it ) {}
            // for ( int point : convexLeft ) {
            for ( auto it = convexLeft.end(); it != convexLeft.begin(); ) {
                --it;
                int point = *(it);
                if ( point == top.index || point == bottom.index ) { continue; }
                convexRight.push_back(point);
            }

            return convexRight;
        };

        // template<typename T>
        // static std::vector<std::pair<T, T>> MinBoundingBoxFromHull( const std::vector<std::pair<T, T>>& convexPoints )
        // {
        // 	auto temp = std::vector<std::pair<T, T>>{};
        // 	return temp;
        // };
        //
        // template<typename T>
        // static std::vector<std::pair<T, T>> MinBoundingBox( const std::vector<std::pair<T, T>>& polygonPoints )
        // {
        // 	const std::vector<int> indexes = ConvexHull<T>(polygonPoints);
        // 	std::vector<std::pair<T, T>> hullPoints;
        // 	hullPoints.reserve(indexes.size());
        // 	for ( auto ind : indexes ) {
        // 		hullPoints.push_back(polygonPoints.at(ind));
        // 	}
        // 	return MinBoundingBoxFromHull(hullPoints);
        // };
    }


    /** @brief Projectile movement functions */
    namespace Ballistics {
        /** @brief Ballistic state. Can move in time with \ref DiscreteStep function
         *
         * Self-sufficient to predict next state with \ref DiscreteStep and \ref AutoStep
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
            Vec3<T> Acceleration = 0;

            /** @brief Movement resistance opposite to current velocity vector. Must be in <0, 1> range. More in details...
             *
             * $$Drag = Velocity^2 * Coeff$$
             */
            T AirFrictionCoeff = 0;

            /** @brief Maximal speed(length) of velocity vector */
            T MaxSpeed = 1000;


            /** @brief Prediction 
             *
             * Accuracy depends on DeltaTime. Big delta is faster but with bigger numeric error.
             * 
             * @param QueryTime Predict position at this time
             * @param DeltaTime Time for single step, suggested range <0.1, 1>
             * @return New State at query time
             */
            ProjectileDynamicState Predict( T QueryTime, T DeltaTime ) const
            {
                ProjectileDynamicState<T> TempState = *this;
                TempState.AutoStep(QueryTime, DeltaTime);
                return TempState;
            }

            /**
             * @brief Quick prediction of ballistic state at time.
             * Limitation: Only constant acceleration and no Air Drag.
             * O(1)
             *
             * @param QueryTime Time To Predict
             * @return Flag tells if projectile has reached full speed or still accelerates
             *
             * @note Use \ref Predict for iterative solution when dealing with AirDrag
             */
            ProjectileDynamicState PredictQuick( T QueryTime ) const
            {
                /* Static because must return bool ( can not return Copied value )
                 * Formula:
                 * | V + At | = V_max
                 * V_max is scalar, max speed
                 * V - Velocity vec
                 * A - Acceleration/Thrust vec
                 * t - time
                 * 
                 * 0 = t^2 * A^2 + 2t * VA + V^2 - V_max^2
                 * Now-> do components sum x+y+z
                 */
                ProjectileDynamicState<T> ProjectileState = *this;

                if ( ProjectileState.Acceleration.IsNearly0() ) {
                    ProjectileState.Position = ProjectileState.Position + ProjectileState.Velocity * QueryTime;
                    return ProjectileState;
                }

                const T Xa = ProjectileState.Acceleration.LengthSquared();
                const T Xb = 2 * (ProjectileState.Velocity * ProjectileState.Acceleration).Sum();
                const T Xc = ProjectileState.Velocity.LengthSquared() - ProjectileState.MaxSpeed * ProjectileState.MaxSpeed;

                const T delta = Xb * Xb - 4 * Xa * Xc;
                T TimeToMaxSpeed = -1;

                if ( delta < 0 ) {
                    /* Technically should never occur
                     * Case(Accel=0) is computed before (Accel 0 can't fall into negative delta condition)
                     */
                    throw std::runtime_error("Delta negative. Can't return false. Solution is not Valid at all.");
                    // return false;
                }
                else if ( fabs(delta) <= YasiuMath::Constants::EPSILON ) {
                    /* 1 solution close to ~0 */
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
                        /* Pick smaller X only when both are valid */
                        TimeToMaxSpeed = x1;
                    }
                }

                /*  
                 * /\ Accel
                 * |    / |
                 * |   /  |
                 * |  /   |
                 * | /  X |
                 * ----------> Time
                 * X - Area is velocity -> Triangle formula 
                 */
                /* TimeMax<0 to catch error when limit is set 0, not to use <= to not use clamping */
                if ( QueryTime < TimeToMaxSpeed || TimeToMaxSpeed < 0 ) {
                    /* Travel to MaxSpeed only */
                    ProjectileState.Position += ProjectileState.Velocity * QueryTime + ProjectileState.Acceleration * (0.5 *
                        QueryTime * QueryTime);
                    ProjectileState.Velocity += ProjectileState.Acceleration * QueryTime;
                }
                else {
                    /* Acceleration Phase */
                    ProjectileState.Position += ProjectileState.Velocity * TimeToMaxSpeed + ProjectileState.Acceleration * (0.5 *
                        TimeToMaxSpeed * TimeToMaxSpeed);
                    /* Account for velocity changes */
                    ProjectileState.Velocity += ProjectileState.Acceleration * TimeToMaxSpeed;

                    /* Normalize to Max */
                    ProjectileState.Velocity = ProjectileState.Velocity.NormalizeInPlace() * ProjectileState.MaxSpeed;
                    /* Max Speed reached */
                    float RestofTime = QueryTime - TimeToMaxSpeed;
                    if ( RestofTime > 0 ) {
                        /* Add rest travel time */
                        ProjectileState.Position += ProjectileState.Velocity * RestofTime;
                    }
                }

                return ProjectileState;
            }

            /**
             * @brief Discrete Step. Function used to modify state in time.
             * @param deltaTime Time distance to move at single step.
             * 
             * @warning Using big deltaTime can lead to numeric inaccuracy when dealing with air drag.
             * 
             * @warning Keep **deltaTime** in small range <0.01, 1>. Suggested value 0.1.
             * 
             */
            virtual void DiscreteStep( T deltaTime )
            {
                auto oldVel = Velocity;
                /* Acceleration */
                if ( !Acceleration.IsNearly0() ) {
                    Velocity += (Acceleration) * deltaTime;
                }

                /* Friction as last step to damp all forces */
                if ( AirFrictionCoeff > 0 ) {
                    /* Preserve sign! not loose it with square */
                    Vec3<T> drag = (Velocity * Velocity.Abs()) * AirFrictionCoeff * deltaTime;
                    if ( drag.Length() >= Velocity.Length() ) {
                        Velocity = 0;
                    }
                    else {
                        Velocity -= drag;
                    }
                }


                /* Speed limit */
                /* Not need for checking Velocity!=0 safety/optimization */
                if ( MaxSpeed > 0 && Velocity.Length() > MaxSpeed ) {
                    Velocity = Velocity.NormalizeInPlace() * MaxSpeed;
                }

                Position += (oldVel + Velocity) * (deltaTime * 0.5);
            };


            /** @brief Step function. O(1) for AirDrag=0. 
             * 
             * Iterative prediction for models with AirDrag > 0
             * Loop count is **N = QueryTime / DeltaTime**
             *
             * @param QueryTime Time to predict into future
             * @param DeltaTime Step size, suggested range <0.1, 1> for stability.
             * 
             * @note Normally function uses \ref DiscreteStep, but switchest to \ref PredictQuick when AirDrag is 0, effectively being O(1)
             * 
             * @note Last step has very small DeltaTime depending on math error
             */
            void AutoStep( T QueryTime, T DeltaTime )
            {
                if ( AirFrictionCoeff == 0 ) {
                    const auto Result = PredictQuick(QueryTime);
                    *this = Result;
                }
                else {
                    const int N = QueryTime / DeltaTime;
                    for ( int i = 0; i < N; i++ ) {
                        DiscreteStep(DeltaTime);
                    }

                    const T missingTime = QueryTime - (DeltaTime * N);
                    if ( missingTime > 0 ) {
                        DiscreteStep(missingTime);
                    }
                }
            }
        };


        /** @brief Params used for ballistics predictions
         * 
         */
        template<typename T>
        struct InterceptorParams {
            /** @brief */
            Vec3f Position{0};

            /** @brief Speed at which object starts moving */
            T InitialSpeed{0};

            /** @brief Acceleration in any direction */
            T Acceleration{0};

            /** @brief Max speed object can reach with acceleration, 0 or less is ignored */
            T MaxSpeed = 0;

            /** @brief Keep in <0, 1> range! */
            T AirResistance = {0};
        };


        /** @brief Linear movement with linear velocity, Fast: O(1) 
        * 
        * @param InterceptLocation Position relative to bullet starting position
        * @param MissilePosition Current Relative position
        * @param MissileVelocity Current Vector
        * @param BulletSpeed Maximal bullet speed
        * @return Flag if solution is valid
        */
        template<typename T>
        bool InterceptMissile_Linear(
            Types::Vec3<T>& InterceptLocation,
            const Types::Vec3<T>& MissilePosition,
            const Types::Vec3<T>& MissileVelocity,
            const double BulletSpeed
        );

        /** @brief Iterative state prediction. Returns first possible intercept location and time.
         *
         * Accuracy is based on DeltaTime.
         *
         *
         *  @note There is no iteration limit. Loop is restricted by **N = QueryTime / DeltaTime**
         *  @note Suggested **Delta** range: <0.1 , 1>. Depending on space and distance.
         * 
         * @param PredictedLocation Predicted intercept location
         * @param Missile State of missile
         * @param Interceptor Params of interceptor
         * @param MaxQueryTime Prediction time to intercept
         * @param DeltaTime Prediction resolution, how big steps to make. Smaller steps = more steps.
         * @return Flag is solution found estimated intercept location
         */
        template<typename T>
        bool InterceptMissile_Dynamic(
            Types::Vec3<T>& PredictedLocation,
            ProjectileDynamicState<T> Missile,
            const InterceptorParams<T>& Interceptor,
            double MaxQueryTime = 10,
            double DeltaTime = 0.1
        );
    }


    /** @brief Any functions that don't have category */
    namespace Numeric {
        /** @brief Checks if value is nearly 0, \ref YasiuMath::Constants::EPSILON */
        template<typename T>
        static bool IsNearly0( T Value )
        {
            return std::fabs(Value) <= YasiuMath::Constants::EPSILON;
        }
    }


    /** @brief Functions that calculate something using algebra */
    namespace Algebra {
        /**
         * @brief Remaps Value between **input** range to **output** range with optional clamping.
         * @tparam T numeric type
         * @param Value Input value
         * @param InMin Input range low value
         * @param InMax Input range high value
         * @param OutMin Output range low value
         * @param OutMax Output range high value
         * @param ClampOutput Flag to clamp output
         * @return Value
         */
        template<typename T>
        static T Remap( const T Value, const T InMin, const T InMax, T OutMin, T OutMax, const bool ClampOutput = false )
        {
            const T SubVal = Value - InMin;
            const T Divid = InMax - InMin;

            if ( OutMax < OutMin ) {
                std::swap(OutMin, OutMax);
            }

            if ( Numeric::IsNearly0(Divid) ) {
                /* What to return? Any result will not be accurate, but 0 division goes to max */
                return OutMax;
            }

            const T Factor = SubVal / Divid;


            const T SubValOut = OutMax - OutMin;
            const T Output = (Factor * SubValOut) + OutMin;
            if ( ClampOutput ) {
                return std::clamp(Output, OutMin, OutMax);
            }

            return Output;
        }
    }
};
