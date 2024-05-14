#ifndef Constants_H
#define Constants_H
#include "Typedefs.hpp"
#include "Log.hpp"

#ifdef M_PIl
static constexpr num_t pi = M_PIl;
static constexpr num_t e = M_El;
#else
#ifdef M_PI
static constexpr num_t pi = M_PI;
static constexpr num_t e = M_E;
#else
static constexpr num_t pi = 3.14;
static constexpr num_t e = 2.71;
#endif
#endif

static constexpr num_t tau = 2 * pi;
static constexpr num_t logarithmicCapacityOfUnitDisk = 0.59017029950804811302;
static constexpr num_t regularPaperfoldingSequence = 0.85073618820186726036;
static constexpr num_t dottieNumber = 0.73908513321516064165;
static constexpr num_t laplaceLimit = 0.66274341934918158097;
static constexpr num_t goldenAngle = pi * (3 - std::sqrt(5));
static constexpr num_t magicAngle = std::atan(std::sqrt(2));
static constexpr num_t silverRatio = 1 + std::sqrt(2);
static constexpr num_t goldenRatio = (1 + std::sqrt(5)) / 2;
static constexpr num_t superGoldenRatio = (1 + std::cbrt((29 + 3 * std::sqrt(93)) / 2) + std::cbrt((29 - 3 * std::sqrt(93)) / 2)) / 3;
static constexpr num_t plasticRatio = std::cbrt(1 / 2 + std::sqrt(69) / 18) + std::cbrt(1 / 2 - std::sqrt(69) / 18);
static constexpr num_t connectiveConstant = std::sqrt(2 + std::sqrt(2));
static constexpr num_t keplerBouwkampConstant = 0.11494204485329620070;
static constexpr num_t wallisConstant = std::cbrt((45 - std::sqrt(1929)) / 18) + std::cbrt((45 + std::sqrt(1929)) / 18);
static constexpr num_t gaussConstant = 0.83462684167407318628;
static constexpr num_t lemniscateConstant = pi * gaussConstant;
static constexpr num_t eulerConstant = 0.57721566490153286060;
static constexpr num_t erdosBorweinConstant = 1.60669515241529176378;
static constexpr num_t omegaConstant = 0.56714329040978387299;
static constexpr num_t aperyConstant = 1.20205690315959428539;
static constexpr num_t ramanujanSoldnerConstant = 1.45136923488338105028;
static constexpr num_t secondHermiteConstant = 2 / std::sqrt(3);
static constexpr num_t liouvilleConstant = 0.110001000000000000000001;
static constexpr num_t firstContinuedFractionConstant = 0.69777465796400798201;
static constexpr num_t ramanujanConstant = std::pow(e, pi * std::sqrt(163));
static constexpr num_t glaisherKinkelinConstant = 1.28242712910062263687;
static constexpr num_t catalanConstant = 0.91596559417721901505;
static constexpr num_t meisselMertensConstant = 0.26149721284764278375;
static constexpr num_t universalParabolicConstant = std::asinh(1) + std::sqrt(2);
static constexpr num_t cahenConstant = 0.64341054628833802618;
static constexpr num_t gelfondConstant = std::pow(e, pi);
static constexpr num_t secondFavardConstant = std::pow(pi, 2) / 8;
static constexpr num_t gelfondSchneiderConstant = std::pow(2, std::sqrt(2));
static constexpr num_t sierpinskiConstant = 2.58498175957925321706;
static constexpr num_t landauRamanujanConstant = 0.76422365358922066299;
static constexpr num_t firstNielsenRamanujanConstant = std::pow(pi, 2) / 12;
static constexpr num_t giesekingConstant = 1.01494160640965362502;
static constexpr num_t bernsteinConstant = 0.28016949902386913303;
static constexpr num_t tribonacciConstant = (1 + 4 * std::cosh((1 / 3) * std::acosh(2 + 3 / 8))) / 3;
static constexpr num_t brunConstant = 1.902160583104;
static constexpr num_t twinPrimesConstant = 0.66016181584686957392;
static constexpr num_t prouhetThueMorseConstant = 0.41245403364010759778;
static constexpr num_t golombDickmanConstant = 0.62432998854355087099;
static constexpr num_t fellerTornierConstant = 0.66131704946962233528;
static constexpr num_t base10ChampernowneConstant = 0.12345678910111213141;
static constexpr num_t salemConstant = 1.17628081825991750654;
static constexpr num_t khinchinConstant = 2.68545200106530644530;
static constexpr num_t levyConstant1 = std::pow(pi, 2) / (12 * std::log(2));
static constexpr num_t levyConstant2 = std::pow(e, levyConstant1);
static constexpr num_t copelandErdosConstant = 0.23571113171923293137;
static constexpr num_t millConstant = 1.30637788386308069046;
static constexpr num_t gompertzConstant = 0.59634736232319407434;
static constexpr num_t vanDerPauwConstant = pi / std::log(2);
static constexpr num_t artinConstant = 0.37395581361920228805;
static constexpr num_t porterConstant = 1.46707807943397547289;
static constexpr num_t lochsConstant = (6 * std::log(2) * std::log(10)) / std::pow(pi, 2);
static constexpr num_t deVicciTesseractConstant = 1.00743475688427937609;
static constexpr num_t liebSquareIceConstant = 8 / (3 * std::sqrt(3));
static constexpr num_t nivenConstant = 1.70521114010536776428;
static constexpr num_t stephensConstant = 0.57595996889294543964;
static constexpr num_t reciprocalFibonacciConstant = 3.35988566624317755317;
static constexpr num_t feigenbaumConstant1 = 4.66920160910299067185;
static constexpr num_t chaitinConstant = 0.0078749969978123844;
static constexpr num_t robbinsConstant = (4 + 17 * std::sqrt(2) - 6 * std::sqrt(3) - 7 * pi) / 105 + std::log(1 + std::sqrt(2)) / 5 + (2 * std::log(2 + std::sqrt(3))) / 5;
static constexpr num_t weierstrassConstant = 0.47494937998792065033;
static constexpr num_t fransenRobinsonConstant = 2.80777024202851936522;
static constexpr num_t feigenbaumConstant2 = 2.50290787509589282228;
static constexpr num_t secondDuBoisReymondConstant = (std::exp(2) - 7) / 2;
static constexpr num_t erdosTenenbaumFordConstant = 1 - ((1 + Log<num_t>(10, Log<num_t>(10, 2))) / Log<num_t>(10, 2));
static constexpr num_t conwayConstant = 1.30357726903429639125;
static constexpr num_t hafnerSarnakMcCurleyConstant = 0.35323637185499598454;
static constexpr num_t backhouseConstant = 1.45607494858268967139;
static constexpr num_t viswanathConstant = 1.1319882487943;
static constexpr num_t komornikLoretiConstant = 1.78723165018296593301;
static constexpr num_t embreeTrefethenConstant = 0.70258;
static constexpr num_t heathBrownMorozConstant = 0.00131764115485317810;
static constexpr num_t mrbConstant = 0.18785964246206712024;
static constexpr num_t primeConstant = 0.41468250985111166024;
static constexpr num_t somosQuadraticRecurrenceConstant = 1.66168794963359412129;
static constexpr num_t foiasConstant = 1.18745235112650105459;
static constexpr num_t taniguchiConstant = 0.67823449191739197803;

#endif