/*
 * Author: Dylan Turner
 * Description:
 *  - Access to a files for copying into module folders to build
 *  - Tools for importing and building the project
 */

#pragma once

#include <string>
#include <vector>

namespace nabd {
    struct InputArguments {
        std::string fileName;
        std::vector<std::string> objects;
        std::vector<std::string> includeFolders;
        bool link;
    };
    InputArguments parseArguments(const int argc, const char **args);

    struct ModuleInfo {
        std::string fileName;
        std::string relativeDirectory;
        std::string baseFileName;
        std::string moduleName;
        std::string buildFolder;
    };
    ModuleInfo extractModuleInfo(const InputArguments &inputs);

    std::string generateMakefile(
        const InputArguments &inputs,
        const ModuleInfo &modInfo
    );

    extern const std::vector<std::string> g_makeFile;
    extern const std::string g_varHpp;
    extern const std::string g_varCpp;
}
