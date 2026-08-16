#include <string>

#include "Package.h"

std::string to_string(const Architecture &architecture) {
    switch(architecture) {
        case Architecture::x86: return "x86";
    }
}