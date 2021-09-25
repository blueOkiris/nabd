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

parser::ParserResult parser::parseProgram(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    std::vector<Token> subTokens;

    while(newInd < code.length()) {
        // Try to get an include at the top level
        const auto isInclude = parseInclude(code, newInd, newLine, newCol);
        if(isInclude.success) {
            subTokens.push_back(isInclude.result);
            newLine = isInclude.newLine;
            newCol = isInclude.newCol;
            newInd = isInclude.newInd;
            continue;
        }

        // Try to get an func def at the top level
        const auto isFuncDef = parseFuncDef(code, newInd, newLine, newCol);
        if(isFuncDef.success) {
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

parser::ParserResult parser::parseInclude(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;

    const auto firstSign = parseDolSign(code, newInd, newLine, newCol);
    if(!firstSign.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newLine = firstSign.newLine;
    newCol = firstSign.newCol;
    newInd = firstSign.newInd;

    const auto modName = parseIdentifier(code, newInd, newLine, newCol);
    if(!modName.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newLine = modName.newLine;
    newCol = modName.newCol;
    newInd = modName.newInd;

    const auto secondSign = parseDolSign(code, newInd, newLine, newCol);
    if(!secondSign.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newLine = secondSign.newLine;
    newCol = secondSign.newCol;
    newInd = secondSign.newInd;

    std::vector<Token> subTokens({
        firstSign.result,
        modName.result,
        secondSign.result
    });

    return {
        { TokenType::Include, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseDolSign(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '$') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::DolSign, "$", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseIdentifier(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    auto startLine = newLine, startCol = newCol;
    if(!isAlpha(code[newInd]) && code[newInd] != '_') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
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
            TokenType::Identifier, ident.str(),
            startLine, startCol, std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseFuncDef(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;

    const auto ident = parseIdentifier(code, newInd, newLine, newCol);
    if(!ident.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = ident.newInd;
    newLine = ident.newLine;
    newCol = ident.newCol;

    const auto equSign = parseEquSign(code, newInd, newLine, newCol);
    if(!equSign.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = equSign.newInd;
    newLine = equSign.newLine;
    newCol = equSign.newCol;

    const auto paramName = parseIdentifier(code, newInd, newLine, newCol);
    if(!paramName.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = paramName.newInd;
    newLine = paramName.newLine;
    newCol = paramName.newCol;

    const auto rightArr = parseRArr(code, newInd, newLine, newCol);
    if(!rightArr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = rightArr.newInd;
    newLine = rightArr.newLine;
    newCol = rightArr.newCol;

    const auto expr = parseExpr(code, newInd, newLine, newCol);
    if(!expr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = expr.newInd;
    newLine = expr.newLine;
    newCol = expr.newCol;

    const auto period = parsePeriod(code, newInd, newLine, newCol);
    if(!period.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = period.newInd;
    newLine = period.newLine;
    newCol = period.newCol;

    const std::vector<Token> subTokens({
        ident.result,
        equSign.result,
        paramName.result,
        rightArr.result,
        expr.result,
        period.result
    });

    return {
        { TokenType::FuncDef, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseEquSign(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '=') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::EquSign, "=", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseRArr(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '>') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::EquSign, ">", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseExpr(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    /*// Try to get a func call
    const auto isCall = parseFuncCall(code, index, curLine, curCol);
    if(isCall.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isCall.result })
            }, isCall.newLine, isCall.newCol, isCall.newInd, true
        };
    }

    // Try to get an ternary
    const auto isTernary = parseTernary(code, index, curLine, curCol);
    if(isTernary.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isTernary.result }) 
            }, isTernary.newLine, isTernary.newCol, isTernary.newInd, true
        };
    }
    
    // Try to get a string
    const auto isString = parseString(code, index, curLine, curCol);
    if(isString.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isString.result }) 
            }, isString.newLine, isString.newCol, isString.newInd, true
        };
    }
    
    // Try to get a decimal number
    const auto isDecimal = parseDecimal(code, index, curLine, curCol);
    if(isDecimal.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isDecimal.result }) 
            }, isDecimal.newLine, isDecimal.newCol, isDecimal.newInd, true
        };
    }
    
    // Try to get a hex number
    const auto isHex = parseHex(code, index, curLine, curCol);
    if(isHex.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isHex.result }) 
            }, isHex.newLine, isHex.newCol, isHex.newInd, true
        };
    }
    
    // Try to get a hex number
    const auto isHex = parseHex(code, index, curLine, curCol);
    if(isHex.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isHex.result }) 
            }, isHex.newLine, isHex.newCol, isHex.newInd, true
        };
    }
    
    // Try to get a hex number
    const auto isHex = parseHex(code, index, curLine, curCol);
    if(isHex.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isHex.result }) 
            }, isHex.newLine, isHex.newCol, isHex.newInd, true
        };
    }
    
    // Try to get a tuple
    const auto isTup = parseTupDef(code, index, curLine, curCol);
    if(isTup.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isTup.result }) 
            }, isTup.newLine, isTup.newCol, isTup.newInd, true
        };
    }
    
    // Try to get a list
    const auto isLs = parseListDef(code, index, curLine, curCol);
    if(isLs.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ isLs.result }) 
            }, isLs.newLine, isLs.newCol, isLs.newInd, true
        };
    }*/

    // Otherwise fail
    return {
        { TokenType::Error, "", 0, 0, std::vector<Token>() },
        curLine, curCol, index, false
    };
}

parser::ParserResult parser::parsePeriod(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '.') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::Period, ".", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}
