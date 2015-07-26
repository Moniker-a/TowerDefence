#pragma once
#include <utility>
#include <cmath>

namespace math2d
{
    //Returns the magnitude of a vector. Arguments dx and dy are x and y components of vector respectively.
    template<typename T>
    T vector_magnitude(T dx, T dy)
    {
        return std::sqrt(dx*dx+dy*dy);
    }

    //Returns distance between two points.
    template <typename T>
    T calc_distance(T x1, T y1, T x2, T y2)
    {
        return vector_magnitude(x1-x2, y1-y2);
    }

    //Returns the direction from one point to the other.
    template <typename T>
    T calc_direction(T fromX, T fromY, T toX, T toY)
    {
        return std::atan((toY-fromY)/(toX-fromX));
    }

    template <typename T>
    std::pair<T, T> cartesian_to_polar(T dx, T dy)
    {
        //Do conversion here.
        T magnitude = vector_magnitude(dx, dy);
        T theta = std::atan(dy/dx);
        return std::pair<T, T>(magnitude, theta); //Returns pair.first = magnitude, pair.second = theta.
    }

    template <typename T1, typename T2>
    std::pair<T1, T2> polar_to_cartesian(const T1 magnitude, const T2 theta)
    {
        T1 dx = magnitude*std::cos(theta);
        T1 dy = magnitude*std::sin(theta);
        return std::pair<T1, T1>(dx, dy);
    }

    //Returns a unit vector from the x and y components of a vector.
    template <typename T>
    std::pair<T, T> unit_vector(T dx, T dy)
    {
        T magnitude = vector_magnitude(dx, dy);
        return std::pair<T, T>(dx/magnitude, dy/magnitude);
    }
}
