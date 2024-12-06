    #include <cmath>
    #include "../Calc/Calculator.h"

    extern "C" void registerPlugin(Calculator &calc) {
        calc.registerUnaryOperation("cos", [](double a) { return std::cos(a); });
    }
