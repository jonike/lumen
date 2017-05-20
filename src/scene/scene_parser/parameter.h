#pragma once

#include <string>
#include <vector>
#include "nex\color.h"
#include "nex\point.h"
#include "nex\vector.h"

namespace lumen {
class parameter {
public:
        parameter();
        explicit parameter(const std::string&);
        explicit parameter(float);

        nex::color color() const;
        nex::point point() const;
        nex::vector vector() const;

        std::string name;

        float floatval;
        std::string strval;
        std::vector<float> arrayval;
};

typedef std::vector<parameter> parameter_list;
}
