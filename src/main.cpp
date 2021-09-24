/*
 * Author: Dylan Turner
 * Description: Entry point of nabd compiler
 */

#include <Utility.hpp>
#include <Parser.hpp>

using namespace nabd;

int main(const int argc, const char **args) {
    const auto code = readFile("examples/TruthMachine.nabd");
    
    // Note: this fails out, so no check for success
    const ProgramParser prgmParser;
    uint64_t index = 0;
    const auto progParserResult = prgmParser.parse(code, index, 1, 1);
    std::cout << progParserResult.result.str() << std::endl;

    return 0;
}
