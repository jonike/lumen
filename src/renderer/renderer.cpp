#include <renderer.h>

namespace lumen {
renderer::renderer(const acceleration_structure_ptr& acceleration,
                const std::vector<light_ptr>& lights) :
        acceleration(acceleration),
        lights(lights)
{
}

renderer::~renderer()
{
}
}
