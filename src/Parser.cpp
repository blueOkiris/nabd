/*
 * Author: Dylan Turner
 * Description: Implementation of tool for extracting meaning from nabd source
 */

#include <string>
#include <vector>
#include <sstream>
#include <map>
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
