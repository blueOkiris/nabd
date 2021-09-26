/*
 * Author: Dylan Turner
 * Description:
 *  - A program that should mimic the desired output for hello world
 *  - $std$main=args>print('Hello, world!\n').
 */

#include <vector>
#include <string>
#include <memory>
#include <Variable.hpp>

#include <std.hpp> // $std$

VariablePointer fake_main(const VariablePointer &args); // main = args > ...

int main(int argc, char **args) {
    std::vector<VariablePointer> argVars;
    for(int i = 1; i < argc; i++) {
        argVars.push_back(std::make_shared<StringVariable>(
            std::string(args[i])
        ));
    }
    const auto retVal = fake_main(std::make_shared<ListVariable>(argVars));
    return static_cast<int>(
        std::dynamic_pointer_cast<NumberVariable>(retVal->toNumber())->value
    );
}

// main = args > print('Hello, world!\n').
VariablePointer fake_main(const VariablePointer &args) {
    return print(std::make_shared<StringVariable>("Hello, world!\n"));
}
