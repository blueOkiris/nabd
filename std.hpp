/*
 * Author: Dylan Turner
 * Description: Library used to provide functions to the language
 */

#pragma once

#include <iostream>
#include <Variable.hpp>

namespace nabd {
    // String -> String
    inline VariablePointer print(VariablePointer rawInput) {
        const auto input = rawInput->toString();
        std::cout << input->value;
        return input;
    }
    
    // Number -> String
    inline VariablePointer input(VariablePointer rawInput) {
        //const auto input = rawInput->toNumber();
        std::string str;
        std::cin >> str;
        return std::make_shared<StringVariable>(str);
    }
}
