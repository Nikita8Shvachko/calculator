    #include "Calculator.h"

    double add_f(double a, double b) { return a + b; }

    double sub_f(double a, double b) { return a - b; }

    double mul_f(double a, double b) { return a * b; }

    double div_f(double a, double b) {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    }


    // Binary operations map
    std::map<std::string, std::function<double(double, double)>> operations = {
            {"+", add_f},
            {"-", sub_f},
            {"*", mul_f},
            {"/", div_f}
    };

    // Unary operations map
    std::map<std::string, std::function<double(double)>> operationsUN = {

    };

    // Register a binary operation
    void Calculator::registerBinaryOperation(const std::string &name, std::function<double(double, double)> func) {
        operations[name] = func;
    }

    // Register a unary operation
    void Calculator::registerUnaryOperation(const std::string &name, std::function<double(double)> func) {
        operationsUN[name] = func;
    }

    // Load plugins
    void Calculator::loadPlugin(const std::string &pluginPath) {
        void *handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error("Failed to load plugin: " + std::string(dlerror()));
        }

        dlerror();

        // Define a function pointer type for the expected registration function in plugins
        using RegisterFunction = void (*)(Calculator &);

        // Attempt to load the registration function `registerPlugin`
        RegisterFunction registerFunc = reinterpret_cast<RegisterFunction>(dlsym(handle, "registerPlugin"));
        const char *error = dlerror();
        if (!registerFunc || error) {
            dlclose(handle);
            throw std::runtime_error("Failed to locate registerPlugin function in plugin: " + std::string(error ? error : "unknown error"));
        }

        // Call the plugin's registration function, passing in the Calculator instance
        try {
            registerFunc(*this);
            std::cout << "Successfully registered plugin from: " << pluginPath << std::endl;
        } catch (const std::exception &e) {
            dlclose(handle);
            throw std::runtime_error("Plugin registration failed: " + std::string(e.what()));
        }

        // Store the handle for later cleanup
        pluginHandles.push_back(handle);
    }

    // Primary method for parsing an expression
    double Calculator::parse(const std::string &expr) {
        std::string input = expr;
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        std::istringstream stream(input);
        return parseExpression(stream);
    }

    // Destructor to close all loaded plugins
    Calculator::~Calculator() {
        for (void *handle: pluginHandles) {
            dlclose(handle);
        }
    }

    // Parse expression for addition and subtraction
    double Calculator::parseExpression(std::istringstream &input) {
        double result = parseTerm(input);
        while (input) {
            char op;
            input >> op;
            if (op == '+' || op == '-') {
                double nextTerm = parseTerm(input);
                result = operations[std::string(1, op)](result, nextTerm);
            } else {
                input.putback(op);
                break;
            }
        }
        return result;
    }

    // Parse term for multiplication and division
    double Calculator::parseTerm(std::istringstream &input) {
        double result = parseFactor(input);
        while (input) {
            char op;
            input >> op;
            if (op == '*' || op == '/') {
                double nextFactor = parseFactor(input);
                result = operations[std::string(1, op)](result, nextFactor);
            } else {
                input.putback(op);
                break;
            }
        }
        return result;
    }

    // Parse factor: number, expression in parentheses, or unary function
    double Calculator::parseFactor(std::istringstream &input) {
        double value;
        char c = input.peek();

        if (c == '(') {
            // Parse expression within parentheses
            input.get(); //  '('
            value = parseExpression(input);
            input.get(); //  ')'
        } else if (std::isalpha(c)) {
            // Parse function name (e.g., sin, cos, exp)
            std::string funcName;

            // Read the function name
            while (std::isalpha(input.peek())) {
                funcName += input.get();
            }

            // Check if its a known single argument function
            if (operationsUN.find(funcName) != operationsUN.end()) {
                // Expect '(' after function name
                if (input.get() != '(') {
                     throw std::runtime_error("Expected '(' after function name: " + funcName);
                }

                // Parse the argument of the function
                value = parseExpression(input);

                // Expect ')' after function argument
                if (input.get() != ')') {
                    throw std::runtime_error("Expected ')' after function argument in: " + funcName);
                }

                // Apply the function
                value = operationsUN[funcName](value);
            } else {
                throw std::runtime_error("Unknown function: " + funcName);
            }
        } else {
            // Parse a number
            input >> value;
        }
        return value;
    }


