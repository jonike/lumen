#include <null_acceleration.h>

namespace lumen {
null_acceleration::null_acceleration(const std::vector<geometry_ptr>& geometry) :
        acceleration_structure(geometry),
        geometry(geometry)
{
}

bool null_acceleration::intersect_geometry(nex::ray* ray, surface* surface) const
{
        bool intersects = false;

        for (const geometry_ptr& geo : geometry) {
                if (geo->intersect(ray, surface)) {
                        intersects = true;
                }
        }

        return intersects;
}

bool null_acceleration::intersect_shadow(nex::ray* ray) const
{
        for (const geometry_ptr& geo : geometry) {
                if (geo->intersect(ray)) {
                        return true;
                }
        }

        return false;
}
}
