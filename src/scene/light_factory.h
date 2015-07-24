#ifndef LUMEN_LIGHTFACTORY_H
#define LUMEN_LIGHTFACTORY_H

#include <light.h>

namespace lumen {
light_ptr create_light(const char* name, int num, const char* tokens[], void* params[]);
}

#endif
