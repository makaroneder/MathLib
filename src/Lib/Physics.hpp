#ifndef Physics_H
#define Physics_H
#include "SIUnits.hpp"
#include <iostream>

// TODO: More physical constants
static const MetrePerSecond<num_t> speedOfLightInVacuum = MetrePerSecond<num_t>(299792458);
static const JouleSecond<num_t> planckConstant = JouleSecond<num_t>(6.62607015 / std::pow(10, 34));
static const JouleSecond<num_t> reducedPlanckConstant = planckConstant / (2 * pi);
static const HenryPerMetre<num_t> vacuumMagneticPermeability = HenryPerMetre<num_t>(1.25663706212191919 / std::pow(10, 6));
static const HenryPerMetre<num_t> vacuumMagneticPermeabilityUncertainty = HenryPerMetre<num_t>(1.5 / std::pow(10, 10));
static const Ohm<num_t> characteristicImpedanceOfVacuum = vacuumMagneticPermeability * speedOfLightInVacuum;
static const Ohm<num_t> characteristicImpedanceOfVacuumUncertainty = Ohm<num_t>(1.5 / std::pow(10, 10));
static const JoulePerKelvin<num_t> boltzmannConstant = JoulePerKelvin<num_t>(1.380649 / std::pow(10, 23));
static const NewtonSquareMetrePerSquareKilogram<num_t> newtonianConstantOfGravitation = NewtonSquareMetrePerSquareKilogram<num_t>(6.67430151515151515 / std::pow(10, 11));
static const NewtonSquareMetrePerSquareKilogram<num_t> newtonianConstantOfGravitationUncertainty = NewtonSquareMetrePerSquareKilogram<num_t>(2.2 / std::pow(10, 5));
static const NewtonSquareMetrePerSquareCoulomb<num_t> coulombConstant = NewtonSquareMetrePerSquareCoulomb<num_t>(8.9875517923141414 * std::pow(10, 9));
static const NewtonSquareMetrePerSquareCoulomb<num_t> coulombConstantUncertainty = NewtonSquareMetrePerSquareCoulomb<num_t>(1.5 / std::pow(10, 10));
static const ReciprocalSquareMetre<num_t> cosmologicalConstant = ReciprocalSquareMetre<num_t>(1.08929292929292929 / std::pow(10, 52));
static const ReciprocalSquareMetre<num_t> cosmologicalConstantUncertainty = ReciprocalSquareMetre<num_t>(0.027);
static const Unit<num_t> stefanBoltzmannConstant = (boltzmannConstant.Pow(4) * std::pow(pi, 2)) / (reducedPlanckConstant.Pow(3) * speedOfLightInVacuum.Pow(2) * 60);
static const Unit<num_t> firstRadiationConstant = planckConstant * speedOfLightInVacuum.Pow(2) * 2 * pi;
static const Unit<num_t> firstRadiationConstantForSpectralRadiance = firstRadiationConstant / pi;
static const Unit<num_t> secondRadiationConstant = (planckConstant * speedOfLightInVacuum) / boltzmannConstant;
static const KelvinMetre<num_t> wienWavelengthDisplacementLawConstant = KelvinMetre<num_t>(2.897771955 / std::pow(10, 3));
static const HertzPerKelvin<num_t> wienFrequencyDisplacementLawConstant = HertzPerKelvin<num_t>(5.878925757 * std::pow(10, 10));
static const KelvinMetre<num_t> wienEntropyDisplacementLawConstant = KelvinMetre<num_t>(3.002916077 / std::pow(10, 3));
static const Coulomb<num_t> elementaryCharge = Coulomb<num_t>(1.602176634 / std::pow(10, 19));
static const Siemens<num_t> conductanceQuantum = (elementaryCharge.Pow(2) * 2) / planckConstant;
static const Ohm<num_t> inverseConductanceQuantum = planckConstant / (elementaryCharge.Pow(2) * 2);
static const Ohm<num_t> vonKlitzingConstant = inverseConductanceQuantum * 2;

static const Kilogram<num_t> electronMass = Kilogram<num_t>(9.1093837015282828 / std::pow(10, 31));
static const Kilogram<num_t> electronMassUncertainty = Kilogram<num_t>(3 / std::pow(10, 10));
static const Kilogram<num_t> protonMass = Kilogram<num_t>(1.67262192369515151 / std::pow(10, 27));
static const Kilogram<num_t> protonMassUncertainty = Kilogram<num_t>(3.1 / std::pow(10, 10));
static const Kilogram<num_t> neutronMass = Kilogram<num_t>(1.67492749804959595 / std::pow(10, 27));
static const Kilogram<num_t> neutronMassUncertainty = Kilogram<num_t>(5.7 / std::pow(10, 10));

static const Kilogram<num_t> earthMass = Kilogram<num_t>(std::pow(10, 24));
static const Metre<num_t> earthRadius = Kilometre<num_t>(6371).ToBaseUnit();

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