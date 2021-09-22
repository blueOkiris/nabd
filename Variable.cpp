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
        case VariableType.List: {
            std::vector<VariableType> subSubTypes;
            for(const auto &type : subTypes) {
                if(type == *(subTypes.first())) {
                    continue;
                }
                subSubTypes.push_back(type);
            }
            
            return std::make_shared<ListVariable>(std::vector<VariablePointer>(
                self->toList(subSubTypes)
            ));
        }
        case VariableType.Number:
            return std::make_shared<ListVariable>(std::vector<VariablePointer>(
                self->toNumber()
            ));
        case VariableType.String:
            return std::make_shared<ListVariable>(std::vector<VariablePointer>(
                self->toString()
            ));
        case VariableType.Tuple:
            return std::make_shared<ListVariable>(std::vector<VariablePointer>(
                self->toTuple()
            ));
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
    return listHelper(std::shared_ptr(this), subTypes);
}

VariablePointer StringVariable::toTuple(void) const {
    return std::make_shared<TupleVariable>(std::make_pair(
        std::shared_ptr(this), std::shared_ptr(this)
    ));
}

/*struct NumberVariable : public Variable {
    public:
        NumberVariable(const double data);
        
        VariablePointer toString(void) const override;
        VariablePointer toNumber(void) const override;
        VariablePointer toList(void) const override;
        VariablePointer toTuple(void) const override;
        
        const double value;
};

struct ListVariable : public Variable {
    public:
        ListVariable(const std::vector<VariablePointer> &data);
        
        VariablePointer toString(void) const override;
        VariablePointer toNumber(void) const override;
        VariablePointer toList(void) const override;
        VariablePointer toTuple(void) const override;
        
        const std::vector<VariablePointer> values;
};

struct TupleVariable : public Variable {
    public:
        TupleVariable(
            const std::pair<VariablePointer, VariablePointer> &data
        );
        
        VariablePointer toString(void) const override;
        VariablePointer toNumber(void) const override;
        VariablePointer toList(void) const override;
        VariablePointer toTuple(void) const override;
        
        const std::pair<VariablePointer, VariablePointer> values;
};*/
