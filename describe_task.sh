#!/bin/sh

ME=`basename $0`

ALPHA='1'
F="0"
I="2 * cos(x) + sin(x)"
BL="10 * sin(50*t)"
BR="10 * cos(50*t)"

OUTPUT_FUNCTIONS="""
/*
 * This file is generated and will be replaced with a new one at every each compile.
 * If you wanna change these functions, look into $ME
 */
#include <math.h>
extern const double alpha = $ALPHA;

extern double f(double x, double t) {
    return $F;
}

extern double initial(double x) {
    return $I;
}

extern double boundaryLeft(double t) {
    return $BL;
}

extern double boundaryRight(double t) {
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