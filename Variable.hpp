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

enum class VariableType {
    String, Number,
    List, Tuple
};

// Generic variable struct that can be any of the four
template<class T1, class T2>
struct Variable {
    Variable(
        const VariableType varType,
        const VariableType subVarType, // only for list of lists
        const std::vector<T1> &first,
        const T2 &second // only for tuple
    );
    
    Variable<std::string, int> toStr(void) const;
    Variable<double, int> toNum(void) const;
    template<class OtherT>
    Variable<OtherT, int> toLs(void) const;
    template<class OtherT1, class OtherT2>
    Variable<OtherT1, OtherT2> toTup(void) const;
    
    const VariableType type;
    const VariableType subType;
    const std::vector<T1> value;
    const T2 value2; // Only used for tuple
};
