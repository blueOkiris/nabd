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
#include <Variable.hpp>

using namespace nabd;

template<class T1, class T2>
Variable::Variable(
        const VariableType varType,
        const std::vector<Variable<T1>> &first, const Variable<T2> &second) :
        type(varType), value(first), value2(second) {
}

std::string Variable::toStr(void) const {
    switch(type) {
        case VariableType.String:
            return first[0];
        case VariableType.Number:
            return std::to_string(first[0]);
        case VariableType.Tuple:
            return "(" + first[0].toStr() + ", " + second.toStr() + ")";
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
            return listRep.str();
        }
    }
}

double Variable::toNum(void) const {
    switch(type) {
        case Variable.String:
            return first[0].length() > 0 ? static_cast<double>(first[0][0]) : 0;
        case Variable.Number:
            return first[0];
        case Variable.Tuple:
        case Variable.List:
            return first[0].toNum();
    }
}

template<class T1, class T2>
std::string Variable::toList(void) const {
    switch(type) {
        case VariableType.String:
        case VariableType.Number:
        case VariableType.Tuple:
            return Variable(
                VariableType.List,
                std::vector<Variable<T1, T2>>({ *this }),
                std::vector<int>({ 0 })
            );
        case VariableType.List:
            return *this;
    }
}
