/*
 * Author: Dylan Turner
 * Description: Small functions for keeping code cleaner
 */

#pragma once

#include <sstream>

namespace nabd {
    inline void padStringStream(
            std::stringstream &ss, const uint32_t padding, char padC = ' ') {
        for(uint32_t i = 0; i < padding; i++) {
            ss << padC;
        }
    }
}
