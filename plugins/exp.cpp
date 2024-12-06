// func_exp.cpp
#include <cmath>
extern "C" double exp(double x) {
    return std::exp(x);
}
//g++ -shared -o plugins/exp.dylib plugins/exp.cpp