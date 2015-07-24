#ifndef LUMEN_MATERIALFACTORY_H
#define LUMEN_MATERIALFACTORY_H

#include <bsdf.h>

namespace lumen {
struct attributes;

bsdf_ptr create_material(const attributes& attr, const char* name,
                        int num, const char* tokens[], void* params[]);
}

#endif
