/*
 * Author: Dylan Turner
 * Description:
 *  - A program that should mimic the desired output for the truth machine
 *  - $std$
 *    i1InfI0Stop = loop >
 *        loop ? i1InfI0Stop(print(0d1#)) : print(0x0#).
 *    main = args >
 *        i1InfI0Stop(
 *            parseNum(input(0d0#))).
 */

#include <vector>
#include <string>
#include <memory>
#include <Variable.hpp>

#include <std.hpp> // $std$

VariablePointer i1InfI0Stop(const VariablePointer &loop); // i1InfI0Stop = loop > ...
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

// i1InfI0Stop = loop > loop ? i1InfI0Stop(print(0d1#)) : print(0x0#).
VariablePointer i1InfI0Stop(const VariablePointer &loop) {
    return std::dynamic_pointer_cast<NumberVariable>(
        loop->toNumber()
    )->value > 0 ?
        i1InfI0Stop(print(std::make_shared<NumberVariable>(1))) :
        print(std::make_shared<NumberVariable>(0));
}

// main = args > i1InfI0Stop(parseNuminput(0d0#))).
VariablePointer fake_main(const VariablePointer &args) {
    return i1InfI0Stop(parseNum(input(std::make_shared<NumberVariable>(0))));
}
