#include "functions.h"
#include <math.h>

double func1(double x){
    return cos(20.0 * x) - fabs(x) * .5 + x * x * x * 0.25;
}