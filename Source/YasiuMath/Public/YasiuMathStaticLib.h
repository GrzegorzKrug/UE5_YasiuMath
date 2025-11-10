#pragma once

#include <vector>
#include <cassert>
#include <numbers>
#include <cmath>
#include <iostream>
#include <math.h>
#include <ostream>
#include <algorithm>
#include <unordered_set>


namespace YasiuMath {
    template<typename T>
    struct YASIUMATH_API Point {
        T x = 0;
        T y = 0;

        Point( T x, T y )
            : x(x), y(y) {}

        Point( std::pair<T, T> point )
            : x(point.first), y(point.second) {}

        T fastAngle()
        {
            if ( x == 0.f ) {
                return y;
            } else {
                return static_cast<T>(atan2(y, x));
            }
        }

        T fastAngleModded()
        {
            // return fastAngle();
            return static_cast<T>(fmod(fastAngle(), static_cast<T>(std::numbers::pi)));
        }
    };


    template<typename U, typename T>
    struct YASIUMATH_API IndexedPair {
        U index = 0;
        T first;
        T second;

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


    /* Object 'point' that has its angle related to center */
    template<typename T>
    struct YASIUMATH_API PointAngle {
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


// public:
    // template<typename T>
    // static std::vector<std::pair<T, T>> SpreadPointsOnArcByXY( const T X, const T Y, const T spreadDistance );

    // template<typename T>
    // static std::vector<std::pair<T, T>> SpreadPointsOnArcByAngleRadius( const T angle, const T radius,
    // const T spreadDistance );
    template<typename T>
    YASIUMATH_API inline T Degrees2Radians( T degree )
    {
        return std::numbers::pi * degree / 180.f;
    }

    template<typename T>
    YASIUMATH_API inline T Radians2Degrees( T radian )
    {
        return radian * 180.f / std::numbers::pi();
    }


    namespace Trigonometry {
        /* Spread points on tangent line to arc, angle is in radians. Distance between points is equal to given Spread Distance */

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
            T beta = static_cast<T>(std::numbers::pi / 2. - angle);

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
         * Spread points on tangent line to arc.
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

        /*
         * Finds radius of circle for both tangent line that intersect.
         * Tangents on circle are defined by angle Alfa and Beta.
         * Tangent line distance to meeting point from circle intersection is `symmetricWidth`.
         * Angle is in radians.
         * 
         *		sin (alfa) * A  - sin(beta) * b
         * R = ----------------------------------
         *		cos(beta) - cos(alfa)
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
            assert(angleDiff != 180); /// Only place when they don't meet in infinite space

            T nominator = symmetricWidth * (sin(alfa) + sin(beta));
            T denominator = cos(beta) - cos(alfa);
            T result = abs(nominator / denominator);

            return result;
        }

        /*
         * This function finds radius of circle both line segments end up touch each other ends.
         * Tangents on circle are defined by angle Alfa and Beta.
         * Tangent line distance to meeting point from circle intersection is `width`.
         * Angle is in radians.
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
                } else {
                    result.first = radius2;
                    result.second = radius1;
                }
            } else {
                /* Pack shorter radius to first */
                if ( radius1 < radius2 ) {
                    result.first = radius1;
                    result.second = radius2;
                } else {
                    result.first = radius2;
                    result.second = radius1;
                }
            }
            return result;
        }
    }


    namespace ConvexHull {
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

        /* Function to check backawrds if any previous points need to be removed */
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
            } else {
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
            } else {
                // std::cout << " Checked pt: " << checkIndex << "\n";
            }
            return;
        }

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

        template<typename T>
        static std::vector<int> ConvexHull( const std::vector<std::pair<T, T>>& polygonPoints )
        {
            if ( polygonPoints.size() == 0 ) {
                return {};
            } else if ( polygonPoints.size() == 1 ) {
                return {0};
            } else if ( polygonPoints.size() == 2 ) {
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
                } else if ( angle > angleThreshold ) {
                    pointsOnLeft.emplace_back(point.index, angle);
                } else {
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
                    if ( visitedPoints.contains(nextPoint.index) ) {
                        // std::cout << "Skipping visited point: " << nextPoint.index << "\n";
                        continue;
                    }
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
};
