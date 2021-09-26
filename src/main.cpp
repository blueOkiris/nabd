/*
 * Author: Dylan Turner
 * Description: Entry point of nabd compiler
 */

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <Utility.hpp>
#include <Parser.hpp>
#include <FileIo.hpp>
#include <CodeGen.hpp>

using namespace nabd;

std::string compile(const InputArguments &cliInputs, const ModuleInfo &modInfo);
void buildObj(
    const std::string &code,
    const InputArguments &cliInputs,
    const ModuleInfo &modInfo
);
void link(
    const InputArguments &cliInputs,
    const ModuleInfo &modInfo
);

int main(const int argc, const char **args) {
    std::cout << args[0] << std::endl;

    auto inputs = parseArguments(argc, args);
    const auto modInfo = extractModuleInfo(inputs);
    inputs.includeFolders.push_back(modInfo.relativeDirectory);
    
    if(!inputs.link) {
        const auto cppCode = compile(inputs, modInfo);
        buildObj(cppCode, inputs, modInfo);
    } else {
        link(inputs, modInfo);
    }

    return 0;
}

std::string compile(
        const InputArguments &cliInputs, const ModuleInfo &modInfo) {
    const auto code = readFile(modInfo.fileName);
    
    // Note: this fails out, so no check for success
    const auto progParserResult = parser::parseProgram(code, 0, 1, 1).result;
    const auto outputCode = codegen::generateCppCode(
        progParserResult, cliInputs, modInfo
    );

    return outputCode;
}

void buildObj(
        const std::string &cppCode,
        const InputArguments &cliInputs, const ModuleInfo &modInfo) {
    std::ofstream writer;

    // Output the C file
    const auto buildCppPath = modInfo.buildFolder + "/" + modInfo.moduleName
        + ".cpp";
    writer.open(buildCppPath);
    if(!writer.is_open()) {
        errorOut("Failed to create the cpp file!");
    }
    writer << cppCode;
    writer.close();

    // Output the needed Variable.hpp file
    const auto varHppPath = modInfo.buildFolder + "/Variable.hpp";
    writer.open(varHppPath);
    if(!writer.is_open()) {
        errorOut("Failed to create the Variable.hpp file!");
    }
    writer << g_varHpp;
    writer.close();

    // Output the Makefile
    const auto makefileSrc = generateMakefile(cliInputs, modInfo);
    const auto makefilePath = modInfo.buildFolder + "/Makefile";
    writer.open(makefilePath);
    if(!writer.is_open()) {
        errorOut("Failed to create Makefile!");
    }
    writer << makefileSrc;
    writer.close();

    // Run make in the build folder
    const auto makeCmd =
#if defined(_WIN32) || defined(WIN32)
        "mingw32-make"
#else
        "make"
#endif
        " -C " + modInfo.buildFolder;
    std::cout << makeCmd << std::endl;
    if(system(makeCmd.c_str()) != 0) {
        errorOut("Failed to build object file!");
    }

    // Copy the new object file back to the
    const auto copyCmd =
#if defined(_WIN32) || defined(WIN32)
        "powershell -command \"copy "
#else
        "cp "
#endif
        + modInfo.buildFolder + "/" + modInfo.moduleName + ".o "
        + modInfo.relativeDirectory
#if defined(_WIN32) || defined(WIN32)
        + "\"";
#else
        ;
#endif
    std::cout << copyCmd << std::endl;
    if(system(copyCmd.c_str()) != 0) {
        errorOut("Failed to copy object file from build folder to root dir!");
    }
}

void link(
        const InputArguments &cliInputs,
        const ModuleInfo &modInfo) {
    if(!dirExists(modInfo.buildFolder)) {
        std::cout
            << "Build folder '" << modInfo.buildFolder
            << "' does not exist. Creating!" << std::endl;
    }
    createDirectory(modInfo.buildFolder);

    std::ofstream writer;

    // Output Variable.hpp (needed by Variable.cpp)
    const auto varHppPath = modInfo.buildFolder + "/Variable.hpp";
    writer.open(varHppPath);
    if(!writer.is_open()) {
        errorOut("Failed to create the Variable.hpp file!");
    }
    writer << g_varHpp;
    writer.close();
    
    // Output Variable.cpp
    const auto varCppPath = modInfo.buildFolder + "/Variable.cpp";
    writer.open(varCppPath);
    if(!writer.is_open()) {
        errorOut("Failed to create the Variable.cpp file!");
    }
    writer << g_varCpp;
    writer.close();

    // Compile the Variable.cpp file
    const auto varCompCmd =
#if defined(_WIN32) || defined(WIN32)
        "mingw32-g++ "
#else
        "g++ "
#endif
        "-O2 -Wall -Werror -std=c++17 -I\"" + modInfo.buildFolder + "\" -o "
        + modInfo.buildFolder + "/Variable.o -c " + modInfo.buildFolder
        + "/Variable.cpp";
    std::cout << varCompCmd << std::endl;
    if(system(varCompCmd.c_str()) != 0) {
        errorOut("Failed to compile Variable.cpp!");
    }

    // Link
    std::stringstream linkCmd;
#if defined(_WIN32) || defined(WIN32)
    linkCmd << "mingw32-g++ ";
#else
    linkCmd << "g++ ";
#endif
    linkCmd << "-o " + modInfo.buildFolder + "/" + modInfo.moduleName << " ";
    for(const auto &obj : cliInputs.objects) {
        linkCmd << obj << " ";
    }
    linkCmd << modInfo.buildFolder << "/Variable.o";
    std::cout << linkCmd.str() << std::endl;
    if(system(linkCmd.str().c_str()) != 0) {
        errorOut("Failed to link objects!");
    }

    // Copy final executable
    const auto copyCmd =
#if defined(_WIN32) || defined(WIN32)
        "powershell -command \"copy "
#else
        "cp "
#endif
        + modInfo.buildFolder + "/" + modInfo.moduleName
#if defined(_WIN32) || defined(WIN32)
        + ".exe " + modInfo.relativeDirectory + "\"";
#else
        + " " + modInfo.relativeDirectory;
#endif
    std::cout << copyCmd << std::endl;
    if(system(copyCmd.c_str()) != 0) {
        errorOut("Failed to copy output file from build folder to root dir!");
    }
}
