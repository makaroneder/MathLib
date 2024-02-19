#ifndef Variable_H
#define Variable_H
#include "../Constants.hpp"
#include <string>
#include <vector>

/// @brief Contains the name, value and type of the variable
struct Variable {
    /// @brief Name of the variable
    std::string name;
    /// @brief Value of the variable
    std::string value;

    /// @brief Creates a new variable
    /// @param n Name of the variable
    /// @param val Value of the variable
    Variable(std::string n, std::string val);
};
/// @brief Default variables for equation solver
static const Variable defaultVariables[] = {
    Variable("e", std::to_string(e)),
    Variable("pi", std::to_string(pi)),
    Variable("tau", std::to_string(tau)),
    Variable("logarithmicCapacityOfUnitDisk", std::to_string(logarithmicCapacityOfUnitDisk)),
    Variable("regularPaperfoldingSequence", std::to_string(regularPaperfoldingSequence)),
    Variable("dottieNumber", std::to_string(dottieNumber)),
    Variable("laplaceLimit", std::to_string(laplaceLimit)),
    Variable("goldenAngle", std::to_string(goldenAngle)),
    Variable("magicAngle", std::to_string(magicAngle)),
    Variable("silverRatio", std::to_string(silverRatio)),
    Variable("goldenRatio", std::to_string(goldenRatio)),
    Variable("superGoldenRatio", std::to_string(superGoldenRatio)),
    Variable("plasticRatio", std::to_string(plasticRatio)),
    Variable("connectiveConstant", std::to_string(connectiveConstant)),
    Variable("keplerBouwkampConstant", std::to_string(keplerBouwkampConstant)),
    Variable("wallisConstant", std::to_string(wallisConstant)),
    Variable("gaussConstant", std::to_string(gaussConstant)),
    Variable("lemniscateConstant", std::to_string(lemniscateConstant)),
    Variable("eulerConstant", std::to_string(eulerConstant)),
    Variable("erdosBorweinConstant", std::to_string(erdosBorweinConstant)),
    Variable("omegaConstant", std::to_string(omegaConstant)),
    Variable("aperyConstant", std::to_string(aperyConstant)),
    Variable("ramanujanSoldnerConstant", std::to_string(ramanujanSoldnerConstant)),
    Variable("secondHermiteConstant", std::to_string(secondHermiteConstant)),
    Variable("liouvilleConstant", std::to_string(liouvilleConstant)),
    Variable("firstContinuedFractionConstant", std::to_string(firstContinuedFractionConstant)),
    Variable("ramanujanConstant", std::to_string(ramanujanConstant)),
    Variable("glaisherKinkelinConstant", std::to_string(glaisherKinkelinConstant)),
    Variable("catalanConstant", std::to_string(catalanConstant)),
    Variable("meisselMertensConstant", std::to_string(meisselMertensConstant)),
    Variable("universalParabolicConstant", std::to_string(universalParabolicConstant)),
    Variable("cahenConstant", std::to_string(cahenConstant)),
    Variable("gelfondConstant", std::to_string(gelfondConstant)),
    Variable("secondFavardConstant", std::to_string(secondFavardConstant)),
    Variable("gelfondSchneiderConstant", std::to_string(gelfondSchneiderConstant)),
    Variable("sierpinskiConstant", std::to_string(sierpinskiConstant)),
    Variable("landauRamanujanConstant", std::to_string(landauRamanujanConstant)),
    Variable("firstNielsenRamanujanConstant", std::to_string(firstNielsenRamanujanConstant)),
    Variable("giesekingConstant", std::to_string(giesekingConstant)),
    Variable("bernsteinConstant", std::to_string(bernsteinConstant)),
    Variable("tribonacciConstant", std::to_string(tribonacciConstant)),
    Variable("brunConstant", std::to_string(brunConstant)),
    Variable("twinPrimesConstant", std::to_string(twinPrimesConstant)),
    Variable("prouhetThueMorseConstant", std::to_string(prouhetThueMorseConstant)),
    Variable("golombDickmanConstant", std::to_string(golombDickmanConstant)),
    Variable("fellerTornierConstant", std::to_string(fellerTornierConstant)),
    Variable("base10ChampernowneConstant", std::to_string(base10ChampernowneConstant)),
    Variable("salemConstant", std::to_string(salemConstant)),
    Variable("khinchinConstant", std::to_string(khinchinConstant)),
    Variable("levyConstant1", std::to_string(levyConstant1)),
    Variable("levyConstant2", std::to_string(levyConstant2)),
    Variable("copelandErdosConstant", std::to_string(copelandErdosConstant)),
    Variable("millConstant", std::to_string(millConstant)),
    Variable("gompertzConstant", std::to_string(gompertzConstant)),
    Variable("vanDerPauwConstant", std::to_string(vanDerPauwConstant)),
    Variable("artinConstant", std::to_string(artinConstant)),
    Variable("porterConstant", std::to_string(porterConstant)),
    Variable("lochsConstant", std::to_string(lochsConstant)),
    Variable("deVicciTesseractConstant", std::to_string(deVicciTesseractConstant)),
    Variable("liebSquareIceConstant", std::to_string(liebSquareIceConstant)),
    Variable("nivenConstant", std::to_string(nivenConstant)),
    Variable("stephensConstant", std::to_string(stephensConstant)),
    Variable("reciprocalFibonacciConstant", std::to_string(reciprocalFibonacciConstant)),
    Variable("feigenbaumConstant1", std::to_string(feigenbaumConstant1)),
    Variable("chaitinConstant", std::to_string(chaitinConstant)),
    Variable("robbinsConstant", std::to_string(robbinsConstant)),
    Variable("weierstrassConstant", std::to_string(weierstrassConstant)),
    Variable("fransenRobinsonConstant", std::to_string(fransenRobinsonConstant)),
    Variable("feigenbaumConstant2", std::to_string(feigenbaumConstant2)),
    Variable("secondDuBoisReymondConstant", std::to_string(secondDuBoisReymondConstant)),
    Variable("erdosTenenbaumFordConstant", std::to_string(erdosTenenbaumFordConstant)),
    Variable("conwayConstant", std::to_string(conwayConstant)),
    Variable("hafnerSarnakMcCurleyConstant", std::to_string(hafnerSarnakMcCurleyConstant)),
    Variable("backhouseConstant", std::to_string(backhouseConstant)),
    Variable("viswanathConstant", std::to_string(viswanathConstant)),
    Variable("komornikLoretiConstant", std::to_string(komornikLoretiConstant)),
    Variable("embreeTrefethenConstant", std::to_string(embreeTrefethenConstant)),
    Variable("heathBrownMorozConstant", std::to_string(heathBrownMorozConstant)),
    Variable("mrbConstant", std::to_string(mrbConstant)),
    Variable("primeConstant", std::to_string(primeConstant)),
    Variable("somosQuadraticRecurrenceConstant", std::to_string(somosQuadraticRecurrenceConstant)),
    Variable("foiasConstant", std::to_string(foiasConstant)),
    Variable("taniguchiConstant", std::to_string(taniguchiConstant)),
};

#endif
