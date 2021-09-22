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
        const auto input = input->toString();
        std::cout << input->value;
    }
}
