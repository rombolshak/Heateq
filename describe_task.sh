#!/bin/sh

ME=`basename $0`

ALPHA='i/2'
#F="x*x/2 + 15 * x * sin(t)"
F="return 200*x;"
#F="return 0;"
# vars below not used in time-independent task
I="exp((-(x-0.5)*(x-0.5))/(2*0.2)) / (sqrt(0.2) * sqrt(2*3.1415926))"
BL="0"
BR="0"

OUTPUT_FUNCTIONS="""
/*
 * This file is generated and will be replaced with a new one at every each compile.
 * If you wanna change these functions, look into $ME
 */
#include <math.h>
#include <complex>

extern double f(double x, double t) {
    $F;
}

extern std::complex< double > initial(double x) {
    return $I;
}

extern std::complex< double > boundaryLeft(double t) {
    return $BL;
}

extern std::complex< double > boundaryRight(double t) {
    return $BR;
}

"""

OUTPUT_DEFINES="""
/*
* This file is generated and will be replaced with a new one at every each compile.
* If you wanna change these functions, look into $ME
*/
#include <string>
extern std::string alpha_description() {return \"$ALPHA\";}
extern std::string f_description() {return \"$F\";}
extern std::string initial_description() {return \"$I\";}
extern std::string left_description() {return \"$BL\";}
extern std::string right_description() {return \"$BR\";}
"""

echo "$OUTPUT_FUNCTIONS" > task_definition.cpp
echo "$OUTPUT_DEFINES" > task_defines.cpp
