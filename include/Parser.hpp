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
        uint64_t newLine, newCol, newInd;
        bool success;
    };

    struct Parser {
        // Returns (Error, code, false) or (Other Token, rest of code, true)
        virtual ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const = 0;
    };

    /* Define all these parsers */

    struct ProgramParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct IncludeParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct FuncDefParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct FuncCallParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct ListDefParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct TernaryParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct TupDefParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct ExprParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct TypeParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct DolSignParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct EquSignParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct PeriodParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct RArrParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct LParParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct RParParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct QMarkParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct ColonParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct LBrakParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct RBrakParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct LCurlParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct RCurlParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct CommaParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct TypeOpParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct DecimalParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct HexParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct StringParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };

    struct IdentifierParser : public Parser {
        ParserResult parse(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        ) const override;
    };
}
