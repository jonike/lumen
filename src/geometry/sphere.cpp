#include <cassert>
#include <cmath>
#include <sphere.h>
#include "nex\util.h"

namespace lumen {
static const float RADIUS = 1.0f;

sphere::sphere(const nex::matrix& world, const bsdf_ptr& bsdf) :
        geometry(world, bsdf),
        inv_area(0.25f * nex::ONE_OVER_PI / (RADIUS * RADIUS))
{
        set_bound(nex::point(-RADIUS, -RADIUS, -RADIUS),
                nex::point(RADIUS, RADIUS, RADIUS));
}

float sphere::pdf() const
{
        return inv_area;
}

bool sphere::intersect(const nex::ray& ray, float* t, nex::vector* norm) const
{
        nex::vector origin = nex::vector(ray.origin);

        float a = nex::dot(ray.direction, ray.direction);
        float b = 2.0f * nex::dot(origin, ray.direction);
        float c = nex::dot(origin, origin) - RADIUS * RADIUS;

        float disc = b * b - 4.0f * a * c;

        if (disc < 0.0f) {
                return false;
        }

        float denom = 1.0f / (2.0f * a);
        disc = std::sqrt(disc);

        *t = (-b - disc) * denom;

        if (*t > 0.0f) {
                *norm = nex::vector(ray(*t));
                return true;
        }

        *t = (-b + disc) * denom;

        if (*t > 0.0f) {
                *norm = nex::vector(ray(*t));
                return true;
        }

        return false;
}

void sphere::get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const
{
        assert(!"NOT IMPLEMENTED");
}
}
