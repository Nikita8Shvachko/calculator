#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <map>
#include <sstream>
#include <stdexcept>
#include <dlfcn.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <functional>
// Functions for basic  operations
double add_f(double a, double b);

double sub_f(double a, double b);

double mul_f(double a, double b);

double div_f(double a, double b);


class Calculator {
public:
    // Register a binary operation
    void registerBinaryOperation(const std::string &name, std::function<double(double, double)> func);

    // Register a unary operation
    void registerUnaryOperation(const std::string &name, std::function<double(double)> func);

    // Load plugins
    void loadPlugin(const std::string &pluginPath);


    // Primary method for parsing an expression
    double parse(const std::string &expr);

    // Destructor to close all loaded plugins
    ~Calculator();

private:
    // Storage for plugin handles to manage their lifecycle
    std::vector<void *> pluginHandles;

    // Parse expression for addition and subtraction
    double parseExpression(std::istringstream &input);

    // Parse term for multiplication and division
    double parseTerm(std::istringstream &input);

    // Parse factor: number, expression in parentheses, or unary function
    double parseFactor(std::istringstream &input);
};

#endif //CALCULATOR_CALCULATOR_H
