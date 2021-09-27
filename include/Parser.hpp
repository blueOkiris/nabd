/*
 * Author: Dylan Turner
 * Description: Tool for extracting meaning from nabd source code
 */

#pragma once

#include <string>
#include <Token.hpp>

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
 * <list-def> ::=       <lbrak> { <exp> { <comma> <expr> } }
 *                      <rbrak>
 * <tup-def> ::=        <lcurl> <expr> <comma> <expr> <rcurl>
 * 
 * <expr> ::=           <func-call> | <ternary> |
 *                      <string> | <decimal> | <hex> |
 *                      <tup-def> | <list-def> | <identifier>
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
 * <exclam> ::=         '!'
 * 
 * <decimal> ::=        /0d[0-9]+(\.[0-9]*)*#/
 * <hex> ::=            /0x[0-9a-fA-F]+#/
 * <string> ::=         /'(\\.|[^\\'])*'
 * <identifier> ::=     /[A-Za-z_][A-Za-z_0-9]+/
 */

namespace nabd {
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
