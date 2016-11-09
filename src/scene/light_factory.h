#ifndef LUMEN_LIGHTFACTORY_H
#define LUMEN_LIGHTFACTORY_H

#include <light.h>
#include "parameter.h"

namespace lumen {
light_ptr create_light(const std::string& name, const parameter_list& params);
}

#endif
