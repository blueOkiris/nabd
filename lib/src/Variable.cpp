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
#include <memory>
#include <Variable.hpp>

using namespace nabd;

// All of them use a similar toList, so this is code reuse
VariablePointer listHelper(
        VariablePointer self, const std::vector<VariableType> &subTypes) {
    switch(subTypes[0]) {
        case VariableType::List: {
            std::vector<VariableType> subSubTypes;
            for(const auto &type : subTypes) {
                if(type == *(subTypes.begin())) {
                    continue;
                }
                subSubTypes.push_back(type);
            }
            
            return std::make_shared<ListVariable>(std::vector<VariablePointer>({
                self->toList(subSubTypes)
            }));
        }
        case VariableType::Number:
            return std::make_shared<ListVariable>(std::vector<VariablePointer>({
                self->toNumber()
            }));
        case VariableType::String:
            return std::make_shared<ListVariable>(std::vector<VariablePointer>({
                self->toString()
            }));
        case VariableType::Tuple:
            return std::make_shared<ListVariable>(std::vector<VariablePointer>({
                self->toTuple()
            }));
        
        default:
            // Should happen
            return std::make_shared<ListVariable>(std::vector<VariablePointer>({
                std::make_shared<StringVariable>(
                    "randomly created list lol oops"
                )
            }));
    }
}

StringVariable::StringVariable(const std::string &data) : value(data) {
}

VariablePointer StringVariable::toString(void) const {
    return std::make_shared<StringVariable>(value);
}

VariablePointer StringVariable::toNumber(void) const {
    return std::make_shared<NumberVariable>(
        value.length() > 0 ? static_cast<double>(value[0]) : 0
    );
}

VariablePointer StringVariable::toList(
        const std::vector<VariableType> &subTypes) const {
    return listHelper(std::make_shared<StringVariable>(value), subTypes);
}

VariablePointer StringVariable::toTuple(void) const {
    return std::make_shared<TupleVariable>(std::make_pair(
        std::make_shared<StringVariable>(value),
        std::make_shared<StringVariable>(value)
    ));
}

NumberVariable::NumberVariable(const double data) : value(data) {
}

VariablePointer NumberVariable::toString(void) const {
    return std::make_shared<StringVariable>(std::to_string(value));
}

VariablePointer NumberVariable::toNumber(void) const {
    return std::make_shared<NumberVariable>(value);
}

VariablePointer NumberVariable::toList(
        const std::vector<VariableType> &subTypes) const {
    return listHelper(std::make_shared<NumberVariable>(value), subTypes);
}

VariablePointer NumberVariable::toTuple(void) const {
    return std::make_shared<TupleVariable>(std::make_pair(
        std::make_shared<NumberVariable>(value),
        std::make_shared<NumberVariable>(value)
    ));
}

TupleVariable::TupleVariable(
        const std::pair<VariablePointer, VariablePointer> &data) :
        values(data) {
}

VariablePointer TupleVariable::toString(void) const {
    return std::make_shared<StringVariable>(
        "(" +
            std::dynamic_pointer_cast<StringVariable>(
                values.first->toString()
            )->value + ", " + std::dynamic_pointer_cast<StringVariable>(
                values.second->toString()
            )->value + ")"
    );
}

VariablePointer TupleVariable::toNumber(void) const {
    return std::make_shared<NumberVariable>(
        std::dynamic_pointer_cast<NumberVariable>(
            values.first->toNumber()
        )->value
    );
}

VariablePointer TupleVariable::toList(
        const std::vector<VariableType> &subTypes) const {
    return listHelper(std::make_shared<TupleVariable>(values), subTypes);
}

VariablePointer TupleVariable::toTuple(void) const {
    return std::make_shared<TupleVariable>(TupleVariable(values));
}

ListVariable::ListVariable(
        const std::vector<VariablePointer> &data) :
        values(data) {
}

VariablePointer ListVariable::toString(void) const {
    std::stringstream listStr;
    listStr << "{";
    for(const auto value : values) {
        listStr <<
            std::dynamic_pointer_cast<StringVariable>(value->toString())->value;
        if(value != *(values.begin())) {
            listStr << ", ";
        }
    }
    return std::make_shared<StringVariable>(listStr.str());
}

VariablePointer ListVariable::toNumber(void) const {
    return std::make_shared<NumberVariable>(
        std::dynamic_pointer_cast<NumberVariable>(values[0]->toNumber())->value
    );
}

VariablePointer ListVariable::toList(
        const std::vector<VariableType> &subTypes) const {
    return std::make_shared<ListVariable>(values);
}

VariablePointer ListVariable::toTuple(void) const {
    return std::make_shared<TupleVariable>(std::make_pair(
        std::make_shared<ListVariable>(values),
        std::make_shared<ListVariable>(values)
    ));
}
