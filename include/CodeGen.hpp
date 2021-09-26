/*
 * Author: Dylan Turner
 * Description:
 *  - Used for turning program tokens into C++ code
 *  - These assume properly formatted tokens though!
 *  - Only use in conjunction with parser!!!!
 */

#pragma once

#include <string>
#include <vector>
#include <Token.hpp>

namespace nabd {
    namespace codegen {
        std::string generateCppCode(
            const Token &program,
            const InputArguments &cliInputs,
            const ModuleInfo &modInfo
        );

        std::string generateIncludeCode(
            const Token &include,
            const InputArguments &cliInputs,
            const ModuleInfo &modInfo
        );

        std::string generateFuncDefCode(const Token &funcDef);
        std::string generateExprCode(const Token &expr);

        // This assumes a file is known to exist and is a .nabd file
        void generateHeaderFile(
            const std::string &moduleFile, const std::string &newFileNameBase,
            const InputArguments &cliInputs,
            const ModuleInfo &modInfo
        );
    }
};
