#include "math.h"

double fabs(double x) {
    return x < 0 ? -x : x;
}
float fabsf(float x) {
    return x < 0 ? -x : x;
}
long double fabsl(long double x) {
    return x < 0 ? -x : x;
}