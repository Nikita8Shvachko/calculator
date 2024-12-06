// func_sin.cpp
#include "../Calc/Calculator.h"
#include <cmath>

extern "C" void registerPlugin(Calculator &calculator) {
    calculator.registerUnaryOperation("sin", [](double a) { return std::sin(a); });
}

//g++ -shared -o plugins/sin.dylib plugins/sin.cpp
