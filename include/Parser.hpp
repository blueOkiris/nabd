/*
 * Author: Dylan Turner
 * Description: Tool for extracting meaning from nabd source code
 */

#pragma once

#include <string>
#include <vector>
#include <tuple>

/*
 * EBNF for nabd
 *
 * <program> ::=        { (<include> | <func-def>) }
 * <include> ::=        <dol-sign> <identifier> <dol-sign>
 * <func-def> ::=       <identifier> <equ-sign> <identifier> <rarr>
 *                      <expr> <period>
 * <func-call> ::=      <identifier> <lpar> <expr> <rpar>
 * <ternary> ::=        <expression> <q-mark> <expr> <colon> <expr>
 * 
 * <list-def> ::=       <lbrak> <type> <type-op> { <exp> { <comma> <expr> } }
 * <tup-def> ::=        <lcurl> <type> <type-op> <expr> <comma>
 *                      <type> <type-op> <expr> <rcurl>
 * 
 * <expr> ::=           <func-call> | <ternary> |
 *                      <string> | <decimal> | <hex> |
 *                      <tup-def> | <list-def>
 * 
 * <type> ::=           'Str' | 'Num' |
 *                      <lbrak> <type> <rbrak> |
 *                      <lcurl> <type> <comma> <type> <rcurl>
 * 
 * <dol-sign> ::=       '$'
 * <equ-sign> ::=       '='
 * <period> ::=         '.'
 * <rarr> ::=           '>'
 * <lpar> ::=           '('
 * <rpar> ::=           ')'
 * <q-mark> ::=         '?'
 * <colon> ::=          ':'
 * <lbrak> ::=          '['
 * <rbrak> ::=          ']'
 * <lcurl> ::=          '{'
 * <rcurl> ::=          '}'
 * <comma> ::=          ','
 * <type-op> ::=        ':>'
 * 
 * <decimal> ::=        /0d[0-9]+(\.[0-9])*#/
 * <hex> ::=            /0x[0-9a-fA-F]+#/
 * <string> ::=         /'(\\.|[^\\'])*'
 * <identifier> ::=     /[A-Za-z_][A-Za-z_0-9]+/
 */

namespace nabd {
    enum class TokenType {
        Program, Include, FuncDef,
        FuncCall, Ternary, ListDef, TupDef, Expr,
        Type,
        DolSign, EquSign, Period, RArr, LPar, RPar, QMark, Colon, LBrak, RBrak,
        LCurl, RCurl, Comma, TypeOp,
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

    struct ParserResult {
        Token result;
        std::string codeLeft;
        bool success;
    };

    struct Parser {
        // Returns (Error, code, false) or (Other Token, rest of code, true)
        virtual ParserResult parse(const std::string &code) = 0;
    };

    // Define all these parsers
    
}
