/*
 * Author: Dylan Turner
 * Description:
 *   Define the built-in types used in the language
 *   and whatever conversion funcs are necessary
 */

/*
 * We assume that
 *  - nabd string -> std::string
 *  - nabd number -> double
 *  - nabd list<T> -> std::vector<T>
 *  - nabd pair<T1, T2> -> std::pair<T1, T2>
 * However, we need more than just conversion functions so we can just get a value
 * when we call toInt or something
 */

#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>

namespace nabd {
    // Interface for all variables to be convertable to eachother
    enum class VariableType {
        String, Number,
        List, Tuple
    };
    struct Variable {
        virtual std::shared_ptr<Variable> toString(void) const = 0;
        virtual std::shared_ptr<Variable> toNumber(void) const = 0;
        virtual std::shared_ptr<Variable> toList(
            const std::vector<VariableType> &subTypes
        ) const = 0;
        virtual std::shared_ptr<Variable> toTuple(void) const = 0;
    };
    typedef std::shared_ptr<Variable> VariablePointer;

    struct StringVariable : public Variable {
        public:
            StringVariable(const std::string &data);
            
            VariablePointer toString(void) const override;
            VariablePointer toNumber(void) const override;
            VariablePointer toList(
                const std::vector<VariableType> &subTypes
            ) const override;
            VariablePointer toTuple(void) const override;
            
            const std::string value;
    };

    struct NumberVariable : public Variable {
        public:
            NumberVariable(const double data);
            
            VariablePointer toString(void) const override;
            VariablePointer toNumber(void) const override;
            VariablePointer toList(
                const std::vector<VariableType> &subTypes
            ) const override;
            VariablePointer toTuple(void) const override;
            
            const double value;
    };

    struct ListVariable : public Variable {
        public:
            ListVariable(const std::vector<VariablePointer> &data);
            
            VariablePointer toString(void) const override;
            VariablePointer toNumber(void) const override;
            VariablePointer toList(
                const std::vector<VariableType> &subTypes
            ) const override;
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
            VariablePointer toList(
                const std::vector<VariableType> &subTypes
            ) const override;
            VariablePointer toTuple(void) const override;
            
            const std::pair<VariablePointer, VariablePointer> values;
    };
}
