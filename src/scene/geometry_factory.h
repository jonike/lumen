#ifndef LUMEN_GEOMETRYFACTORY_H
#define LUMEN_GEOMETRYFACTORY_H

#include <geometry.h>

namespace lumen {
struct attributes;

geometry_ptr create_geometry(const attributes& attr, const char* name,
                                int num, const char* tokens[], void* params[]);
}

#endif
