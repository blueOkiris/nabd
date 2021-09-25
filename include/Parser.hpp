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
 * <ternary> ::=        <exclam> <expr> <q-mark> <expr> <colon> <expr>
 * 
 * <list-def> ::=       <lbrak> <type> <type-op> { <exp> { <comma> <expr> } }
 * <tup-def> ::=        <lcurl> <type> <type-op> <expr> <comma>
 *                      <type> <type-op> <expr> <rcurl>
 * 
 * <expr> ::=           <func-call> | <ternary> |
 *                      <string> | <decimal> | <hex> |
 *                      <tup-def> | <list-def> | <identifier>
 * 
 * <type> ::=           <str-tp-name> | <num-tp-name> |
 *                      <ls-tp> | <tup-tp>
 * <ls-tp> ::=          <lbrak> <type> <rbrak>
 * <tup-tp> ::=         <lcurl> <type> <comma> <type> <rcurl>
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
 * <exclam> ::=         '!'
 * 
 * <str-tp-name> :=     'Str'
 * <num-tp-name> :=     'Num'
 * <decimal> ::=        /0d[0-9]+(\.[0-9]*)*#/
 * <hex> ::=            /0x[0-9a-fA-F]+#/
 * <string> ::=         /'(\\.|[^\\'])*'
 * <identifier> ::=     /[A-Za-z_][A-Za-z_0-9]+/
 */

namespace nabd {
    enum class TokenType {
        Program, Include, FuncDef,
        FuncCall, Ternary, ListDef, TupDef, Expr,
        StrTpName, NumTpName, LsTp, TupTp, Type,
        DolSign, EquSign, Period, RArr, LPar, RPar, QMark, Colon, LBrak, RBrak,
        LCurl, RCurl, Comma, TypeOp, Exclam,
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

    /*
     * This is the base form of a parser:
     * ParserResult parse(
     *     const std::string &code, const uint64_t index,
     *     const uint64_t curLine, const uint64_t curCol
     * );
     */
    namespace parser {
        struct ParserResult {
            Token result;
            uint64_t newLine, newCol, newInd;
            bool success;
        };

        ParserResult parseProgram(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseInclude(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseFuncDef(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseFuncCall(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseTernary(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseListDef(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseTupDef(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseExpr(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseStrTpName(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseNumTpName(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseLsTp(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseTupTp(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseType(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseDolSign(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseEquSign(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parsePeriod(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseRArr(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseLPar(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseRPar(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseQMark(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseColon(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseLBrak(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseRBrak(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseLCurl(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseRCurl(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseComma(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseTypeOp(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseExclam(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseDecimal(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseHex(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseString(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
        ParserResult parseIdentifier(
            const std::string &code, const uint64_t index,
            const uint64_t curLine, const uint64_t curCol
        );
    }
}
