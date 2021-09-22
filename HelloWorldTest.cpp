/*
 * Author: Dylan Turner
 * Description: A program that should mimic the desired output of the compiler
 */

#include <vector>
#include <string>
#include <memory>
#include <Variable.hpp>

#include <std.hpp>

using namespace nabd;

VariablePointer main(const VariablePointer &rawInput) {
    rawInput->toList(std::vector({ VariableType.String }));
}

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
