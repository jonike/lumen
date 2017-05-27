#include <cassert>
#include <plane.h>
#include <sampler.h>

namespace lumen {
static const float PLANE_WIDTH = 512.0f;

plane::plane(const nex::matrix& world, const bsdf_ptr& bsdf) :
        geometry(world, bsdf),
        normal(0.0f, 1.0f, 0.0f)
{
        set_bound(nex::point(-PLANE_WIDTH, 0.0f, -PLANE_WIDTH),
                nex::point(PLANE_WIDTH, 0.0f, PLANE_WIDTH));
}

float plane::pdf() const
{
        assert(!"NOT IMPLEMENTED");
        return 0.0f;
}

bool plane::intersect(const nex::ray& ray, float* t, nex::vector* norm, nex::point* texcoord) const
{
        *t = -ray.origin.y / ray.direction.y;
        *norm = normal;

        return true;
}

void plane::get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const
{
        assert(!"NOT IMPLEMENTED");
}
}
