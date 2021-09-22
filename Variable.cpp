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
        const std::vector<Variable<T1>> &first, const Variable<T2> &second) :
        value(first), value2(second) {
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


