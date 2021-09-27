/*
 * Author: Dylan Turner
 * Description: Library used to provide functions to the language
 */

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <Variable.hpp>

inline VariablePointer print(const VariablePointer &msg) {
    std::cout <<
        std::dynamic_pointer_cast<StringVariable>(msg->toString())->value;
    return msg->toString();
}

inline VariablePointer input(const VariablePointer &dummy) {
    std::string str;
    std::getline(std::cin, str);
    return std::make_shared<StringVariable>(str);
}

inline VariablePointer parseNum(const VariablePointer &str) {
    const auto strData = std::dynamic_pointer_cast<StringVariable>(
        str
    )->value;
    char *end;
    const auto num = std::strtod(strData.c_str(), &end);
    return std::make_shared<NumberVariable>(num);
}

inline VariablePointer len(const VariablePointer &ls) {
    const auto strData = std::dynamic_pointer_cast<ListVariable>(
        ls->toList(std::vector<VariableType>({ VariableType::Tuple }))
    );
    return std::make_shared<NumberVariable>(strData->values.size());
}

inline VariablePointer gt(const VariablePointer &tup) {
    const auto items = std::dynamic_pointer_cast<TupleVariable>(
        tup->toTuple()
    )->values;
    const auto num1 = std::dynamic_pointer_cast<NumberVariable>(
        items.first->toNumber()
    )->value;
    const auto num2 = std::dynamic_pointer_cast<NumberVariable>(
        items.second->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num1 > num2 ? 1 : -1);
}

inline VariablePointer lt(const VariablePointer &tup) {
    const auto items = std::dynamic_pointer_cast<TupleVariable>(
        tup->toTuple()
    )->values;
    const auto num1 = std::dynamic_pointer_cast<NumberVariable>(
        items.first->toNumber()
    )->value;
    const auto num2 = std::dynamic_pointer_cast<NumberVariable>(
        items.second->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num1 < num2 ? 1 : -1);
}

inline VariablePointer eq(const VariablePointer &tup) {
    const auto items = std::dynamic_pointer_cast<TupleVariable>(
        tup->toTuple()
    )->values;
    const auto num1 = std::dynamic_pointer_cast<NumberVariable>(
        items.first->toNumber()
    )->value;
    const auto num2 = std::dynamic_pointer_cast<NumberVariable>(
        items.second->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num1 == num2 ? 1 : -1);
}

inline VariablePointer gte(const VariablePointer &tup) {
    const auto items = std::dynamic_pointer_cast<TupleVariable>(
        tup->toTuple()
    )->values;
    const auto num1 = std::dynamic_pointer_cast<NumberVariable>(
        items.first->toNumber()
    )->value;
    const auto num2 = std::dynamic_pointer_cast<NumberVariable>(
        items.second->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num1 >= num2 ? 1 : -1);
}

inline VariablePointer lte(const VariablePointer &tup) {
    const auto items = std::dynamic_pointer_cast<TupleVariable>(
        tup->toTuple()
    )->values;
    const auto num1 = std::dynamic_pointer_cast<NumberVariable>(
        items.first->toNumber()
    )->value;
    const auto num2 = std::dynamic_pointer_cast<NumberVariable>(
        items.second->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num1 <= num2 ? 1 : -1);
}

inline VariablePointer ne(const VariablePointer &tup) {
    const auto items = std::dynamic_pointer_cast<TupleVariable>(
        tup->toTuple()
    )->values;
    const auto num1 = std::dynamic_pointer_cast<NumberVariable>(
        items.first->toNumber()
    )->value;
    const auto num2 = std::dynamic_pointer_cast<NumberVariable>(
        items.second->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num1 != num2 ? 1 : -1);
}

inline VariablePointer swap(const VariablePointer &param) {
    const auto paramItems = std::dynamic_pointer_cast<ListVariable>(
        param
    )->values;
    
    if(paramItems.size() < 2) {
        return param;
    }
    if(paramItems.size() < 3) {
        return paramItems[1];
    }

    const auto ind = std::dynamic_pointer_cast<NumberVariable>(
        paramItems[0]->toNumber()
    )->value;
    const auto baseList = std::dynamic_pointer_cast<ListVariable>(
        paramItems[1]->toList({ VariableType::Tuple })
    )->values;

    if(ind >= baseList.size()) {
        return paramItems[1];
    } else {
        std::vector<VariablePointer> newData;
        for(size_t i = 0; i < baseList.size(); i++) {
            if(i == static_cast<size_t>(std::floor(ind))) {
                newData.push_back(paramItems[2]);
                continue;
            }

            newData.push_back(baseList[i]);
        }
        return std::make_shared<ListVariable>(newData);
    }
}

inline VariablePointer seedRandom(const VariablePointer &dummy) {
    std::srand(std::time(NULL));
    return std::make_shared<NumberVariable>(0);
}

inline VariablePointer random(const VariablePointer &btwn) {
    const auto rangeVar = std::dynamic_pointer_cast<TupleVariable>(
        btwn->toTuple()
    )->values;
    const auto min = std::dynamic_pointer_cast<NumberVariable>(
        rangeVar.first->toNumber()
    )->value;
    const auto max = std::dynamic_pointer_cast<NumberVariable>(
        rangeVar.second->toNumber()
    )->value;

    const auto range = max - min;
    const auto div = static_cast<double>(RAND_MAX) / range;
    const auto num = min + (static_cast<double>(rand()) / div);

    return std::make_shared<NumberVariable>(num);
}

inline VariablePointer dup(const VariablePointer &var) {
    return std::make_shared<TupleVariable>(std::make_pair(var, var));
}

inline VariablePointer round(const VariablePointer &num) {
    const auto value = std::dynamic_pointer_cast<NumberVariable>(
        num->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(std::round(value));
}

inline VariablePointer floor(const VariablePointer &num) {
    const auto value = std::dynamic_pointer_cast<NumberVariable>(
        num->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(std::floor(value));
}

inline VariablePointer ceil(const VariablePointer &num) {
    const auto value = std::dynamic_pointer_cast<NumberVariable>(
        num->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(std::ceil(value));
}

inline VariablePointer fst(const VariablePointer &param) {
    const auto pair = std::dynamic_pointer_cast<TupleVariable>(
        param->toTuple()
    )->values;
    return pair.first;
}

inline VariablePointer snd(const VariablePointer &param) {
    const auto pair = std::dynamic_pointer_cast<TupleVariable>(
        param->toTuple()
    )->values;
    return pair.second;
}

inline VariablePointer elem(const VariablePointer &param) {
    const auto pair = std::dynamic_pointer_cast<TupleVariable>(
        param->toTuple()
    )->values;
    const auto list = std::dynamic_pointer_cast<ListVariable>(
        pair.second->toList(std::vector<VariableType>({ VariableType::Tuple }))
    )->values;
    const auto ind = std::dynamic_pointer_cast<NumberVariable>(
        pair.first->toNumber()
    )->value;

    if(ind >= list.size()) {
        return param;
    }

    return list[ind];
}

inline VariablePointer inc(const VariablePointer &numVar) {
    const auto num = std::dynamic_pointer_cast<NumberVariable>(
        numVar->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num + 1);
}

inline VariablePointer dec(const VariablePointer &numVar) {
    const auto num = std::dynamic_pointer_cast<NumberVariable>(
        numVar->toNumber()
    )->value;
    return std::make_shared<NumberVariable>(num - 1);
}
