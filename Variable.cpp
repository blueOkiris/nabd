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
        const VariableType varType,
        const std::vector<T1> &first, const T2 &second) :
        type(varType), value(first), value2(second) {
}

Variable<std::string> Variable::toStr(void) const {
    switch(type) {
        case VariableType.String:
            return *this;
        case VariableType.Number:
            return Variable<std::string, int>(
                VariableType.String,
                std::to_string(value[0]), 0
            );
        case VariableType.Tuple:
            return Variable<std::string, int>(
                VariableType.String,
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
                VariableType.String,
                listRep.str(), 0
            );
        }
    }
}

double Variable::toNum(void) const {
    switch(type) {
        case Variable.String:
            return Variable<double, int>(
                VariableType.Number,
                value[0].length() > 0 ? static_cast<double>(value[0][0]) : 0, 0
            );
        case Variable.Number:
            return *this;
        case Variable.Tuple:
        case Variable.List:
            return Variable<double, int>(
                VariableType.Number,
                value[0].toNum(), 0
            );
    }
}

template<class OtherT>
std::string Variable::toLs(const VariableType subType) const {
    std::vector<OtherT> newData;
    switch(type) {
        case VariableType.String:
        case VariableType.Number:
        case VariableType.Tuple:
            switch(subType) {
                case Variable.String:
                    newData.push_back(toStr());
                    break;
                case Variable.Number:
                    newData.push_back(toNum());
                    break;
                case Variable.Tuple:
                    newData.push_back(toTup());
                    break;
                case Variable.List:
                    newData.push_back(toLs(type));
                    break;
            }
            break;
        case VariableType.List:
            for(const auto &datum : value) {
                switch(subType) {
                    case Variable.String:
                        newData.push_back(datum.toStr());
                        break;
                    case Variable.Number:
                        newData.push_back(datum.toNum());
                        break;
                    case Variable.Tuple:
                        newData.push_back(datum.toTup());
                        break;
                    case Variable.List:
                        newData.push_back(datum.toLs(datum.type));
                        break;
                }
            }
            break;
    }
    return Variable<OtherT, int>(
        VariableType.List,
        newData, 0
    );
}
