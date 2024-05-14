#ifndef Physics_SIUnits_H
#define Physics_SIUnits_H
#include "../MathObject.hpp"
#include "../Constants.hpp"
#include "../Printable.hpp"
#include <array>

template <typename T>
struct Unit : Printable {
    CreateOperators(Unit<T>, T)
    enum BaseUnit : size_t {
        AmountOfSubstance,
        ElectricCurrent,
        Length,
        LuminousIntensity,
        Mass,
        ThermodynamicTemperature,
        Time,
        End,
    };
    Unit<T>(T count_ = 0, T amountOfSubstance_ = 0, T electricCurrent_ = 0, T length_ = 0, T luminousIntensity_ = 0, T mass_ = 0, T thermodynamicTemperature_ = 0, T time_ = 0) : count(count_) {
        this->baseUnits[BaseUnit::AmountOfSubstance] = amountOfSubstance_;
        this->baseUnits[BaseUnit::ElectricCurrent] = electricCurrent_;
        this->baseUnits[BaseUnit::Length] = length_;
        this->baseUnits[BaseUnit::LuminousIntensity] = luminousIntensity_;
        this->baseUnits[BaseUnit::Mass] = mass_;
        this->baseUnits[BaseUnit::ThermodynamicTemperature] = thermodynamicTemperature_;
        this->baseUnits[BaseUnit::Time] = time_;
    }
    Unit<T>(T count_, std::array<T, BaseUnit::End> baseUnits_) : count(count_) {
        this->baseUnits[BaseUnit::AmountOfSubstance] = baseUnits_[BaseUnit::AmountOfSubstance];
        this->baseUnits[BaseUnit::ElectricCurrent] = baseUnits_[BaseUnit::ElectricCurrent];
        this->baseUnits[BaseUnit::Length] = baseUnits_[BaseUnit::Length];
        this->baseUnits[BaseUnit::LuminousIntensity] = baseUnits_[BaseUnit::LuminousIntensity];
        this->baseUnits[BaseUnit::Mass] = baseUnits_[BaseUnit::Mass];
        this->baseUnits[BaseUnit::ThermodynamicTemperature] = baseUnits_[BaseUnit::ThermodynamicTemperature];
        this->baseUnits[BaseUnit::Time] = baseUnits_[BaseUnit::Time];
    }
    virtual constexpr T GetValue(void) const {
        return count;
    }
    constexpr T GetBaseUnit(size_t i) const {
        return baseUnits.at(i);
    }
    virtual std::string ToString(std::string padding = "") const override {
        if (count == 0) return padding + "0";
        std::string str = count == 1 ? "" : std::to_string(count);
        bool first = true;
        for (size_t i = 0; i < BaseUnit::End; i++) {
            if (baseUnits[i] != 0) {
                if (!first) str += std::string(" * ");
                str += baseUnitsStr[i];
                if (baseUnits[i] != 1) str += "^" + std::to_string(baseUnits[i]);
                first = false;
            }
        }
        return padding + ((first && count == 1) ? "1" : str);
    }
    constexpr Unit<T> Pow(T scalar) const {
        return Unit<T>(
            std::pow(count, scalar),
            this->baseUnits[BaseUnit::AmountOfSubstance] * scalar,
            this->baseUnits[BaseUnit::ElectricCurrent] * scalar,
            this->baseUnits[BaseUnit::Length] * scalar,
            this->baseUnits[BaseUnit::LuminousIntensity] * scalar,
            this->baseUnits[BaseUnit::Mass] * scalar,
            this->baseUnits[BaseUnit::ThermodynamicTemperature] * scalar,
            this->baseUnits[BaseUnit::Time] * scalar
        );
    }
    constexpr Unit<T> operator*(Unit<T> other) const {
        return Unit<T>(
            count * other.count,
            this->baseUnits[BaseUnit::AmountOfSubstance] + other.baseUnits[BaseUnit::AmountOfSubstance],
            this->baseUnits[BaseUnit::ElectricCurrent] + other.baseUnits[BaseUnit::ElectricCurrent],
            this->baseUnits[BaseUnit::Length] + other.baseUnits[BaseUnit::Length],
            this->baseUnits[BaseUnit::LuminousIntensity] + other.baseUnits[BaseUnit::LuminousIntensity],
            this->baseUnits[BaseUnit::Mass] + other.baseUnits[BaseUnit::Mass],
            this->baseUnits[BaseUnit::ThermodynamicTemperature] + other.baseUnits[BaseUnit::ThermodynamicTemperature],
            this->baseUnits[BaseUnit::Time] + other.baseUnits[BaseUnit::Time]
        );
    }
    constexpr Unit<T> operator/(Unit<T> other) const {
        return Unit<T>(
            count / other.count,
            this->baseUnits[BaseUnit::AmountOfSubstance] - other.baseUnits[BaseUnit::AmountOfSubstance],
            this->baseUnits[BaseUnit::ElectricCurrent] - other.baseUnits[BaseUnit::ElectricCurrent],
            this->baseUnits[BaseUnit::Length] - other.baseUnits[BaseUnit::Length],
            this->baseUnits[BaseUnit::LuminousIntensity] - other.baseUnits[BaseUnit::LuminousIntensity],
            this->baseUnits[BaseUnit::Mass] - other.baseUnits[BaseUnit::Mass],
            this->baseUnits[BaseUnit::ThermodynamicTemperature] - other.baseUnits[BaseUnit::ThermodynamicTemperature],
            this->baseUnits[BaseUnit::Time] - other.baseUnits[BaseUnit::Time]
        );
    }
    constexpr Unit<T> operator*=(Unit<T> other) {
        count *= other.count;
        for (size_t i = 0; i < BaseUnit::End; i++) this->baseUnits[i] += other.baseUnits[i];
        return *this;
    }
    constexpr Unit<T> operator/=(Unit<T> other) {
        count /= other.count;
        for (size_t i = 0; i < BaseUnit::End; i++) this->baseUnits[i] -= other.baseUnits[i];
        return *this;
    }
    constexpr bool operator==(Unit<T> other) const {
        bool ret = true;
        for (size_t i = 0; i < BaseUnit::End && ret; i++) ret = this->baseUnits[i] == other.baseUnits[i];
        return ret && this->count == other.count;
    }

    protected:
    T count;
    std::array<T, BaseUnit::End> baseUnits;
    static constexpr const char* baseUnitsStr[] = {
        "mol", "A", "m", "cd", "kg", "K", "s",
    };

    private:
    void Add(Unit<T> other) {
        for (size_t i = 0; i < BaseUnit::End; i++)
            if (this->baseUnits[i] != other.baseUnits[i]) throw std::runtime_error("Invalid units added");
        this->count += other.count;
    }
    void Multiply(T scalar) {
        this->count *= scalar;
    }
};
template <typename T>
struct Gram : Unit<T> {
    Gram<T>(T count_ = 0) : Unit<T>(count_ / 1000, 0, 0, 0, 0, 1) {}
    Gram<T>(Unit<T> other) : Unit<T>(other.GetValue(), 0, 0, 0, 0, 1) {
        for (size_t i = 0; i < Unit<T>::BaseUnit::End; i++)
            if (this->baseUnits[i] != other.GetBaseUnit(i)) throw std::runtime_error("Invalid units converted");
    }
    virtual constexpr T GetValue(void) const override {
        return this->count * 1000;
    }
};

#define CreateUnit(name, ...)                                                                                       \
template <typename T>                                                                                               \
struct name : Unit<T> {                                                                                             \
    name<T>(T count_ = 0) : Unit<T>(count_, __VA_ARGS__) {}                                                         \
    name<T>(Unit<T> other) : Unit<T>(other.GetValue(), __VA_ARGS__) {                                               \
        for (size_t i = 0; i < Unit<T>::BaseUnit::End; i++)                                                         \
            if (this->baseUnits[i] != other.GetBaseUnit(i)) throw std::runtime_error("Invalid units converted");    \
    }                                                                                                               \
}
#define CreateAlternativeUnit(name, base, mul)                                                                      \
template <typename T>                                                                                               \
struct name : base<T> {                                                                                             \
    name<T>(T count_ = 0) : base<T>(count_ * (mul)) {}                                                              \
    name<T>(base<T> other) : base<T>(other.GetValue()) {                                                            \
        for (size_t i = 0; i < Unit<T>::BaseUnit::End; i++)                                                         \
            if (this->baseUnits[i] != other.GetBaseUnit(i)) throw std::runtime_error("Invalid units converted");    \
    }                                                                                                               \
    constexpr base<T> ToBaseUnit(void) const {                                                                      \
        return base<T>(this->count);                                                                                \
    }                                                                                                               \
    virtual constexpr T GetValue(void) const override {                                                             \
        return this->count / (mul);                                                                                 \
    }                                                                                                               \
}
#define CreateUnitPrefix(name, base)                                                                                \
CreateAlternativeUnit(Quetta##name, base, 1e30);                                                        \
CreateAlternativeUnit(Ronna##name, base, 1e27);                                                         \
CreateAlternativeUnit(Yotta##name, base, 1e24);                                                         \
CreateAlternativeUnit(Zetta##name, base, 1e21);                                                         \
CreateAlternativeUnit(Exa##name, base, 1e18);                                                           \
CreateAlternativeUnit(Peta##name, base, 1e15);                                                          \
CreateAlternativeUnit(Tera##name, base, 1e12);                                                          \
CreateAlternativeUnit(Giga##name, base, 1e9);                                                           \
CreateAlternativeUnit(Mega##name, base, 1e6);                                                           \
CreateAlternativeUnit(Kilo##name, base, 1e3);                                                           \
CreateAlternativeUnit(Hecto##name, base, 1e2);                                                          \
CreateAlternativeUnit(Deca##name, base, 1e1);                                                           \
CreateAlternativeUnit(Deci##name, base, 1e-1);                                                          \
CreateAlternativeUnit(Centi##name, base, 1e-2);                                                         \
CreateAlternativeUnit(Milli##name, base, 1e-3);                                                         \
CreateAlternativeUnit(Micro##name, base, 1e-6);                                                         \
CreateAlternativeUnit(Nano##name, base, 1e-9);                                                          \
CreateAlternativeUnit(Pico##name, base, 1e-12);                                                         \
CreateAlternativeUnit(Femto##name, base, 1e-15);                                                        \
CreateAlternativeUnit(Atto##name, base, 1e-18);                                                         \
CreateAlternativeUnit(Zepto##name, base, 1e-21);                                                        \
CreateAlternativeUnit(Yocto##name, base, 1e-24);                                                        \
CreateAlternativeUnit(Ronto##name, base, 1e-27);                                                        \
CreateAlternativeUnit(Quecto##name, base, 1e-30);                                                       \
CreateAlternativeUnit(Kibi##name, base, std::pow(2, 10));                                                           \
CreateAlternativeUnit(Mebi##name, base, std::pow(2, 20));                                                           \
CreateAlternativeUnit(Gibi##name, base, std::pow(2, 30));                                                           \
CreateAlternativeUnit(Tebi##name, base, std::pow(2, 40));                                                           \
CreateAlternativeUnit(Pebi##name, base, std::pow(2, 50));                                                           \
CreateAlternativeUnit(Exbi##name, base, std::pow(2, 60));                                                           \
CreateAlternativeUnit(Zebi##name, base, std::pow(2, 70));                                                           \
CreateAlternativeUnit(Yobi##name, base, std::pow(2, 80))

CreateUnit(Scalar, 0);
CreateUnit(Mole, 1);
CreateUnit(Ampere, 0, 1);
CreateUnit(Metre, 0, 0, 1);
CreateUnit(Candela, 0, 0, 0, 1);
CreateUnit(Kelvin, 0, 0, 0, 0, 0, 1);
CreateUnit(Second, 0, 0, 0, 0, 0, 0, 1);
CreateUnit(Hertz, 0, 0, 0, 0, 0, 0, -1);
CreateUnit(Radian, 0);
CreateUnit(Steradian, 0);
CreateUnit(Newton, 0, 0, 1, 0, 1, 0, -2);
CreateUnit(Pascal, 0, 0, -1, 0, 1, 0, -2);
CreateUnit(Joule, 0, 0, 2, 0, 1, 0, -2);
CreateUnit(Watt, 0, 0, 2, 0, 1, 0, -3);
CreateUnit(Coulomb, 0, 1, 0, 0, 0, 0, 1);
CreateUnit(Volt, 0, -1, 2, 0, 1, 0, -3);
CreateUnit(Farad, 0, 2, -2, 0, -1, 0, 4);
CreateUnit(Ohm, 0, -2, 2, 0, 1, 0, -3);
CreateUnit(Siemens, 0, 2, -2, 0, -1, 0, 3);
CreateUnit(Weber, 0, -1, 2, 0, 1, 0, -2);
CreateUnit(Tesla, 0, -1, 0, 0, 1, 0, -2);
CreateUnit(Henry, 0, -2, 2, 0, 1, 0, -2);
CreateUnit(Lumen, 0, 0, 0, 1);
CreateUnit(Lux, 0, 0, -2, 1);
CreateUnit(Becquerel, 0, 0, 0, 0, 0, 0, -1);
CreateUnit(Gray, 0, 0, 2, 0, 0, 0, -2);
CreateUnit(Sievert, 0, 0, 2, 0, 0, 0, -2);
CreateUnit(Katal, 1, 0, 0, 0, 0, 0, -1);

CreateUnit(ReciprocalWeber, 0, 1, -2, 0, -1, 0, 2);
CreateUnit(ReciprocalMetre, 0, 0, -1);
CreateUnit(ReciprocalMole, -1);
CreateUnit(ReciprocalSquareMetre, 0, 0, -2);
CreateUnit(ReciprocalPascal, 0, 0, 1, 0, -1, 0, 2);
CreateUnit(ReciprocalHenry, 0, 1, 0, 0, -1, 0, 2);
CreateUnit(ReciprocalKelvin, 0, 0, 0, 0, 0, -1);
CreateUnit(WattSquareMetre, 0, 0, 4, 0, 1, 0, -3);
CreateUnit(WattPerSquareMetreKelvinToTheFourth, 0, 0, 0, 0, 1, -4, -3);
CreateUnit(HertzPerWatt, 0, 0, -2, 0, -1, 0, 2);
CreateUnit(JouleSecondPerMole, -1, 0, 2, 0, 1, 0, -1);
CreateUnit(CoulombPerMole, -1, 1, 0, 0, 0, 0, 1);
CreateUnit(MetrePerSecond, 0, 0, 1, 0, 0, 0, -1);
CreateUnit(MetrePerSecondSquared, 0, 0, 1, 0, 0, 0, -2);
CreateUnit(MetrePerSecondCubed, 0, 0, 1, 0, 0, 0, -3);
CreateUnit(MetrePerSecondToTheFourth, 0, 0, 1, 0, 0, 0, -4);
CreateUnit(RadianPerSecond, 0, 0, 0, 0, 0, 0, -1);
CreateUnit(RadianPerSecondSquared, 0, 0, 0, 0, 0, 0, -2);
CreateUnit(HertzPerSecond, 0, 0, 0, 0, 0, 0, -2);
CreateUnit(CubicMetrePerSecond, 0, 0, 3, 0, 0, 0, -1);
CreateUnit(SquareMetre, 0, 0, 2);
CreateUnit(CubicMetre, 0, 0, 3);
CreateUnit(NewtonSecond, 0, 0, 1, 0, 1, 0, -1);
CreateUnit(NewtonMetreSecond, 0, 0, 2, 0, 1, 0, -1);
CreateUnit(NewtonMetre, 0, 0, 2, 0, 1, 0, -2);
CreateUnit(NewtonPerSecond, 0, 0, 1, 0, 1, 0, -3);
CreateUnit(KilogramPerSquareMetre, 0, 0, -2, 0, 1);
CreateUnit(KilogramPerCubicMetre, 0, 0, -3, 0, 1);
CreateUnit(CubicMetrePerKilogram, 0, 0, 3, 0, -1);
CreateUnit(JouleSecond, 0, 0, 2, 0, 1, 0, -1);
CreateUnit(JoulePerKilogram, 0, 0, 2, 0, 0, 0, -2);
CreateUnit(JoulePerCubicMetre, 0, 0, -1, 0, 1, 0, -2);
CreateUnit(NewtonPerMetre, 0, 0, 0, 0, 1, 0, -2);
CreateUnit(WattPerSquareMetre, 0, 0, 0, 0, 1, 0, -3);
CreateUnit(SquareMetrePerSecond, 0, 0, 2, 0, 0, 0, -1);
CreateUnit(PascalSecond, 0, 0, -1, 0, 1, 0, -1);
CreateUnit(KilogramPerMetre, 0, 0, -1, 0, 1);
CreateUnit(KilogramPerSecond, 0, 0, 0, 0, 1, 0, -1);
CreateUnit(WattPerSteradianSquareMetre, 0, 0, 0, 0, 1, 0, -3);
CreateUnit(WattPerSteradianCubicMetre, 0, 0, -1, 0, 1, 0, -3);
CreateUnit(WattPerMetre, 0, 0, 1, 0, 1, 0, -3);
CreateUnit(GrayPerSecond, 0, 0, 2, 0, 0, 0, -3);
CreateUnit(WattPerCubicMetre, 0, 0, -1, 0, 1, 0, -3);
CreateUnit(JoulePerSquareMetreSecond, 0, 0, 0, 0, 1, 0, -3);
CreateUnit(JoulePerSquareMetre, 0, 0, 0, 0, 1, 0, -2);
CreateUnit(KilogramSquareMetre, 0, 0, 2, 0, 1);
CreateUnit(NewtonMetreSecondPerKilogram, 0, 0, 2, 0, 0, 0, -1);
CreateUnit(WattPerSteradian, 0, 0, 2, 0, 1, 0, -3);
CreateUnit(WattPerSteradianMetre, 0, 0, 1, 0, 1, 0, -3);
CreateUnit(MolePerCubicMetre, 1, 0, -3);
CreateUnit(CubicMetrePerMole, -1, 0, 3);
CreateUnit(JoulePerKelvinMole, -1, 0, 2, 0, 1, -1, -2);
CreateUnit(JoulePerMole, -1, 0, 2, 0, 1, 0, -2);
CreateUnit(SiemensSquareMetrePerMole, -1, 2, 0, 0, -1, 0, 3);
CreateUnit(MolePerKilogram, 1, 0, 0, 0, -1);
CreateUnit(KilogramPerMole, -1, 0, 0, 0, 1);
CreateUnit(CubicMetrePerMoleSecond, -1, 0, 3, 0, 0, 0, -1);
CreateUnit(CoulombPerSquareMetre, 0, 1, -2, 0, 0, 0, 1);
CreateUnit(CoulombPerCubicMetre, 0, 1, -3, 0, 0, 0, 1);
CreateUnit(AmperePerSquareMetre, 0, 1, -2);
CreateUnit(SiemensPerMetre, 0, 2, -3, 0, -1, 0, 3);
CreateUnit(FaradPerMetre, 0, 2, -3, 0, -1, 0, 4);
CreateUnit(HenryPerMetre, 0, -2, 1, 0, 1, 0, -2);
CreateUnit(VoltPerMetre, 0, -1, 1, 0, 1, 0, -3);
CreateUnit(AmperePerMetre, 0, 1, -1);
CreateUnit(CoulombPerKilogram, 0, 1, 0, 0, -1, 0, 1);
CreateUnit(OhmMetre, 0, -2, 3, 0, 1, 0, -3);
CreateUnit(CoulombPerMetre, 0, 1, -1, 0, 0, 0, 1);
CreateUnit(JoulePerTesla, 0, 1, 2);
CreateUnit(SquareMetrePerVoltSecond, 0, 1, 0, 0, -1, 0, 2);
CreateUnit(WeberPerMetre, 0, -1, 1, 0, 1, 0, -2);
CreateUnit(WeberMetre, 0, -1, 3, 0, 1, 0, -2);
CreateUnit(TeslaMetre, 0, -1, 1, 0, 1, 0, -2);
CreateUnit(AmpereRadian, 0, 1);
CreateUnit(MeterPerHenry, 0, 2, -1, 0, -1, 0, 2);
CreateUnit(LumenSecond, 0, 0, 0, 1, 0, 0, 1);
CreateUnit(LuxSecond, 0, 0, -2, 1, 0, 0, 1);
CreateUnit(CandelaPerSquareMetre, 0, 0, -2, 1);
CreateUnit(LumenPerWatt, 0, 0, -2, 1, -1, 0, 3);
CreateUnit(JoulePerKelvin, 0, 0, 2, 0, 1, -1, -2);
CreateUnit(JoulePerKilogramKelvin, 0, 0, 2, 0, 0, -1, -2);
CreateUnit(WattPerMetreKelvin, 0, 0, 1, 0, 1, -1, -3);
CreateUnit(KelvinPerWatt, 0, 0, -2, 0, -1, 1, 3);
CreateUnit(KelvinPerMetre, 0, 0, -1, 0, 0, 1);
CreateUnit(NewtonSquareMetrePerSquareKilogram, 0, 0, 3, 0, -1, 0, -2);
CreateUnit(NewtonSquareMetrePerSquareCoulomb, 0, -2, 3, 0, 1, 0, -4);
CreateUnit(KelvinMetre, 0, 0, 1, 0, 0, 1);
CreateUnit(HertzPerKelvin, 0, 0, 0, 0, 0, -1, -1);

CreateUnitPrefix(scalar, Scalar);
CreateUnitPrefix(mole, Mole);
CreateUnitPrefix(ampere, Ampere);
CreateUnitPrefix(metre, Metre);
CreateUnitPrefix(candela, Candela);
CreateUnitPrefix(gram, Gram);
CreateUnitPrefix(kelvin, Kelvin);
CreateUnitPrefix(second, Second);
CreateUnitPrefix(hertz, Hertz);
CreateUnitPrefix(radian, Radian);
CreateUnitPrefix(steradian, Steradian);
CreateUnitPrefix(newton, Newton);
CreateUnitPrefix(pascal, Pascal);
CreateUnitPrefix(joule, Joule);
CreateUnitPrefix(watt, Watt);
CreateUnitPrefix(coulomb, Coulomb);
CreateUnitPrefix(volt, Volt);
CreateUnitPrefix(farad, Farad);
CreateUnitPrefix(ohm, Ohm);
CreateUnitPrefix(siemens, Siemens);
CreateUnitPrefix(weber, Weber);
CreateUnitPrefix(tesla, Tesla);
CreateUnitPrefix(henry, Henry);
CreateUnitPrefix(lumen, Lumen);
CreateUnitPrefix(lux, Lux);
CreateUnitPrefix(becquerel, Becquerel);
CreateUnitPrefix(gray, Gray);
CreateUnitPrefix(sievert, Sievert);
CreateUnitPrefix(katal, Katal);

CreateAlternativeUnit(AstronomicalUnit, Metre, 149597870700);
CreateAlternativeUnit(Minute, Second, 60);
CreateAlternativeUnit(Hour, Minute, 60);
CreateAlternativeUnit(Day, Hour, 24);
CreateAlternativeUnit(Degree, Radian, pi / 180);
CreateAlternativeUnit(Arcminute, Degree, 1 / 60);
CreateAlternativeUnit(Arcsecond, Arcminute, 1 / 60);
CreateAlternativeUnit(Hectare, SquareMetre, 10000);
CreateAlternativeUnit(Litre, CubicMetre, 0.001);
CreateAlternativeUnit(Tonne, Kilogram, 1000);
CreateAlternativeUnit(Dalton, Kilogram, 1.66053906660505050e-27);
CreateAlternativeUnit(Electronvolt, Joule, 1.602176634e-19);

#endif