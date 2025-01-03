#ifndef MathLib_Physics_Constants_H
#define MathLib_Physics_Constants_H
#include "SIUnits.hpp"

namespace MathLib {
    static const MetrePerSecond<num_t> speedOfLightInVacuum = MetrePerSecond<num_t>(299792458);
    static const JouleSecond<num_t> planckConstant = JouleSecond<num_t>(6.62607015e-34);
    static const JouleSecond<num_t> reducedPlanckConstant = planckConstant / (2 * pi);
    static const HenryPerMetre<num_t> vacuumMagneticPermeability = HenryPerMetre<num_t>(1.25663706212191919e-6);
    static const Ohm<num_t> characteristicImpedanceOfVacuum = vacuumMagneticPermeability * speedOfLightInVacuum;
    static const FaradPerMetre<num_t> vacuumElectricPermittivity = (vacuumMagneticPermeability * speedOfLightInVacuum.Pow(2)).Pow(-1);
    static const JoulePerKelvin<num_t> boltzmannConstant = JoulePerKelvin<num_t>(1.380649e-23);
    static const NewtonSquareMetrePerSquareKilogram<num_t> constantOfGravitation = NewtonSquareMetrePerSquareKilogram<num_t>(6.67430151515151515e-11);
    static const NewtonSquareMetrePerSquareCoulomb<num_t> coulombConstant = (vacuumElectricPermittivity * 4 * pi).Pow(-1);
    static const ReciprocalSquareMetre<num_t> cosmologicalConstant = ReciprocalSquareMetre<num_t>(1.08929292929292929e-52);
    static const WattPerSquareMetreKelvinToTheFourth<num_t> stefanBoltzmannConstant = (boltzmannConstant.Pow(4) * Pow(pi, 2)) / (reducedPlanckConstant.Pow(3) * speedOfLightInVacuum.Pow(2) * 60);
    static const WattSquareMetre<num_t> firstRadiationConstant = planckConstant * speedOfLightInVacuum.Pow(2) * 2 * pi;
    static const WattSquareMetre<num_t> firstRadiationConstantForSpectralRadiance = firstRadiationConstant / pi;
    static const KelvinMetre<num_t> secondRadiationConstant = (planckConstant * speedOfLightInVacuum) / boltzmannConstant;
    static const KelvinMetre<num_t> wienWavelengthDisplacementLawConstant = KelvinMetre<num_t>(2.897771955e-3);
    static const HertzPerKelvin<num_t> wienFrequencyDisplacementLawConstant = HertzPerKelvin<num_t>(5.878925757e10);
    static const KelvinMetre<num_t> wienEntropyDisplacementLawConstant = KelvinMetre<num_t>(3.002916077e-3);
    static const Coulomb<num_t> elementaryCharge = Coulomb<num_t>(1.602176634e-19);
    static const Siemens<num_t> conductanceQuantum = (elementaryCharge.Pow(2) * 2) / planckConstant;
    static const Ohm<num_t> inverseConductanceQuantum = planckConstant / (elementaryCharge.Pow(2) * 2);
    static const Ohm<num_t> vonKlitzingConstant = planckConstant / elementaryCharge.Pow(2);
    static const ReciprocalWeber<num_t> josephsonConstant = (elementaryCharge * 2) / planckConstant;
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
    static const Scalar<num_t> electronGFactor = Scalar<num_t>(-2.00231930436256353535);
    static const Scalar<num_t> mounGFactor = Scalar<num_t>(-2.0023318418131313);
    static const Scalar<num_t> protonGFactor = Scalar<num_t>(5.5856946893161616);
    static const SquareMetrePerSecond<num_t> quantumOfCirculation = planckConstant / (electronMass * 2);
    static const JoulePerTesla<num_t> bohrMagneton = (reducedPlanckConstant * elementaryCharge) / (electronMass * 2);
    static const JoulePerTesla<num_t> nuclearMagneton = (reducedPlanckConstant * elementaryCharge) / (protonMass * 2);
    static const Metre<num_t> classicalElectronRadius = elementaryCharge.Pow(2) * coulombConstant / (electronMass * speedOfLightInVacuum.Pow(2));
    static const SquareMetre<num_t> thomsonCrossSection = classicalElectronRadius.Pow(2) * (8 * pi / 3);
    static const Metre<num_t> bohrRadius = classicalElectronRadius / fineStructureConstant.Pow(2);
    static const Joule<num_t> hartreeEnergy = fineStructureConstant.Pow(2) * speedOfLightInVacuum.Pow(2) * electronMass;
    static const Joule<num_t> rydbergUnitOfEnergy = hartreeEnergy / 2;
    static const ReciprocalMetre<num_t> rydbergConstant = fineStructureConstant.Pow(2) * electronMass * speedOfLightInVacuum / (planckConstant * 2);
    static const ReciprocalMole<num_t> avogadroConstant = ReciprocalMole<num_t>(6.02214076e23);
    static const JoulePerKelvinMole<num_t> molarGasConstant = avogadroConstant * boltzmannConstant;
    static const CoulombPerMole<num_t> faradayConstant = avogadroConstant * elementaryCharge;
    static const JouleSecondPerMole<num_t> molarPlanckConstant = avogadroConstant * planckConstant;
    static const Kilogram<num_t> atomicMassConstant = Kilogram<num_t>(1.66053906660505050e-27);
    static const KilogramPerMole<num_t> molarMassConstant = avogadroConstant * atomicMassConstant;
    static const CubicMetrePerMole<num_t> molarVolumeOfSilicon = CubicMetrePerMole<num_t>(1.205883199606060e-5);
    static const Hertz<num_t> hyperfineTransitionFrequencyOf133Cs = Hertz<num_t>(9192631770);
    static const Kilogram<num_t> earthMass = Kilogram<num_t>(1e24);
    static const Metre<num_t> earthRadius = Kilometre<num_t>(6371).ToBaseUnit();
}

#endif