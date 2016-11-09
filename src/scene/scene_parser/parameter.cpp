#include "parameter.h"

namespace lumen {
parameter::parameter() :
        name(),
        floatval(0.0f),
        strval(),
        arrayval()
{
}

parameter::parameter(const std::string& s) :
        name(),
        floatval(0.0f),
        strval(s),
        arrayval()
{
}

parameter::parameter(float f) :
        name(),
        floatval(f),
        strval(),
        arrayval()
{
}

nex::color parameter::color() const
{
        if (arrayval.size() == 3) {
                return nex::color(arrayval[0], arrayval[1], arrayval[2]);
        } else {
                throw std::invalid_argument("invalid color parameter \"" + name + "\"");
        }
}

nex::point parameter::point() const
{
        if (arrayval.size() == 3) {
                return nex::point(arrayval[0], arrayval[1], arrayval[2]);
        } else {
                throw std::invalid_argument("invalid point parameter \"" + name + "\"");
        }
}

nex::vector parameter::vector() const
{
        if (arrayval.size() == 3) {
                return nex::vector(arrayval[0], arrayval[1], arrayval[2]);
        } else {
                throw std::invalid_argument("invalid vector parameter \"" + name + "\"");
        }
}
}
