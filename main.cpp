    #include <iostream>
    #include "Calc/Calculator.h"



    int main() {
        Calculator calc;
        try {
            calc.loadPlugin("../plugins/ build /sin.dylib");

            std::string expression = "sin(3) ";
            double result = calc.parse(expression);
            std::cout << "Result: " << result << std::endl;

        } catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        return 0;
    }
