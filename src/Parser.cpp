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
#include <Token.hpp>
#include <Parser.hpp>

using namespace nabd;

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
            eatWhiteSpace(code, newInd, newLine, newCol);
            continue;
        }

        // Try to get an func def at the top level
        const auto isFuncDef = parseFuncDef(code, newInd, newLine, newCol);
        if(isFuncDef.success) {
            subTokens.push_back(isFuncDef.result);
            newLine = isFuncDef.newLine;
            newCol = isFuncDef.newCol;
            newInd = isFuncDef.newInd;
            eatWhiteSpace(code, newInd, newLine, newCol);
            continue;
        }

        // Otherwise fail
        errorOut(
            std::string("Could not parse include or func def on ln ")
                + std::to_string(isFuncDef.newLine) + std::string(", col ")
                + std::to_string(isFuncDef.newCol)
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
            newLine, newCol, newInd, false
        };
    }
    newLine = firstSign.newLine;
    newCol = firstSign.newCol;
    newInd = firstSign.newInd;

    const auto modName = parseIdentifier(code, newInd, newLine, newCol);
    if(!modName.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newLine = modName.newLine;
    newCol = modName.newCol;
    newInd = modName.newInd;

    const auto secondSign = parseDolSign(code, newInd, newLine, newCol);
    if(!secondSign.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
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
    while(newInd < code.length() &&
            (isAlpha(code[newInd]) || isDigit(code[newInd])
            || code[newInd] == '_')) {
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
            newLine, newCol, newInd, false
        };
    }
    newInd = equSign.newInd;
    newLine = equSign.newLine;
    newCol = equSign.newCol;

    const auto paramName = parseIdentifier(code, newInd, newLine, newCol);
    if(!paramName.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = paramName.newInd;
    newLine = paramName.newLine;
    newCol = paramName.newCol;

    const auto rightArr = parseRArr(code, newInd, newLine, newCol);
    if(!rightArr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = rightArr.newInd;
    newLine = rightArr.newLine;
    newCol = rightArr.newCol;

    const auto expr = parseExpr(code, newInd, newLine, newCol);
    if(!expr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = expr.newInd;
    newLine = expr.newLine;
    newCol = expr.newCol;

    const auto period = parsePeriod(code, newInd, newLine, newCol);
    if(!period.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
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
    // Try to get a func call
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
    }
    
    // Try to get an identifier last
    const auto ident = parseIdentifier(code, index, curLine, curCol);
    if(ident.success) {
        return {
            {
                TokenType::Expr, "", 0, 0,
                std::vector<Token>({ ident.result }) 
            }, ident.newLine, ident.newCol, ident.newInd, true
        };
    }

    // Otherwise fail
    return {
        { TokenType::Error, "", 0, 0, std::vector<Token>() },
        curLine, curCol, index, false
    };
}

parser::ParserResult parser::parseFuncCall(
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

    const auto lPar = parseLPar(code, newInd, newLine, newCol);
    if(!lPar.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = lPar.newInd;
    newLine = lPar.newLine;
    newCol = lPar.newCol;

    const auto paramVal = parseExpr(code, newInd, newLine, newCol);
    if(!paramVal.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = paramVal.newInd;
    newLine = paramVal.newLine;
    newCol = paramVal.newCol;

    const auto rPar = parseRPar(code, newInd, newLine, newCol);
    if(!rPar.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = rPar.newInd;
    newLine = rPar.newLine;
    newCol = rPar.newCol;

    const std::vector<Token> subTokens({
        ident.result,
        lPar.result,
        paramVal.result,
        rPar.result
    });

    return {
        { TokenType::FuncCall, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseTernary(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;

    const auto exclam = parseExclam(code, newInd, newLine, newCol);
    if(!exclam.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = exclam.newInd;
    newLine = exclam.newLine;
    newCol = exclam.newCol;

    const auto testExpr = parseExpr(code, newInd, newLine, newCol);
    if(!testExpr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = testExpr.newInd;
    newLine = testExpr.newLine;
    newCol = testExpr.newCol;

    const auto qMark = parseQMark(code, newInd, newLine, newCol);
    if(!qMark.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = qMark.newInd;
    newLine = qMark.newLine;
    newCol = qMark.newCol;

    const auto trueExpr = parseExpr(code, newInd, newLine, newCol);
    if(!trueExpr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = trueExpr.newInd;
    newLine = trueExpr.newLine;
    newCol = trueExpr.newCol;

    const auto colon = parseColon(code, newInd, newLine, newCol);
    if(!colon.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = colon.newInd;
    newLine = colon.newLine;
    newCol = colon.newCol;

    const auto falseExpr = parseExpr(code, newInd, newLine, newCol);
    if(!falseExpr.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = falseExpr.newInd;
    newLine = falseExpr.newLine;
    newCol = falseExpr.newCol;

    const std::vector<Token> subTokens({
        exclam.result,
        testExpr.result,
        qMark.result,
        trueExpr.result,
        colon.result,
        falseExpr.result
    });

    return {
        { TokenType::Ternary, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseExclam(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '!') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::Exclam, "!", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseQMark(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '?') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::QMark, "?", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseColon(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != ':') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::Colon, ":", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseLPar(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '(') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::LPar, "(", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseRPar(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != ')') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::RPar, ")", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseString(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    auto startLine = newLine, startCol = newCol;
    if(code[newInd] != '\'') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd++;
    newCol++;
    std::stringstream str;
    while(newInd < code.length() && code[newInd] != '\'') {
        str << code[newInd];

        if(code[newInd] == '\\' && newInd + 1 < code.length()) {
            newInd++;
            newCol++;
            str << code[newInd];
        }
        
        if(code[newInd] == '\n') {
            newLine++;
            newCol = 1;
        } else {
            newCol++;
        }
        newInd++;
    }
    newCol++;
    newInd++;
    return {
        {
            TokenType::String, str.str(),
            startLine, startCol, std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseDecimal(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    auto startLine = newLine, startCol = newCol;
    if(code[newInd] != '0') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd++;
    newCol++;
    if(newInd >= code.length() || code[newInd] != 'd') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd++;
    newCol++;
    if(newInd >= code.length() || !isDigit(code[newInd])) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    std::stringstream numStr;
    while(newInd < code.length() && isDigit(code[newInd])) {
        numStr << code[newInd];
        
        newInd++;
        newCol++;
    }
    if(newInd < code.length() && code[newInd] == '.') {
        numStr << '.';
        newInd++;
        newCol++;
        while(newInd < code.length() && isDigit(code[newInd])) {
            numStr << code[newInd];
            
            newInd++;
            newCol++;
        }
    }
    if(newInd >= code.length() || code[newInd] != '#') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd++;
    newCol++;
    return {
        {
            TokenType::Decimal, numStr.str(),
            startLine, startCol, std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseHex(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    auto startLine = newLine, startCol = newCol;
    if(code[newInd] != '0') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd++;
    newCol++;
    if(newInd >= code.length() || code[newInd] != 'x') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd++;
    newCol++;
    if(newInd >= code.length()
            || !(
                isDigit(code[newInd])
                    || (code[newInd] >= 'A' && code[newInd] <= 'F')
                    || (code[newInd] >= 'a' && code[newInd] <= 'f')
            )) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    std::stringstream numStr;
    while(newInd < code.length()
            && (
                isDigit(code[newInd])
                    || (code[newInd] >= 'A' && code[newInd] <= 'F')
                    || (code[newInd] >= 'a' && code[newInd] <= 'f')
            )) {
        numStr << code[newInd];
        
        newInd++;
        newCol++;
    }
    if(newInd >= code.length() || code[newInd] != '#') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd++;
    newCol++;
    return {
        {
            TokenType::Hex, numStr.str(),
            startLine, startCol, std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseTupDef(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;

    const auto lCurl = parseLCurl(code, newInd, newLine, newCol);
    if(!lCurl.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = lCurl.newInd;
    newLine = lCurl.newLine;
    newCol = lCurl.newCol;

    const auto type1 = parseType(code, newInd, newLine, newCol);
    if(!type1.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = type1.newInd;
    newLine = type1.newLine;
    newCol = type1.newCol;

    const auto typeOp1 = parseTypeOp(code, newInd, newLine, newCol);
    if(!typeOp1.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = typeOp1.newInd;
    newLine = typeOp1.newLine;
    newCol = typeOp1.newCol;

    const auto expr1 = parseExpr(code, newInd, newLine, newCol);
    if(!expr1.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = expr1.newInd;
    newLine = expr1.newLine;
    newCol = expr1.newCol;

    const auto comma = parseComma(code, newInd, newLine, newCol);
    if(!comma.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = comma.newInd;
    newLine = comma.newLine;
    newCol = comma.newCol;

    const auto type2 = parseType(code, newInd, newLine, newCol);
    if(!type2.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = type2.newInd;
    newLine = type2.newLine;
    newCol = type2.newCol;

    const auto typeOp2 = parseTypeOp(code, newInd, newLine, newCol);
    if(!typeOp2.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = typeOp2.newInd;
    newLine = typeOp2.newLine;
    newCol = typeOp2.newCol;

    const auto expr2 = parseExpr(code, newInd, newLine, newCol);
    if(!expr2.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = expr2.newInd;
    newLine = expr2.newLine;
    newCol = expr2.newCol;

    const auto rCurl = parseRCurl(code, newInd, newLine, newCol);
    if(!rCurl.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = rCurl.newInd;
    newLine = rCurl.newLine;
    newCol = rCurl.newCol;

    const std::vector<Token> subTokens({
        lCurl.result,
        type1.result,
        typeOp1.result,
        expr1.result,
        type2.result,
        typeOp2.result,
        expr2.result,
        rCurl.result
    });

    return {
        { TokenType::TupDef, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseLCurl(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '{') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::LCurl, "{", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseType(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    // 'Str'
    const auto isStr = parseStrTpName(code, index, curLine, curCol);
    if(isStr.success) {
        return {
            {
                TokenType::Type, "", 0, 0,
                std::vector<Token>({ isStr.result })
            }, isStr.newLine, isStr.newCol, isStr.newInd, true
        };
    }

    // 'Num'
    const auto isNum = parseNumTpName(code, index, curLine, curCol);
    if(isNum.success) {
        return {
            {
                TokenType::Type, "", 0, 0,
                std::vector<Token>({ isNum.result }) 
            }, isNum.newLine, isNum.newCol, isNum.newInd, true
        };
    }
    
    // [ <type> ]
    const auto isLs = parseLsTp(code, index, curLine, curCol);
    if(isLs.success) {
        return {
            {
                TokenType::Type, "", 0, 0,
                std::vector<Token>({ isLs.result }) 
            }, isLs.newLine, isLs.newCol, isLs.newInd, true
        };
    }
    
    // { <type> , <type> }
    const auto isTup = parseTupTp(code, index, curLine, curCol);
    if(isTup.success) {
        return {
            {
                TokenType::Type, "", 0, 0,
                std::vector<Token>({ isTup.result }) 
            }, isTup.newLine, isTup.newCol, isTup.newInd, true
        };
    }

    // Otherwise fail
    return {
        { TokenType::Error, "", 0, 0, std::vector<Token>() },
        curLine, curCol, index, false
    };
}

parser::ParserResult parser::parseStrTpName(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != 'S') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    if(newInd >= code.length() || code[newInd] != 't') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newCol++;
    newInd++;
    if(newInd >= code.length() || code[newInd] != 'r') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newCol++;
    newInd++;
    return {
        {
            TokenType::StrTpName, "Str", newLine, newCol - 1,
            std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseNumTpName(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != 'N') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    if(newInd >= code.length() || code[newInd] != 'u') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newCol++;
    newInd++;
    if(newInd >= code.length() || code[newInd] != 'm') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newCol++;
    newInd++;
    return {
        {
            TokenType::NumTpName, "Num", newLine, newCol - 1,
            std::vector<Token>()
        }, newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseLsTp(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;

    const auto lBrak = parseLBrak(code, newInd, newLine, newCol);
    if(!lBrak.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = lBrak.newInd;
    newLine = lBrak.newLine;
    newCol = lBrak.newCol;

    const auto type = parseType(code, newInd, newLine, newCol);
    if(!type.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = type.newInd;
    newLine = type.newLine;
    newCol = type.newCol;

    const auto rBrak = parseRBrak(code, newInd, newLine, newCol);
    if(!rBrak.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = rBrak.newInd;
    newLine = rBrak.newLine;
    newCol = rBrak.newCol;

    const std::vector<Token> subTokens({
        lBrak.result,
        type.result,
        rBrak.result
    });

    return {
        { TokenType::LsTp, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseLBrak(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '[') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::LBrak, "[", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseRBrak(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != ']') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::RBrak, "]", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseTupTp(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;

    const auto lCurl = parseLCurl(code, newInd, newLine, newCol);
    if(!lCurl.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = lCurl.newInd;
    newLine = lCurl.newLine;
    newCol = lCurl.newCol;

    const auto type1 = parseType(code, newInd, newLine, newCol);
    if(!type1.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = type1.newInd;
    newLine = type1.newLine;
    newCol = type1.newCol;

    const auto comma = parseComma(code, newInd, newLine, newCol);
    if(!comma.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = comma.newInd;
    newLine = comma.newLine;
    newCol = comma.newCol;

    const auto type2 = parseType(code, newInd, newLine, newCol);
    if(!type2.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = type2.newInd;
    newLine = type2.newLine;
    newCol = type2.newCol;

    const auto rCurl = parseRCurl(code, newInd, newLine, newCol);
    if(!rCurl.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = rCurl.newInd;
    newLine = rCurl.newLine;
    newCol = rCurl.newCol;

    const std::vector<Token> subTokens({
        lCurl.result,
        type1.result,
        comma.result,
        type2.result,
        rCurl.result
    });

    return {
        { TokenType::TupTp, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseTypeOp(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != ':') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    if(newInd >= code.length() || code[newInd] != '>') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::TypeOp, ":>", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseComma(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != ',') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::Comma, ",", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseRCurl(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    eatWhiteSpace(code, newInd, newLine, newCol);
    if(code[newInd] != '}') {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newCol++;
    newInd++;
    return {
        { TokenType::RCurl, "}", newLine, newCol - 1, std::vector<Token>() },
        newLine, newCol, newInd, true
    };
}

parser::ParserResult parser::parseListDef(
        const std::string &code, const uint64_t index,
        const uint64_t curLine, const uint64_t curCol) {
    auto newLine = curLine, newCol = curCol, newInd = index;
    std::vector<Token> subTokens;

    const auto lBrak = parseLBrak(code, newInd, newLine, newCol);
    if(!lBrak.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            curLine, curCol, index, false
        };
    }
    newInd = lBrak.newInd;
    newLine = lBrak.newLine;
    newCol = lBrak.newCol;
    subTokens.push_back(lBrak.result);

    const auto type = parseType(code, newInd, newLine, newCol);
    if(!type.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = type.newInd;
    newLine = type.newLine;
    newCol = type.newCol;
    subTokens.push_back(type.result);

    const auto typeOp = parseTypeOp(code, newInd, newLine, newCol);
    if(!typeOp.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = typeOp.newInd;
    newLine = typeOp.newLine;
    newCol = typeOp.newCol;
    subTokens.push_back(typeOp.result);

    const auto expr = parseExpr(code, newInd, newLine, newCol);
    if(expr.success) {
        newInd = expr.newInd;
        newLine = expr.newLine;
        newCol = expr.newCol;
        subTokens.push_back(expr.result);
    }
    while(true) {
        const auto comma = parseComma(code, newInd, newLine, newCol);
        if(!comma.success) {
            break;
        }
        newInd = comma.newInd;
        newLine = comma.newLine;
        newCol = comma.newCol;

        const auto expr = parseExpr(code, newInd, newLine, newCol);
        if(!expr.success) {
            break;
        }
        newInd = expr.newInd;
        newLine = expr.newLine;
        newCol = expr.newCol;

        subTokens.push_back(comma.result);
        subTokens.push_back(expr.result);
    }

    const auto rBrak = parseRBrak(code, newInd, newLine, newCol);
    if(!rBrak.success) {
        return {
            { TokenType::Error, "", 0, 0, std::vector<Token>() },
            newLine, newCol, newInd, false
        };
    }
    newInd = rBrak.newInd;
    newLine = rBrak.newLine;
    newCol = rBrak.newCol;

    return {
        { TokenType::ListDef, "", 0, 0, subTokens },
        newLine, newCol, newInd, true
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
