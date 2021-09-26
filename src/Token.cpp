/*
 * Author: Dylan Turner
 * Description: Implementation of token string function
 */

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <Utility.hpp>
#include <Token.hpp>

using namespace nabd;

const std::map<nabd::TokenType, std::string> g_typeStr = {
    { TokenType::Program,       "Program" },
    { TokenType::Include,       "Include" },
    { TokenType::FuncDef,       "FuncDef" },
    { TokenType::FuncCall,      "FuncCall" },
    { TokenType::Ternary,       "Ternary" },
    { TokenType::ListDef,       "ListDef" },
    { TokenType::TupDef,        "TupDef" },
    { TokenType::StrTpName,     "StrTpName" },
    { TokenType::NumTpName,     "NumTpName" },
    { TokenType::LsTp,          "LsTp" },
    { TokenType::TupTp,         "TupTp" },
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
    { TokenType::Exclam,        "Exclam" },
    { TokenType::Decimal,       "Decimal" },
    { TokenType::Hex,           "Hex" },
    { TokenType::String,        "String" },
    { TokenType::Identifier,    "Identifier" },
    { TokenType::Error,         "Error" }
};

std::string Token::str(const uint32_t padding) const {
    std::stringstream tokStr;
    padStringStream(tokStr, padding, '|');
    tokStr << "Tok w/ tp '";
    tokStr << g_typeStr.at(type);
    if(value != "") {
        tokStr << "' & val='" << value;
    }
    tokStr << "'";
    if(line != 0 && col != 0) {
        tokStr << " on ln " << line << ", col " << col;
    }
    for(const auto &child : children) {
        tokStr << '\n' << child.str(padding + 1);
    }
    return tokStr.str();
}
