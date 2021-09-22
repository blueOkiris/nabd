/*
 * Author: Dylan Turner
 * Description: A program that should mimic the desired output for hello world
 */

#include <vector>
#include <string>
#include <memory>
#include <Variable.hpp>

using namespace nabd;

#include <std.hpp> // $std$

VariablePointer main(const VariablePointer &rawInput); // main =

int main(int argc, char **args) {
    std::vector<VariablePointer> argVars;
    for(int i = 1; i < argc; i++) {
        argVars.push_back(std::make_shared<StringVariable>(
            std::string(args[i])
        ));
    }
    const auto retVal = main(std::make_shared<ListVariable>(argVars));
    return static_cast<int>(retVal->toNumber()->value);
}

// main = args :: [Str] > print('Hello, world!\n').
VariablePointer main(const VariablePointer &rawInput) {
    const auto args = rawInput->toList(std::vector({ VariableType.String }));
    return print(std::make_shared<StringVariable>("Hello, world!\n"));
}
