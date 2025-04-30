#include "Variable.hpp"
#include "../Math/Constants.hpp"

namespace MathLib {
    Variable::Variable(void) {
        EmptyBenchmark
    }
    Variable::Variable(const Sequence<char>& name, const Sequence<char>& dataType, Node* value, bool constant) : name(CollectionToString(name)), dataType(CollectionToString(dataType)), value(value), constant(constant) {
        EmptyBenchmark
    }
    Variable::Variable(const Sequence<char>& name, const Sequence<char>& dataType, const Sequence<char>& value, bool constant) : name(CollectionToString(name)), dataType(CollectionToString(dataType)), value(new Node(Node::Type::Constant, value)), constant(constant) {
        EmptyBenchmark
    }
    bool Variable::operator==(const Variable& other) const {
        StartAndReturnFromBenchmark(name == other.name && dataType == other.dataType && constant == other.constant);
    }
    bool Variable::operator!=(const Variable& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
    Array<Variable> CreateDefaultVariables(void) {
        const Variable defaultVariables[] = {
            Variable("true"_M, "Bool"_M, '1'_M, true),
            Variable("false"_M, "Bool"_M, '0'_M, true),
            Variable("e"_M, "R"_M, ToString(e), true),
            Variable("pi"_M, "R"_M, ToString(pi), true),
            Variable("tau"_M, "R"_M, ToString(tau), true),
            Variable("logarithmicCapacityOfUnitDisk"_M, "R"_M, ToString(logarithmicCapacityOfUnitDisk), true),
            Variable("regularPaperfoldingSequence"_M, "R"_M, ToString(regularPaperfoldingSequence), true),
            Variable("dottieNumber"_M, "R"_M, ToString(dottieNumber), true),
            Variable("laplaceLimit"_M, "R"_M, ToString(laplaceLimit), true),
            Variable("goldenAngle"_M, "R"_M, ToString(goldenAngle), true),
            Variable("magicAngle"_M, "R"_M, ToString(magicAngle), true),
            Variable("silverRatio"_M, "R"_M, ToString(silverRatio), true),
            Variable("goldenRatio"_M, "R"_M, ToString(goldenRatio), true),
            Variable("superGoldenRatio"_M, "R"_M, ToString(superGoldenRatio), true),
            Variable("plasticRatio"_M, "R"_M, ToString(plasticRatio), true),
            Variable("connectiveConstant"_M, "R"_M, ToString(connectiveConstant), true),
            Variable("keplerBouwkampConstant"_M, "R"_M, ToString(keplerBouwkampConstant), true),
            Variable("wallisConstant"_M, "R"_M, ToString(wallisConstant), true),
            Variable("gaussConstant"_M, "R"_M, ToString(gaussConstant), true),
            Variable("lemniscateConstant"_M, "R"_M, ToString(lemniscateConstant), true),
            Variable("eulerConstant"_M, "R"_M, ToString(eulerConstant), true),
            Variable("erdosBorweinConstant"_M, "R"_M, ToString(erdosBorweinConstant), true),
            Variable("omegaConstant"_M, "R"_M, ToString(omegaConstant), true),
            Variable("aperyConstant"_M, "R"_M, ToString(aperyConstant), true),
            Variable("ramanujanSoldnerConstant"_M, "R"_M, ToString(ramanujanSoldnerConstant), true),
            Variable("secondHermiteConstant"_M, "R"_M, ToString(secondHermiteConstant), true),
            Variable("liouvilleConstant"_M, "R"_M, ToString(liouvilleConstant), true),
            Variable("firstContinuedFractionConstant"_M, "R"_M, ToString(firstContinuedFractionConstant), true),
            Variable("ramanujanConstant"_M, "R"_M, ToString(ramanujanConstant), true),
            Variable("glaisherKinkelinConstant"_M, "R"_M, ToString(glaisherKinkelinConstant), true),
            Variable("catalanConstant"_M, "R"_M, ToString(catalanConstant), true),
            Variable("meisselMertensConstant"_M, "R"_M, ToString(meisselMertensConstant), true),
            Variable("universalParabolicConstant"_M, "R"_M, ToString(universalParabolicConstant), true),
            Variable("cahenConstant"_M, "R"_M, ToString(cahenConstant), true),
            Variable("gelfondConstant"_M, "R"_M, ToString(gelfondConstant), true),
            Variable("secondFavardConstant"_M, "R"_M, ToString(secondFavardConstant), true),
            Variable("gelfondSchneiderConstant"_M, "R"_M, ToString(gelfondSchneiderConstant), true),
            Variable("sierpinskiConstant"_M, "R"_M, ToString(sierpinskiConstant), true),
            Variable("landauRamanujanConstant"_M, "R"_M, ToString(landauRamanujanConstant), true),
            Variable("firstNielsenRamanujanConstant"_M, "R"_M, ToString(firstNielsenRamanujanConstant), true),
            Variable("giesekingConstant"_M, "R"_M, ToString(giesekingConstant), true),
            Variable("bernsteinConstant"_M, "R"_M, ToString(bernsteinConstant), true),
            Variable("tribonacciConstant"_M, "R"_M, ToString(tribonacciConstant), true),
            Variable("brunConstant"_M, "R"_M, ToString(brunConstant), true),
            Variable("twinPrimesConstant"_M, "R"_M, ToString(twinPrimesConstant), true),
            Variable("prouhetThueMorseConstant"_M, "R"_M, ToString(prouhetThueMorseConstant), true),
            Variable("golombDickmanConstant"_M, "R"_M, ToString(golombDickmanConstant), true),
            Variable("fellerTornierConstant"_M, "R"_M, ToString(fellerTornierConstant), true),
            Variable("base10ChampernowneConstant"_M, "R"_M, ToString(base10ChampernowneConstant), true),
            Variable("salemConstant"_M, "R"_M, ToString(salemConstant), true),
            Variable("khinchinConstant"_M, "R"_M, ToString(khinchinConstant), true),
            Variable("levyConstant1"_M, "R"_M, ToString(levyConstant1), true),
            Variable("levyConstant2"_M, "R"_M, ToString(levyConstant2), true),
            Variable("copelandErdosConstant"_M, "R"_M, ToString(copelandErdosConstant), true),
            Variable("millConstant"_M, "R"_M, ToString(millConstant), true),
            Variable("gompertzConstant"_M, "R"_M, ToString(gompertzConstant), true),
            Variable("vanDerPauwConstant"_M, "R"_M, ToString(vanDerPauwConstant), true),
            Variable("artinConstant"_M, "R"_M, ToString(artinConstant), true),
            Variable("porterConstant"_M, "R"_M, ToString(porterConstant), true),
            Variable("lochsConstant"_M, "R"_M, ToString(lochsConstant), true),
            Variable("deVicciTesseractConstant"_M, "R"_M, ToString(deVicciTesseractConstant), true),
            Variable("liebSquareIceConstant"_M, "R"_M, ToString(liebSquareIceConstant), true),
            Variable("nivenConstant"_M, "R"_M, ToString(nivenConstant), true),
            Variable("stephensConstant"_M, "R"_M, ToString(stephensConstant), true),
            Variable("reciprocalFibonacciConstant"_M, "R"_M, ToString(reciprocalFibonacciConstant), true),
            Variable("feigenbaumConstant1"_M, "R"_M, ToString(feigenbaumConstant1), true),
            Variable("chaitinConstant"_M, "R"_M, ToString(chaitinConstant), true),
            Variable("robbinsConstant"_M, "R"_M, ToString(robbinsConstant), true),
            Variable("weierstrassConstant"_M, "R"_M, ToString(weierstrassConstant), true),
            Variable("fransenRobinsonConstant"_M, "R"_M, ToString(fransenRobinsonConstant), true),
            Variable("feigenbaumConstant2"_M, "R"_M, ToString(feigenbaumConstant2), true),
            Variable("secondDuBoisReymondConstant"_M, "R"_M, ToString(secondDuBoisReymondConstant), true),
            Variable("erdosTenenbaumFordConstant"_M, "R"_M, ToString(erdosTenenbaumFordConstant), true),
            Variable("conwayConstant"_M, "R"_M, ToString(conwayConstant), true),
            Variable("hafnerSarnakMcCurleyConstant"_M, "R"_M, ToString(hafnerSarnakMcCurleyConstant), true),
            Variable("backhouseConstant"_M, "R"_M, ToString(backhouseConstant), true),
            Variable("viswanathConstant"_M, "R"_M, ToString(viswanathConstant), true),
            Variable("komornikLoretiConstant"_M, "R"_M, ToString(komornikLoretiConstant), true),
            Variable("embreeTrefethenConstant"_M, "R"_M, ToString(embreeTrefethenConstant), true),
            Variable("heathBrownMorozConstant"_M, "R"_M, ToString(heathBrownMorozConstant), true),
            Variable("mrbConstant"_M, "R"_M, ToString(mrbConstant), true),
            Variable("primeConstant"_M, "R"_M, ToString(primeConstant), true),
            Variable("somosQuadraticRecurrenceConstant"_M, "R"_M, ToString(somosQuadraticRecurrenceConstant), true),
            Variable("foiasConstant"_M, "R"_M, ToString(foiasConstant), true),
            Variable("taniguchiConstant"_M, "R"_M, ToString(taniguchiConstant), true),
        };
        StartAndReturnFromBenchmark(Array<Variable>(defaultVariables, SizeOfArray(defaultVariables)));
    }
}