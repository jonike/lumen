#ifndef LUMEN_MATERIALFACTORY_H
#define LUMEN_MATERIALFACTORY_H

#include <bsdf.h>
#include "parameter.h"

namespace lumen {
struct attributes;

bsdf_ptr create_material(const attributes& attr, const std::string& name, const parameter_list& params);
}

#endif
