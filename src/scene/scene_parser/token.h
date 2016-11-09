#pragma once

#include <string>
#include "parameter.h"

namespace lumen {
class token {
public:
    float floatval;
    std::string strval;
    parameter paramval;
};
}

#define YYSTYPE lumen::token
