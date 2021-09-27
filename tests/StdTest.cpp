/*
 * Author: Dylan Turner
 * Description: Tests of std.h library functions
 */

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <Variable.hpp>
#include <std.hpp>

void testPrint(void);
void testInput(void);
void testParseNum(void);
void testLen(void);
void testGt(void);
void testLt(void);
void testEq(void);
void testGte(void);
void testLte(void);
void testNe(void);

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
        } else if(std::string(args[i]) == "gt") {
            testGt();
        } else if(std::string(args[i]) == "lt") {
            testLt();
        } else if(std::string(args[i]) == "eq") {
            testEq();
        } else if(std::string(args[i]) == "gte") {
            testGte();
        } else if(std::string(args[i]) == "lte") {
            testLte();
        } else if(std::string(args[i]) == "ne") {
            testNe();
        }
    }
}

void testNe(void) {
    std::cout << "Testing std.hpp:ne." << std::endl;

    const auto num1 = std::make_shared<NumberVariable>(10.0);
    const auto num2 = std::make_shared<NumberVariable>(10.1);
    const auto tup1 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num2
    ));
    std::cout
        << "Created tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup1->toString()
        )->value << "." << std::endl;
    
    const auto neVal1 = ne(tup1);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            neVal1->toNumber()
        )->value << "." << std::endl;
    
    const auto tup2 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup2->toString()
        )->value << "." << std::endl;
    
    const auto neVal2 = ne(tup2);
    std::cout
        << "Expected result is -1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            neVal2->toNumber()
        )->value << "." << std::endl
        << "Test completed." << std::endl;
}

void testLte(void) {
    std::cout << "Testing std.hpp:lte." << std::endl;

    const auto num1 = std::make_shared<NumberVariable>(10.0);
    const auto num2 = std::make_shared<NumberVariable>(10.1);
    const auto tup1 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num2
    ));
    std::cout
        << "Created tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup1->toString()
        )->value << "." << std::endl;
    
    const auto lteVal1 = lte(tup1);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            lteVal1->toNumber()
        )->value << "." << std::endl;
    
    const auto tup2 = std::make_shared<TupleVariable>(std::make_pair(
        num2, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup2->toString()
        )->value << "." << std::endl;
    
    const auto lteVal2 = lte(tup2);
    std::cout
        << "Expected result is -1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            lteVal2->toNumber()
        )->value << "." << std::endl;
    
    const auto tup3 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup3->toString()
        )->value << "." << std::endl;
    
    const auto lteVal3 = lte(tup3);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            lteVal3->toNumber()
        )->value << "." << std::endl
        << "Test completed." << std::endl;
}

void testGte(void) {
    std::cout << "Testing std.hpp:gte." << std::endl;

    const auto num1 = std::make_shared<NumberVariable>(10.0);
    const auto num2 = std::make_shared<NumberVariable>(10.1);
    const auto tup1 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num2
    ));
    std::cout
        << "Created tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup1->toString()
        )->value << "." << std::endl;
    
    const auto gteVal1 = gte(tup1);
    std::cout
        << "Expected result is -1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            gteVal1->toNumber()
        )->value << "." << std::endl;
    
    const auto tup2 = std::make_shared<TupleVariable>(std::make_pair(
        num2, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup2->toString()
        )->value << "." << std::endl;
    
    const auto gteVal2 = gte(tup2);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            gteVal2->toNumber()
        )->value << "." << std::endl;
    
    const auto tup3 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup3->toString()
        )->value << "." << std::endl;
    
    const auto gteVal3 = gte(tup3);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            gteVal3->toNumber()
        )->value << "." << std::endl
        << "Test completed." << std::endl;
}

void testEq(void) {
    std::cout << "Testing std.hpp:eq." << std::endl;

    const auto num1 = std::make_shared<NumberVariable>(10.0);
    const auto num2 = std::make_shared<NumberVariable>(10.1);
    const auto tup1 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num2
    ));
    std::cout
        << "Created tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup1->toString()
        )->value << "." << std::endl;
    
    const auto eqVal1 = eq(tup1);
    std::cout
        << "Expected result is -1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            eqVal1->toNumber()
        )->value << "." << std::endl;
    
    const auto tup2 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup2->toString()
        )->value << "." << std::endl;
    
    const auto eqVal2 = eq(tup2);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            eqVal2->toNumber()
        )->value << "." << std::endl
        << "Test completed." << std::endl;
}

void testLt(void) {
    std::cout << "Testing std.hpp:lt." << std::endl;

    const auto num1 = std::make_shared<NumberVariable>(10.0);
    const auto num2 = std::make_shared<NumberVariable>(10.1);
    const auto tup1 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num2
    ));
    std::cout
        << "Created tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup1->toString()
        )->value << "." << std::endl;
    
    const auto ltVal1 = lt(tup1);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            ltVal1->toNumber()
        )->value << "." << std::endl;
    
    const auto tup2 = std::make_shared<TupleVariable>(std::make_pair(
        num2, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup2->toString()
        )->value << "." << std::endl;
    
    const auto ltVal2 = lt(tup2);
    std::cout
        << "Expected result is -1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            ltVal2->toNumber()
        )->value << "." << std::endl
        << "Test completed." << std::endl;
}

void testGt(void) {
    std::cout << "Testing std.hpp:gt." << std::endl;

    const auto num1 = std::make_shared<NumberVariable>(10.0);
    const auto num2 = std::make_shared<NumberVariable>(10.1);
    const auto tup1 = std::make_shared<TupleVariable>(std::make_pair(
        num1, num2
    ));
    std::cout
        << "Created tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup1->toString()
        )->value << "." << std::endl;
    
    const auto gtVal1 = gt(tup1);
    std::cout
        << "Expected result is -1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            gtVal1->toNumber()
        )->value << "." << std::endl;
    
    const auto tup2 = std::make_shared<TupleVariable>(std::make_pair(
        num2, num1
    ));
    std::cout
        << "Created new tuple of numbers: "
        << std::dynamic_pointer_cast<StringVariable>(
            tup2->toString()
        )->value << "." << std::endl;
    
    const auto gtVal2 = gt(tup2);
    std::cout
        << "Expected result is 1. Actual: "
        << std::dynamic_pointer_cast<NumberVariable>(
            gtVal2->toNumber()
        )->value << "." << std::endl
        << "Test completed." << std::endl;
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
