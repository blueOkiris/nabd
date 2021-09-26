/*
 * Author: Dylan Turner
 * Description: Small functions for keeping code cleaner
 */

#pragma once

// For the directory creation, directory exists, and full path
#if defined(_WIN32) || defined(WIN32)
#include <io.h>
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

namespace nabd {
    inline void errorOut(const std::string &errorMsg) {
        std::cerr << "Error: " << errorMsg << std::endl;
        exit(-1);
    }

    inline void padStringStream(
            std::stringstream &ss, const uint32_t padding, char padC = ' ') {
        for(uint32_t i = 0; i < padding; i++) {
            ss << padC;
        }
    }

    inline std::string readFile(const std::string &fileName) {
        std::ifstream file(fileName);
        if(!file.is_open()) {
            errorOut("Could not open file '" + fileName + "'!");
        }
        std::ostringstream sstr;
        sstr << file.rdbuf();
        file.close();
        return sstr.str();
    }

    inline bool isWhiteSpace(const char c) {
        return (c == ' ') || (c == '\n') || (c == '\r') || (c == '\t');
    }

    inline bool isAlpha(const char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    inline bool isDigit(const char c) {
        return (c >= '0' && c <= '9');
    }

    inline void eatWhiteSpace(
            const std::string &code,
            uint64_t &index, uint64_t &line, uint64_t &col) {
        while(index < code.length() && isWhiteSpace(code[index])) {
            if(code[index] == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
            index++;
        }
    }

    inline bool dirExists(const std::string &name) {
#if defined(_WIN32) || defined(WIN32)
        const auto fileType = GetFileAttributesA(name.c_str());
        return fileType != INVALID_FILE_ATTRIBUTES
            && fileType & FILE_ATTRIBUTE_DIRECTORY;
#else
        struct stat st = { 0 };
        return stat(name.c_str(), &st) != -1;
#endif
    }

    inline bool createDirectory(const std::string &name) {
#if defined(_WIN32) || defined(WIN32)
        return mkdir(name.c_str()) == 0;
#else
        return mkdir(name.c_str(), 0700) == 0;
#endif
    }

    inline std::string getCurrentDir(void) {
        char buff[FILENAME_MAX];
        GetCurrentDir(buff, FILENAME_MAX);
        return std::string(buff);
    }
}
