/*
 * Author: Dylan Turner
 * Description:
 *   Define the built-in types used in the language
 *   and whatever conversion funcs are necessary
 */

#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <any>
#include <Variable.hpp>

using namespace nabd;

template<class T1, class T2>
Variable::Variable(
        const VariableType varType, const VariableType subVarType,
        const std::vector<T1> &first, const T2 &second) :
        type(varType), subType(subVarType), value(first), value2(second) {
}

Variable<std::string, int> Variable::toStr(void) const {
    switch(type) {
        case VariableType.String:
            return *this;
        case VariableType.Number:
            return Variable<std::string, int>(
                VariableType.String, VariableType.String,
                std::to_string(value[0]), 0
            );
        case VariableType.Tuple:
            return Variable<std::string, int>(
                VariableType.String, VariableType.String,
                "(" + value[0].toStr() + ", " + value2.toStr() + ")", 0
            );
        case VariableType.List: {
            std::stringstream listRep;
            listRep << "{";
            for(const auto &var : first) {
                listRep << var.toStr();
                if(&var != first.end() - 1) {
                    listRep << ", ";
                }
            }
            listRep << "}";
            return Variable<std::string, int>(
                VariableType.String, VariableType.String,
                listRep.str(), 0
            );
        }
    }
}

Variable<double, int> Variable::toNum(void) const {
    switch(type) {
        case Variable.String:
            return Variable<double, int>(
                VariableType.Number, VariableType.Number,
                value[0].length() > 0 ? static_cast<double>(value[0][0]) : 0, 0
            );
        case Variable.Number:
            return *this;
        case Variable.Tuple:
        case Variable.List:
            return value[0].toNum();
    }
}

/*
 * This is complex because of this problem:
 * How to convert a, say, string to a list of list of ints or something?
 * Well, first we want to convert the string to an num
 *  - num(strVar)
 *  - toNum()
 * Then we want to package it:
 *  - [ num(strVar) ]
 *  - Variable<double, int>(List, Number, std::vector<double>({ toNum().value[0]) }, 0)
 * Then we want to package that:
 *  - [ [ numberVar ] ]
 *  - Variable<Variable<double, int>, int>(
 *        List, List,
 *        std::vector<Variable<double, int>({ toLs(Number)
 * So how do we know to put a number there this time?
 * We can't get the type of the template's template and check it's double, right?
 * So we have to convert to list first, and then convert to the right kind of list
 */

template<class OtherT>
Variable<T, int> Variable::toLs(const VariableType subType) {

}

template<class T1, class T2>
Variable<T1, T2> Variable::_makeLs(void) const {
    return Variable<T1, T2>(
        VariableType.List, VariableType.String,
        std::vector<Variable<T1, T2>>({ *this }), 0
    );
}
