#pragma once
#include <boost/icl/interval.hpp>
//#include <span>
//#include <compare>
//#include <type_traits>


namespace TP::math
{
    /*template<typename T>
    using Interval = std::span<T>;*/

    // If you need bounds checking or more interval-like behavior:
    //template<typename T>
    //struct IntervalBounds {
    //    T start;
    //    T end;

    //    // Default constructor
    //    IntervalBounds() = default;

    //    // Constructor from bounds
    //    IntervalBounds(T s, T e) : start(s), end(e) {}

    //    // Comparison operators (C++20 spaceship operator)
    //    auto operator<=>(const IntervalBounds&) const = default;

    //    // Check if a value is within bounds
    //    bool contains(T value) const {
    //        return value >= start && value <= end;
    //    }

    //    // Check if intervals overlap
    //    bool overlaps(const IntervalBounds& other) const {
    //        return start <= other.end && end >= other.start;
    //    }
    //};
    //template<typename T>
    //using Interval = IntervalBounds<T>;


	template<typename T>
	using Interval = typename boost::icl::interval<T>::type;


}