#ifndef LUMEN_GEOMETRYFACTORY_H
#define LUMEN_GEOMETRYFACTORY_H

#include <geometry.h>
#include "parameter.h"

namespace lumen {
struct attributes;

geometry_ptr create_geometry(const attributes& attr, const std::string& name, const parameter_list& params);
}

#endif
