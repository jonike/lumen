#include <acceleration_structure.h>

namespace lumen {
acceleration_structure::acceleration_structure(const std::vector<geometry_ptr>& geometry) :
        world_bound()
{
        for (const geometry_ptr& geo : geometry) {
                world_bound.expand(geo->world_bound());
        }
}

acceleration_structure::~acceleration_structure()
{
}

bool acceleration_structure::intersect(nex::ray* ray, surface* surface) const
{
        if (!world_bound.intersect(*ray)) {
                return false;
        }

        return intersect_geometry(ray, surface);
}

bool acceleration_structure::intersect(nex::ray* ray) const
{
        if (!world_bound.intersect(*ray)) {
                return false;
        }

        return intersect_shadow(ray);
}
}
