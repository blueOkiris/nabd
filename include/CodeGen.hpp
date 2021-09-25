/*
 * Author: Dylan Turner
 * Description: Used for turning program tokens into C++ code
 */

#pragma once

#include <string>
#include <Token.hpp>

namespace nabd {
    namespace codegen {
        std::string generateCppCode(const Token &program);
    }
};
