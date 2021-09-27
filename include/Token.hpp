/*
 * Author: Dylan Turner
 * Description: Abstraction of a source code piece for Nabd
 */

#pragma once

#include <string>
#include <vector>

namespace nabd {
    enum class TokenType {
        Program, Include, FuncDef,
        FuncCall, Ternary, ListDef, TupDef, Expr,
        DolSign, EquSign, Period, RArr, LPar, RPar, QMark, Colon, LBrak, RBrak,
        LCurl, RCurl, Comma, Exclam,
        Decimal, Hex, String, Identifier,
        Error
    };

    struct Token {
        std::string str(const uint32_t padding = 0) const;

        TokenType type;
        std::string value;
        uint64_t line, col;
        std::vector<Token> children;
    };
}
