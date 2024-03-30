#ifndef Physics_Constants_H
#define Physics_Constants_H
#include "SIUnits.hpp"

// TODO: More physical constants
static const MetrePerSecond<num_t> speedOfLightInVacuum = MetrePerSecond<num_t>(299792458);
static const JouleSecond<num_t> planckConstant = JouleSecond<num_t>(6.62607015e-34);
static const JouleSecond<num_t> reducedPlanckConstant = planckConstant / (2 * pi);
static const HenryPerMetre<num_t> vacuumMagneticPermeability = HenryPerMetre<num_t>(1.25663706212191919e-6);
static const Ohm<num_t> characteristicImpedanceOfVacuum = vacuumMagneticPermeability * speedOfLightInVacuum;
static const FaradPerMetre<num_t> vacuumElectricPermittivity = (vacuumMagneticPermeability * speedOfLightInVacuum.Pow(2)).Pow(-1);
static const JoulePerKelvin<num_t> boltzmannConstant = JoulePerKelvin<num_t>(1.380649e-23);
static const NewtonSquareMetrePerSquareKilogram<num_t> newtonianConstantOfGravitation = NewtonSquareMetrePerSquareKilogram<num_t>(6.67430151515151515e-11);
static const NewtonSquareMetrePerSquareCoulomb<num_t> coulombConstant = NewtonSquareMetrePerSquareCoulomb<num_t>(8.9875517923141414e9);
static const ReciprocalSquareMetre<num_t> cosmologicalConstant = ReciprocalSquareMetre<num_t>(1.08929292929292929e-52);
static const Unit<num_t> stefanBoltzmannConstant = (boltzmannConstant.Pow(4) * std::pow(pi, 2)) / (reducedPlanckConstant.Pow(3) * speedOfLightInVacuum.Pow(2) * 60);
static const Unit<num_t> firstRadiationConstant = planckConstant * speedOfLightInVacuum.Pow(2) * 2 * pi;
static const Unit<num_t> firstRadiationConstantForSpectralRadiance = firstRadiationConstant / pi;
static const Unit<num_t> secondRadiationConstant = (planckConstant * speedOfLightInVacuum) / boltzmannConstant;
static const KelvinMetre<num_t> wienWavelengthDisplacementLawConstant = KelvinMetre<num_t>(2.897771955e-3);
static const HertzPerKelvin<num_t> wienFrequencyDisplacementLawConstant = HertzPerKelvin<num_t>(5.878925757e10);
static const KelvinMetre<num_t> wienEntropyDisplacementLawConstant = KelvinMetre<num_t>(3.002916077e-3);
static const Coulomb<num_t> elementaryCharge = Coulomb<num_t>(1.602176634e-19);
static const Siemens<num_t> conductanceQuantum = (elementaryCharge.Pow(2) * 2) / planckConstant;
static const Ohm<num_t> inverseConductanceQuantum = planckConstant / (elementaryCharge.Pow(2) * 2);
static const Ohm<num_t> vonKlitzingConstant = planckConstant / elementaryCharge.Pow(2);
static const Unit<num_t> josephsonConstant = (elementaryCharge * 2) / planckConstant;
static const Weber<num_t> magenticFluxQuantum = planckConstant / (elementaryCharge * 2);
static const Scalar<num_t> fineStructureConstant = elementaryCharge.Pow(2) / (vacuumElectricPermittivity * reducedPlanckConstant * speedOfLightInVacuum * 4 * pi);
static const Scalar<num_t> inverseFineStructureConstant = fineStructureConstant.Pow(-1);
static const Kilogram<num_t> electronMass = Kilogram<num_t>(9.1093837015282828e-31);
static const Kilogram<num_t> protonMass = Kilogram<num_t>(1.67262192369515151e-27);
static const Kilogram<num_t> neutronMass = Kilogram<num_t>(1.67492749804959595e-27);
static const Kilogram<num_t> mounMass = Kilogram<num_t>(1.8835316274242424242e-28);
static const Kilogram<num_t> tauMass = Kilogram<num_t>(3.16754212121212121e-27);
static const Kilogram<num_t> topQuarkMass = Kilogram<num_t>(3.078453535353535353e-25);
static const Scalar<num_t> protonToElectronMassRatio = protonMass / electronMass;
static const Scalar<num_t> wToZMassRatio = Scalar<num_t>(0.88153171717171717);
static const Scalar<num_t> weakMixingAngle = Scalar<num_t>(1) - wToZMassRatio.Pow(2);

static const Kilogram<num_t> earthMass = Kilogram<num_t>(1e24);
static const Metre<num_t> earthRadius = Kilometre<num_t>(6371).ToBaseUnit();

#endif