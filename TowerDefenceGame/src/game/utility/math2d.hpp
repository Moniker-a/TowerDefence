#pragma once
#include <utility>
#include <cmath>

//Returns the magnitude of a vector. Arguments dx and dy are x and y components of vector respectively.
template<typename T>
T vector_magnitude(T dx, T dy)
{
    return std::sqrt(dx*dx+dy*dy);
}

template <typename T>
std::pair<T, T> cartesian_to_polar(T dx, T dy)
{
    //Do conversion here.
    T magnitude = vector_magnitude(dx, dy);
    T theta = std::atan(dy/dx);
    return std::pair<T, T>(magnitude, theta); //Returns pair.first = magnitude, pair.second = theta.
}

template <typename T>
std::pair<T, T> polar_to_cartesian(T magnitude, T theta)
{
    T dx = magnitude*std::cos(theta);
    T dy = magnitude*std::sin(theta);
    return std::pair<T, T>(dx, dy);
}

//Returns a unit vector from the x and y components of a vector.
template <typename T>
std::pair<T, T> unit_vector(T dx, T dy)
{
    T magnitude = vector_magnitude(dx, dy);
    return std::pair<T, T>(dx/magnitude, dy/magnitude);
}
