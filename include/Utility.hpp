/*
 * Author: Dylan Turner
 * Description: Small functions for keeping code cleaner
 */

#pragma once

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
        while(isWhiteSpace(code[index])) {
            index++;
            if(code[index] == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
        }
    }
}
