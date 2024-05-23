#ifndef Constants_H
#define Constants_H
#include "Host.hpp"
#include "Log.hpp"

#ifdef M_PIl
static const num_t pi = M_PIl;
static const num_t e = M_El;
#else
#ifdef M_PI
static const num_t pi = M_PI;
static const num_t e = M_E;
#else
static const num_t pi = 3.14;
static const num_t e = 2.71;
#endif
#endif

static const num_t tau = 2 * pi;
static const num_t logarithmicCapacityOfUnitDisk = 0.59017029950804811302;
static const num_t regularPaperfoldingSequence = 0.85073618820186726036;
static const num_t dottieNumber = 0.73908513321516064165;
static const num_t laplaceLimit = 0.66274341934918158097;
static const num_t goldenAngle = pi * (3 - Sqrt(5));
static const num_t magicAngle = std::atan(Sqrt(2));
static const num_t silverRatio = 1 + Sqrt(2);
static const num_t goldenRatio = (1 + Sqrt(5)) / 2;
static const num_t superGoldenRatio = (1 + std::cbrt((29 + 3 * Sqrt(93)) / 2) + std::cbrt((29 - 3 * Sqrt(93)) / 2)) / 3;
static const num_t plasticRatio = std::cbrt(1 / 2 + Sqrt(69) / 18) + std::cbrt(1 / 2 - Sqrt(69) / 18);
static const num_t connectiveConstant = Sqrt(2 + Sqrt(2));
static const num_t keplerBouwkampConstant = 0.11494204485329620070;
static const num_t wallisConstant = std::cbrt((45 - Sqrt(1929)) / 18) + std::cbrt((45 + Sqrt(1929)) / 18);
static const num_t gaussConstant = 0.83462684167407318628;
static const num_t lemniscateConstant = pi * gaussConstant;
static const num_t eulerConstant = 0.57721566490153286060;
static const num_t erdosBorweinConstant = 1.60669515241529176378;
static const num_t omegaConstant = 0.56714329040978387299;
static const num_t aperyConstant = 1.20205690315959428539;
static const num_t ramanujanSoldnerConstant = 1.45136923488338105028;
static const num_t secondHermiteConstant = 2 / Sqrt(3);
static const num_t liouvilleConstant = 0.110001000000000000000001;
static const num_t firstContinuedFractionConstant = 0.69777465796400798201;
static const num_t ramanujanConstant = std::exp(pi * Sqrt(163));
static const num_t glaisherKinkelinConstant = 1.28242712910062263687;
static const num_t catalanConstant = 0.91596559417721901505;
static const num_t meisselMertensConstant = 0.26149721284764278375;
static const num_t universalParabolicConstant = std::asinh(1) + Sqrt(2);
static const num_t cahenConstant = 0.64341054628833802618;
static const num_t gelfondConstant = std::exp(pi);
static const num_t secondFavardConstant = Pow(pi, 2) / 8;
static const num_t gelfondSchneiderConstant = Pow(2, Sqrt(2));
static const num_t sierpinskiConstant = 2.58498175957925321706;
static const num_t landauRamanujanConstant = 0.76422365358922066299;
static const num_t firstNielsenRamanujanConstant = Pow(pi, 2) / 12;
static const num_t giesekingConstant = 1.01494160640965362502;
static const num_t bernsteinConstant = 0.28016949902386913303;
static const num_t tribonacciConstant = (1 + 4 * std::cosh((1 / 3) * std::acosh(2 + 3 / 8))) / 3;
static const num_t brunConstant = 1.902160583104;
static const num_t twinPrimesConstant = 0.66016181584686957392;
static const num_t prouhetThueMorseConstant = 0.41245403364010759778;
static const num_t golombDickmanConstant = 0.62432998854355087099;
static const num_t fellerTornierConstant = 0.66131704946962233528;
static const num_t base10ChampernowneConstant = 0.12345678910111213141;
static const num_t salemConstant = 1.17628081825991750654;
static const num_t khinchinConstant = 2.68545200106530644530;
static const num_t levyConstant1 = Pow(pi, 2) / (12 * NaturalLog(2));
static const num_t levyConstant2 = std::exp(levyConstant1);
static const num_t copelandErdosConstant = 0.23571113171923293137;
static const num_t millConstant = 1.30637788386308069046;
static const num_t gompertzConstant = 0.59634736232319407434;
static const num_t vanDerPauwConstant = pi / NaturalLog(2);
static const num_t artinConstant = 0.37395581361920228805;
static const num_t porterConstant = 1.46707807943397547289;
static const num_t lochsConstant = (6 * NaturalLog(2) * NaturalLog(10)) / Pow(pi, 2);
static const num_t deVicciTesseractConstant = 1.00743475688427937609;
static const num_t liebSquareIceConstant = 8 / (3 * Sqrt(3));
static const num_t nivenConstant = 1.70521114010536776428;
static const num_t stephensConstant = 0.57595996889294543964;
static const num_t reciprocalFibonacciConstant = 3.35988566624317755317;
static const num_t feigenbaumConstant1 = 4.66920160910299067185;
static const num_t chaitinConstant = 0.0078749969978123844;
static const num_t robbinsConstant = (4 + 17 * Sqrt(2) - 6 * Sqrt(3) - 7 * pi) / 105 + NaturalLog(1 + Sqrt(2)) / 5 + (2 * NaturalLog(2 + Sqrt(3))) / 5;
static const num_t weierstrassConstant = 0.47494937998792065033;
static const num_t fransenRobinsonConstant = 2.80777024202851936522;
static const num_t feigenbaumConstant2 = 2.50290787509589282228;
static const num_t secondDuBoisReymondConstant = (std::exp(2) - 7) / 2;
static const num_t erdosTenenbaumFordConstant = 1 - ((1 + Log<num_t>(10, Log<num_t>(10, 2))) / Log<num_t>(10, 2));
static const num_t conwayConstant = 1.30357726903429639125;
static const num_t hafnerSarnakMcCurleyConstant = 0.35323637185499598454;
static const num_t backhouseConstant = 1.45607494858268967139;
static const num_t viswanathConstant = 1.1319882487943;
static const num_t komornikLoretiConstant = 1.78723165018296593301;
static const num_t embreeTrefethenConstant = 0.70258;
static const num_t heathBrownMorozConstant = 0.00131764115485317810;
static const num_t mrbConstant = 0.18785964246206712024;
static const num_t primeConstant = 0.41468250985111166024;
static const num_t somosQuadraticRecurrenceConstant = 1.66168794963359412129;
static const num_t foiasConstant = 1.18745235112650105459;
static const num_t taniguchiConstant = 0.67823449191739197803;

#endif