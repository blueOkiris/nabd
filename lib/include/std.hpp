/*
 * Author: Dylan Turner
 * Description: Library used to provide functions to the language
 */

#pragma once

#include <iostream>
#include <Variable.hpp>

namespace nabd {
    inline VariablePointer print(const VariablePointer &msg) {
        std::cout << msg->toString()->value;
        return msg->toString();
    }
    
    inline VariablePointer input(const VariablePointer &dummy) {
        std::string str;
        std::cin >> str;
        return std::make_shared<StringVariable>(str);
    }
}
