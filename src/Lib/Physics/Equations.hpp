#ifndef Physics_Equations_H
#define Physics_Equations_H
#include "Constants.hpp"
#include <iostream>

/// @brief F = G * m1 * m2 / r^2
/// @tparam T Type of number
/// @param force Gravitational force
/// @param mass1 Mass of object 1
/// @param mass2 Mass of object 2
/// @param distance Distance between centers of 2 objects
/// @return Gravitational force, mass 1, mass 2 or distance depending on input values
template <typename T>
Unit<T> GravitationalForce(Newton<T> force, Kilogram<T> mass1, Kilogram<T> mass2, Metre<T> distance) {
    if (std::isnan(force.GetValue())) return (newtonianConstantOfGravitation * mass1 * mass2) / distance.Pow(2);
    if (std::isnan(distance.GetValue())) return ((newtonianConstantOfGravitation * mass1 * mass2) / force).Pow(0.5);
    if (std::isnan(mass1.GetValue())) return (force * distance.Pow(2)) / (newtonianConstantOfGravitation * mass2);
    if (std::isnan(mass2.GetValue())) return (force * distance.Pow(2)) / (newtonianConstantOfGravitation * mass1);
    else return Unit<T>(NAN);
}
/// @brief a = F / m
/// @tparam T Type of number
/// @param force Force
/// @param mass Mass
/// @param acceleration Acceleration
/// @return Force, mass or acceleration depending on input values
template <typename T>
Unit<T> ForceMassAcceleration(Newton<T> force, Kilogram<T> mass, MetrePerSecondSquared<T> acceleration) {
    if (std::isnan(force.GetValue())) return acceleration * mass;
    else if (std::isnan(mass.GetValue())) return force / acceleration;
    else if (std::isnan(acceleration.GetValue())) return force / mass;
    else return Unit<T>(NAN);
}

#endif