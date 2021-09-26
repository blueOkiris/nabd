/*
 * Author: Dylan Turner
 * Description: Convert a program token into a compilable C++ program
 */

#include <string>
#include <sstream>
#include <fstream>
#include <Utility.hpp>
#include <Token.hpp>
#include <Parser.hpp>
#include <FileIo.hpp>
#include <CodeGen.hpp>

using namespace nabd;

std::string codegen::generateCppCode(
        const Token &program,
        const InputArguments &cliInputs,
        const ModuleInfo &modInfo) {
    if(!dirExists(modInfo.buildFolder)) {
        std::cout
            << "Build folder '" << modInfo.buildFolder
            << "' does not exist. Creating!" << std::endl;
    }
    createDirectory(modInfo.buildFolder);

    std::stringstream cppCode;
    cppCode << "#include <Variable.hpp>\n";

    // Add includes and header definitions
    for(const auto &topLevelTok : program.children) {
        switch(topLevelTok.type) {
            case TokenType::FuncDef:
                cppCode
                    << "VariablePointer "
                    << (
                        topLevelTok.children[0].value == "main" ?
                            "fake_main" :
                            topLevelTok.children[0].value
                    ) << "(const VariablePointer &"
                    << topLevelTok.children[2].value
                    << ");\n";
                break;
            
            case TokenType::Include:
                cppCode
                    << generateIncludeCode(topLevelTok, cliInputs, modInfo)
                    << "\n";
                break;
            
            default:
                break;
        }
    }

    // Actually implement the functions
    for(const auto &topLevelTok : program.children) {
        switch(topLevelTok.type) {
            case TokenType::FuncDef:
                cppCode << generateFuncDefCode(topLevelTok) << "\n";
                if(topLevelTok.children[0].value == "main") {
                    cppCode
                        << "int main(int argc, char **args) {\n"
                        << "  std::vector<VariablePointer> argVars;\n"
                        << "  for(int i = 1; i < argc; i++) {\n"
                        << "    argVars.push_back(\n"
                        << "      std::make_shared<StringVariable>(\n"
                        << "        std::string(args[i])\n"
                        << "      )\n"
                        << "    );\n"
                        << "  }\n"
                        << "  const auto retVal = fake_main(\n"
                        << "    std::make_shared<ListVariable>(argVars)\n"
                        << "  );\n"
                        << "  return static_cast<int>(\n"
                        << "    std::dynamic_pointer_cast<NumberVariable>(\n"
                        << "      retVal->toNumber()\n"
                        << "    )->value\n"
                        << "  );\n"
                        << "}\n";
                }
                break;
            
            default:
                break;
        }
    }

    return cppCode.str();
}

std::string codegen::generateIncludeCode(
        const Token &include,
        const InputArguments &cliInputs,
        const ModuleInfo &modInfo) {
    const auto ident = include.children[1].value; // $ <ident> $ -> <ident>

    // Get the real file name corresponding to the modul name
    std::string fileName = "";
    bool isCpp = false;
    for(const auto &folder : cliInputs.includeFolders) {
        std::cout
            << "Checking for module '" << ident << "' in folder '"
            << folder << '\'' << std::endl;

        // Check for C++ code
        const auto hppFileName =
            (folder != "" ? (folder + "/") : "") + ident + ".hpp";
        std::cout << "Checking for '" << hppFileName << "'..." << std::endl;
        std::ifstream hppFile(hppFileName);
        if(hppFile.is_open()) {
            hppFile.close();
            fileName = hppFileName;
            isCpp = true;
            break;
        }

        // Check for nabd code
        const auto nabdFileName =
            (folder != "" ? (folder + "/") : "") + ident + ".nabd";
        std::cout << "Checking for '" << nabdFileName << "'..." << std::endl;
        std::ifstream moduleFile(nabdFileName);
        if(moduleFile.is_open()) {
            moduleFile.close();
            fileName = nabdFileName;
            isCpp = false;
            break;
        }
    }

    if(fileName == "") {
        errorOut("Can't find included module '" + ident + "'!");
    }

    /*
     * If it's a header file, we can just include and gcc will handle it
     * But if it's not, we need to open the file and generate a header
     * in the module folder
     */
    if(!isCpp) {
        generateHeaderFile(fileName, ident, cliInputs, modInfo);
    } else {

    }
    
    return "#include <" + ident + ".hpp>";
}

// This assumes a file is known to exist and is a .nabd file
void codegen::generateHeaderFile(
        const std::string &moduleFile, const std::string &newFileNameBase,
        const InputArguments &cliInputs,
        const ModuleInfo &modInfo) {
    // We have to parse so we can extract function definitions
    const auto code = readFile(moduleFile);
    const auto prog = parser::parseProgram(code, 0, 1, 1).result;

    // Extract function definitions & store in header file code
    std::stringstream headerCode;
    headerCode << "#pragma once\n#include <Variable.hpp>\n";
    for(const auto &topLevelTok : prog.children) {
        switch(topLevelTok.type) {
            case TokenType::FuncDef:
                // ident = ident > ...
                headerCode
                    << "VariablePointer "
                    << (
                        topLevelTok.children[0].value == "main" ?
                            "fake_main" :
                            topLevelTok.children[0].value
                    ) << "(const VariablePointer &"
                    << topLevelTok.children[2].value
                    << ");\n";
                break;
            
            default:
                break;
        }
    }
    headerCode << "\n";

    // Save the header code
    const auto fileName = modInfo.buildFolder + "/" + newFileNameBase + ".hpp";
    std::ofstream headerFile(fileName);
    if(!headerFile.is_open()) {
        errorOut(
            "Failed to create header file for included module '"
                + newFileNameBase + "'!"
        );
    }
    headerFile << headerCode.str();
    headerFile.close();
}

std::string codegen::generateFuncDefCode(const Token &funcDef) {
    auto funcName = funcDef.children[0].value;
    if(funcName == "main") {
        funcName = "fake_main";
    }
    const auto funcParamName = funcDef.children[2].value;
    const auto exprCode = generateExprCode(funcDef.children[4]);

    return "VariablePointer " + funcName
        + "(const VariablePointer &" + funcParamName
        + ") {\n    return " + exprCode + ";\n}";
}

std::string codegen::generateExprCode(const Token &expr) {
    const auto subExpr =
        expr.type == TokenType::Identifier ?
            expr :
            expr.children[0];
    switch(subExpr.type) {
        case TokenType::FuncCall:
            return subExpr.children[0].value + "("
                + generateExprCode(subExpr.children[2]) + ")";
        
        case TokenType::Ternary:
            return generateExprCode(subExpr.children[1])
                + " ? " + generateExprCode(subExpr.children[3])
                + " : " + generateExprCode(subExpr.children[5]);
        
        case TokenType::String:
            return "std::make_shared<StringVariable>(\""
                + subExpr.value + "\")";
        
        case TokenType::Decimal:
            return "std::make_shared<NumberVariable>("
                + subExpr.value + ")";
        
        case TokenType::Hex:
            return "std::make_shared<NumberVariable>("
                "static_cast<double>(0x"
                + subExpr.value + "))";
        
        case TokenType::TupDef:
            // TODO!
            break;
        
        case TokenType::ListDef:
            // TODO!
            break;
        
        case TokenType::Identifier:
            return subExpr.value;
        
        default:
            break;
    }
    return "";
}
