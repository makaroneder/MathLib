#ifndef Variable_H
#define Variable_H
#include "../Constants.hpp"
#include "Node.hpp"

/// @brief Contains the name, value and type of the variable
struct Variable {
    /// @brief Name of the variable
    String name;
    /// @brief Value of the variable
    Node* value;

    /// @brief Creates empty variable
    Variable(void);
    /// @brief Creates a new variable
    /// @param n Name of the variable
    /// @param val Value of the variable
    Variable(String n, Node* val);
    /// @brief Creates a new variable
    /// @param n Name of the variable
    /// @param val Value of the variable
    Variable(String n, String val);
};
/// @brief Default variables for equation solver
static const Variable defaultVariables[] = {
    Variable("e", ToString(e)),
    Variable("pi", ToString(pi)),
    Variable("tau", ToString(tau)),
    Variable("logarithmicCapacityOfUnitDisk", ToString(logarithmicCapacityOfUnitDisk)),
    Variable("regularPaperfoldingSequence", ToString(regularPaperfoldingSequence)),
    Variable("dottieNumber", ToString(dottieNumber)),
    Variable("laplaceLimit", ToString(laplaceLimit)),
    Variable("goldenAngle", ToString(goldenAngle)),
    Variable("magicAngle", ToString(magicAngle)),
    Variable("silverRatio", ToString(silverRatio)),
    Variable("goldenRatio", ToString(goldenRatio)),
    Variable("superGoldenRatio", ToString(superGoldenRatio)),
    Variable("plasticRatio", ToString(plasticRatio)),
    Variable("connectiveConstant", ToString(connectiveConstant)),
    Variable("keplerBouwkampConstant", ToString(keplerBouwkampConstant)),
    Variable("wallisConstant", ToString(wallisConstant)),
    Variable("gaussConstant", ToString(gaussConstant)),
    Variable("lemniscateConstant", ToString(lemniscateConstant)),
    Variable("eulerConstant", ToString(eulerConstant)),
    Variable("erdosBorweinConstant", ToString(erdosBorweinConstant)),
    Variable("omegaConstant", ToString(omegaConstant)),
    Variable("aperyConstant", ToString(aperyConstant)),
    Variable("ramanujanSoldnerConstant", ToString(ramanujanSoldnerConstant)),
    Variable("secondHermiteConstant", ToString(secondHermiteConstant)),
    Variable("liouvilleConstant", ToString(liouvilleConstant)),
    Variable("firstContinuedFractionConstant", ToString(firstContinuedFractionConstant)),
    Variable("ramanujanConstant", ToString(ramanujanConstant)),
    Variable("glaisherKinkelinConstant", ToString(glaisherKinkelinConstant)),
    Variable("catalanConstant", ToString(catalanConstant)),
    Variable("meisselMertensConstant", ToString(meisselMertensConstant)),
    Variable("universalParabolicConstant", ToString(universalParabolicConstant)),
    Variable("cahenConstant", ToString(cahenConstant)),
    Variable("gelfondConstant", ToString(gelfondConstant)),
    Variable("secondFavardConstant", ToString(secondFavardConstant)),
    Variable("gelfondSchneiderConstant", ToString(gelfondSchneiderConstant)),
    Variable("sierpinskiConstant", ToString(sierpinskiConstant)),
    Variable("landauRamanujanConstant", ToString(landauRamanujanConstant)),
    Variable("firstNielsenRamanujanConstant", ToString(firstNielsenRamanujanConstant)),
    Variable("giesekingConstant", ToString(giesekingConstant)),
    Variable("bernsteinConstant", ToString(bernsteinConstant)),
    Variable("tribonacciConstant", ToString(tribonacciConstant)),
    Variable("brunConstant", ToString(brunConstant)),
    Variable("twinPrimesConstant", ToString(twinPrimesConstant)),
    Variable("prouhetThueMorseConstant", ToString(prouhetThueMorseConstant)),
    Variable("golombDickmanConstant", ToString(golombDickmanConstant)),
    Variable("fellerTornierConstant", ToString(fellerTornierConstant)),
    Variable("base10ChampernowneConstant", ToString(base10ChampernowneConstant)),
    Variable("salemConstant", ToString(salemConstant)),
    Variable("khinchinConstant", ToString(khinchinConstant)),
    Variable("levyConstant1", ToString(levyConstant1)),
    Variable("levyConstant2", ToString(levyConstant2)),
    Variable("copelandErdosConstant", ToString(copelandErdosConstant)),
    Variable("millConstant", ToString(millConstant)),
    Variable("gompertzConstant", ToString(gompertzConstant)),
    Variable("vanDerPauwConstant", ToString(vanDerPauwConstant)),
    Variable("artinConstant", ToString(artinConstant)),
    Variable("porterConstant", ToString(porterConstant)),
    Variable("lochsConstant", ToString(lochsConstant)),
    Variable("deVicciTesseractConstant", ToString(deVicciTesseractConstant)),
    Variable("liebSquareIceConstant", ToString(liebSquareIceConstant)),
    Variable("nivenConstant", ToString(nivenConstant)),
    Variable("stephensConstant", ToString(stephensConstant)),
    Variable("reciprocalFibonacciConstant", ToString(reciprocalFibonacciConstant)),
    Variable("feigenbaumConstant1", ToString(feigenbaumConstant1)),
    Variable("chaitinConstant", ToString(chaitinConstant)),
    Variable("robbinsConstant", ToString(robbinsConstant)),
    Variable("weierstrassConstant", ToString(weierstrassConstant)),
    Variable("fransenRobinsonConstant", ToString(fransenRobinsonConstant)),
    Variable("feigenbaumConstant2", ToString(feigenbaumConstant2)),
    Variable("secondDuBoisReymondConstant", ToString(secondDuBoisReymondConstant)),
    Variable("erdosTenenbaumFordConstant", ToString(erdosTenenbaumFordConstant)),
    Variable("conwayConstant", ToString(conwayConstant)),
    Variable("hafnerSarnakMcCurleyConstant", ToString(hafnerSarnakMcCurleyConstant)),
    Variable("backhouseConstant", ToString(backhouseConstant)),
    Variable("viswanathConstant", ToString(viswanathConstant)),
    Variable("komornikLoretiConstant", ToString(komornikLoretiConstant)),
    Variable("embreeTrefethenConstant", ToString(embreeTrefethenConstant)),
    Variable("heathBrownMorozConstant", ToString(heathBrownMorozConstant)),
    Variable("mrbConstant", ToString(mrbConstant)),
    Variable("primeConstant", ToString(primeConstant)),
    Variable("somosQuadraticRecurrenceConstant", ToString(somosQuadraticRecurrenceConstant)),
    Variable("foiasConstant", ToString(foiasConstant)),
    Variable("taniguchiConstant", ToString(taniguchiConstant)),
};

#endif
