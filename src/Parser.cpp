/*
 * Author: Dylan Turner
 * Description: Implementation of tool for extracting meaning from nabd source
 */

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <tuple>
#include <Utility.hpp>
#include <Parser.hpp>

using namespace nabd;

const std::map<TokenType, std::string> g_typeStr = {
    { TokenType::Program,       "Program" },
    { TokenType::Include,       "Include" },
    { TokenType::FuncDef,       "FuncDef" },
    { TokenType::FuncCall,      "FuncCall" },
    { TokenType::Ternary,       "Ternary" },
    { TokenType::ListDef,       "ListDef" },
    { TokenType::TupDef,        "TupDef" },
    { TokenType::Expr,          "Expr" },
    { TokenType::Type,          "Type" },
    { TokenType::DolSign,       "DolSign" },
    { TokenType::EquSign,       "EquSign" },
    { TokenType::Period,        "Period" },
    { TokenType::RArr,          "RArr" },
    { TokenType::LPar,          "LPar" },
    { TokenType::RPar,          "RPar" },
    { TokenType::QMark,         "QMark" },
    { TokenType::Colon,         "Colon" },
    { TokenType::LBrak,         "LBrak" },
    { TokenType::RBrak,         "RBrak" },
    { TokenType::LCurl,         "LCurl" },
    { TokenType::RCurl,         "RCurl" },
    { TokenType::Comma,         "Comma" },
    { TokenType::TypeOp,        "TypeOp" },
    { TokenType::Decimal,       "Decimal" },
    { TokenType::Hex,           "Hex" },
    { TokenType::String,        "String" },
    { TokenType::Identifier,    "Identifier" },
    { TokenType::Error,         "Error" }
};

std::string Token::str(const uint32_t padding) const {
    std::stringstream tokStr;
    padStringStream(tokStr, padding);
    tokStr << "Tok w/ tp '";
    tokStr << g_typeStr.at(type);
    if(value != "") {
        tokStr << "' & val='" << value;
    }
    tokStr << "'";
    if(line != 0 && col != 0) {
        tokStr << " on ln " << line << ", col " << col;
    }
    if(children.size() < 0) {
        for(const auto &child : children) {
            tokStr << '\n' << child.str(padding + 1);
        }
    }
    return tokStr.str();
}

/* Parser implementations */

ParserResult ProgramParser::parse(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) const {
    auto newLine = curLine, newCol = curCol, newInd = index;
    std::vector<Token> subTokens;

    while(newInd < code.length()) {
        // Try to get an include at the top level
        const IncludeParser inc;
        const auto isInclude = inc.parse(code, newInd, newLine, newCol);
        if(isInclude.success) {
            subTokens.push_back(isInclude.result);
            newLine = isInclude.newLine;
            newCol = isInclude.newCol;
            newInd = isInclude.newInd;
            continue;
        }

        // Try to get an func def at the top level
        const FuncDefParser funcDef;
        const auto isFuncDef = funcDef.parse(code, newInd, newLine, newCol);
        if(isInclude.success) {
            subTokens.push_back(isFuncDef.result);
            newLine = isFuncDef.newLine;
            newCol = isFuncDef.newCol;
            newInd = isFuncDef.newInd;
            continue;
        }

        // Otherwise fail
        errorOut(
            std::string("Could not parse include or func def on ln ")
                + std::to_string(newLine) + std::string(", col ")
                + std::to_string(newCol)
        );
    }
    return {
        { TokenType::Program, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

ParserResult IncludeParser::parse(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) const {
    auto newLine = curLine, newCol = curCol, newInd = index;
    std::vector<Token> subTokens;

    const DolSignParser dolParser;
    const auto firstSign = dolParser.parse(code, newInd, newLine, newCol);
    if(!firstSign.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newLine = firstSign.newLine;
    newCol = firstSign.newCol;
    newInd = firstSign.newInd;

    const IdentifierParser identParser;
    const auto modName = identParser.parse(code, newInd, newLine, newCol);
    if(!modName.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newLine = modName.newLine;
    newCol = modName.newCol;
    newInd = modName.newInd;

    const auto secondSign = dolParser.parse(code, newInd, newLine, newCol);
    if(!secondSign.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newLine = secondSign.newLine;
    newCol = secondSign.newCol;
    newInd = secondSign.newInd;

    subTokens.push_back(firstSign.result);
    subTokens.push_back(modName.result);
    subTokens.push_back(secondSign.result);

    return {
        { TokenType::Include, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

ParserResult FuncDefParser::parse(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) const {
    return {
        { TokenType::Error, "", 0, 0, std::vector<Token>() },
        curLine, curCol, index, false
    };
}

ParserResult DolSignParser::parse(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) const {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '$') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::DolSign, "$", newLine, newCol, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

ParserResult IdentifierParser::parse(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) const {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    auto startLine = newLine, startCol = newCol;
    if(!isAlpha(code[newInd]) && code[newInd] != '_') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    std::stringstream ident;
    while(isAlpha(code[newInd]) || isDigit(code[newInd])
            || code[newInd] == '_') {
        ident << code[newInd];
        
        newInd++;
        newCol++;
    }
    return {
        {
            TokenType::String, ident.str(),
            startLine, startCol, std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}
