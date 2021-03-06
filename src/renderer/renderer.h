#ifndef LUMEN_RENDERER_H
#define LUMEN_RENDERER_H

#include <acceleration_structure.h>
#include <light.h>
#include <vector>
#include "nex\color.h"
#include "nex\ray.h"

namespace lumen {
class sampler;

class renderer {
public:
        renderer(const acceleration_structure_ptr& acceleration,
                const std::vector<light_ptr>& lights);
        virtual ~renderer();

        virtual nex::color radiance(const sampler*, nex::ray*) const = 0;
protected:
        const acceleration_structure_ptr acceleration;
        const std::vector<light_ptr> lights;
};
}

#endif
