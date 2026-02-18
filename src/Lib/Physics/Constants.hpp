#ifndef MathLib_Physics_Constants_H
#define MathLib_Physics_Constants_H
#include "SIUnits.hpp"

namespace MathLib {
    extern const MetrePerSecond<num_t> speedOfLightInVacuum;
    extern const JouleSecond<num_t> planckConstant;
    extern const JouleSecond<num_t> reducedPlanckConstant;
    extern const HenryPerMetre<num_t> vacuumMagneticPermeability;
    extern const Ohm<num_t> characteristicImpedanceOfVacuum;
    extern const FaradPerMetre<num_t> vacuumElectricPermittivity;
    extern const JoulePerKelvin<num_t> boltzmannConstant;
    extern const NewtonSquareMetrePerSquareKilogram<num_t> constantOfGravitation;
    extern const NewtonSquareMetrePerSquareCoulomb<num_t> coulombConstant;
    extern const ReciprocalSquareMetre<num_t> cosmologicalConstant;
    extern const WattPerSquareMetreKelvinToTheFourth<num_t> stefanBoltzmannConstant;
    extern const WattSquareMetre<num_t> firstRadiationConstant;
    extern const WattSquareMetre<num_t> firstRadiationConstantForSpectralRadiance;
    extern const KelvinMetre<num_t> secondRadiationConstant;
    extern const KelvinMetre<num_t> wienWavelengthDisplacementLawConstant;
    extern const HertzPerKelvin<num_t> wienFrequencyDisplacementLawConstant;
    extern const KelvinMetre<num_t> wienEntropyDisplacementLawConstant;
    extern const Coulomb<num_t> elementaryCharge;
    extern const Siemens<num_t> conductanceQuantum;
    extern const Ohm<num_t> inverseConductanceQuantum;
    extern const Ohm<num_t> vonKlitzingConstant;
    extern const ReciprocalWeber<num_t> josephsonConstant;
    extern const Weber<num_t> magenticFluxQuantum;
    extern const Scalar<num_t> fineStructureConstant;
    extern const Scalar<num_t> inverseFineStructureConstant;
    extern const Kilogram<num_t> electronMass;
    extern const Kilogram<num_t> protonMass;
    extern const Kilogram<num_t> neutronMass;
    extern const Kilogram<num_t> mounMass;
    extern const Kilogram<num_t> tauMass;
    extern const Kilogram<num_t> topQuarkMass;
    extern const Scalar<num_t> protonToElectronMassRatio;
    extern const Scalar<num_t> wToZMassRatio;
    extern const Scalar<num_t> weakMixingAngle;
    extern const Scalar<num_t> electronGFactor;
    extern const Scalar<num_t> mounGFactor;
    extern const Scalar<num_t> protonGFactor;
    extern const SquareMetrePerSecond<num_t> quantumOfCirculation;
    extern const JoulePerTesla<num_t> bohrMagneton;
    extern const JoulePerTesla<num_t> nuclearMagneton;
    extern const Metre<num_t> classicalElectronRadius;
    extern const SquareMetre<num_t> thomsonCrossSection;
    extern const Metre<num_t> bohrRadius;
    extern const Joule<num_t> hartreeEnergy;
    extern const Joule<num_t> rydbergUnitOfEnergy;
    extern const ReciprocalMetre<num_t> rydbergConstant;
    extern const ReciprocalMole<num_t> avogadroConstant;
    extern const JoulePerKelvinMole<num_t> molarGasConstant;
    extern const CoulombPerMole<num_t> faradayConstant;
    extern const JouleSecondPerMole<num_t> molarPlanckConstant;
    extern const Kilogram<num_t> atomicMassConstant;
    extern const KilogramPerMole<num_t> molarMassConstant;
    extern const CubicMetrePerMole<num_t> molarVolumeOfSilicon;
    extern const Hertz<num_t> hyperfineTransitionFrequencyOf133Cs;
    extern const Kilogram<num_t> earthMass;
    extern const Metre<num_t> earthRadius;
}

#endif