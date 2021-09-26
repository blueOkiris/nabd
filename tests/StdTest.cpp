/*
 * Author: Dylan Turner
 * Description: Tests of std.h library functions
 */

#include <iostream>
#include <memory>
#include <string>
#include <Variable.hpp>
#include <std.hpp>

void testPrint(void);
void testInput(void);
void testParseNum(void);
void testLen(void);

int main(const int argc, const char **args) {
    for(int i = 1; i < argc; i++) {
        if(std::string(args[i]) == "all") {
            testPrint();
            testInput();
            testParseNum();
        } else if(std::string(args[i]) == "print") {
            testPrint();
        } else if(std::string(args[i]) == "input") {
            testInput();
        } else if(std::string(args[i]) == "parseNum") {
            testParseNum();
        } else if(std::string(args[i]) == "len") {
            testLen();
        }
    }
}

void testLen(void) {
    std::cout << "Testing std.hpp:len." << std::endl;

    const auto strVar = std::make_shared<StringVariable>("Hello, world!\n");
    std::cout
        << "Created string variable with \"Hello, world!\n\"." << std::endl;

    const auto lenStrVar = len(strVar);
    std::cout
        << "Calculated length of string variable: "
        << std::dynamic_pointer_cast<NumberVariable>(lenStrVar)->value
        << " (Expected " << std::string("Hello, world!\n").length() << ")."
        << std::endl;
    
    const auto lsVar = std::make_shared<ListVariable>(
        std::vector<VariablePointer>({
            std::make_shared<NumberVariable>(1),
            std::make_shared<NumberVariable>(2),
            std::make_shared<NumberVariable>(3),
            std::make_shared<NumberVariable>(4),
            std::make_shared<NumberVariable>(5),
            std::make_shared<NumberVariable>(6)
        })
    );
    std::cout << "Created list variable of length 6." << std::endl;

    const auto lenLsVar = len(lsVar);
    std::cout
        << "Expected length 6, Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(lenLsVar)->value
        << "." << std::endl
        << "Test completed." << std::endl;
}

void testParseNum(void) {
    std::cout << "Testing std.hpp:parseNum." << std::endl;

    for(double i = 0; i < 1; i += 0.01) {
        const std::string converted = std::to_string(i);
        std::cout
            << "Created string version of " << i
            << ": \"" << converted << "\"." << std::endl;
        
        const auto convertVar = std::make_shared<StringVariable>(converted);
        std::cout
            << "Created StringVariable from converted number." << std::endl;

        const auto numVar = parseNum(convertVar);
        std::cout
            << "Created NumberVariable via parseNum: "
            << std::dynamic_pointer_cast<NumberVariable>(
                numVar->toNumber()
            )->value << "." << std::endl
            << "Test complete." << std::endl;
    }
}

void testInput(void) {
    std::cout << "Testing std.hpp:input" << std::endl;
    
    const auto dummyNum = std::make_shared<NumberVariable>(0);
    std::cout
        << "Created dummy number to pass to input function." << std::endl
        << "Please enter a value to test input: ";
    
    const auto inputResult = input(dummyNum);
    std::cout
        << "Function 'input' returned: '\""
        << std::dynamic_pointer_cast<StringVariable>(
            inputResult->toString()
        )->value << "\"" << std::endl
        << "Test complete." << std::endl;
}

void testPrint(void) {
    std::cout << "Testing std.hpp:print" << std::endl;

    const auto hWorldMsg = std::make_shared<StringVariable>("Hello, world!\n");
    std::cout
        << "Created \"Hello, world!\n\" StringVariable." << std::endl
        << "Printing StringVariable." << std::endl;
    const auto printResult = print(hWorldMsg);
    std::cout
        << "Printed StringVariable." << std::endl
        << "Function 'print' returned \""
        << std::dynamic_pointer_cast<StringVariable>(
            printResult->toString()
        )->value << "\"" << std::endl
        << "Test complete." << std::endl;
}
