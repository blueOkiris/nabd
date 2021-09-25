/*
 * Author: Dylan Turner
 * Description: Entry point of nabd compiler
 */

#include <Utility.hpp>
#include <Parser.hpp>
#include <CodeGen.hpp>

using namespace nabd;

int main(const int argc, const char **args) {
    const auto code = readFile("examples/TruthMachine.nabd");
    
    // Note: this fails out, so no check for success
    const auto progParserResult = parser::parseProgram(code, 0, 1, 1);
    std::cout << progParserResult.result.str() << std::endl;

    const auto outputCode = codegen::generateCppCode(progParserResult.result);
    std::cout << outputCode << std::endl;

    return 0;
}
