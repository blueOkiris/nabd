/*
 * Author: Dylan Turner
 * Description: Library used to provide functions to the language
 */

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <Variable.hpp>

inline VariablePointer print(const VariablePointer &msg) {
    std::cout <<
        std::dynamic_pointer_cast<StringVariable>(msg->toString())->value;
    return msg->toString();
}

inline VariablePointer input(const VariablePointer &dummy) {
    std::string str;
    std::cin >> str;
    return std::make_shared<StringVariable>(str);
}

inline VariablePointer parseNum(const VariablePointer &str) {
    const auto strData = std::dynamic_pointer_cast<StringVariable>(
        str
    )->value;
    char *end;
    const auto num = std::strtod(strData.c_str(), &end);
    return std::make_shared<NumberVariable>(num);
}
