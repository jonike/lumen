#include <rectangle.h>
#include <sampler.h>

namespace lumen {
static const float WIDTH = 1.0f;
static const float HEIGHT = 1.0f;

rectangle::rectangle(const nex::matrix& world, const bsdf_ptr& bsdf) :
        geometry(world, bsdf),
        half_width(WIDTH / 2.0f),
        half_height(HEIGHT / 2.0f),
        inv_area(1.0f / (WIDTH * HEIGHT)),
        normal(0.0f, 1.0f, 0.0f),
        top_left(-half_width, 0.0f, half_height),
        horizontal(WIDTH, 0.0f, 0.0f),
        vertical(0.0f, 0.0f, -HEIGHT)
{
        set_bound(nex::point(-half_width, 0.0f, -half_height),
                nex::point(half_width, 0.0f, half_height));
}

float rectangle::pdf() const
{
        return inv_area;
}

bool rectangle::intersect(const nex::ray& ray, float* t, nex::vector* norm) const
{
        *t = -ray.origin.y / ray.direction.y;
        *norm = normal;

        nex::point p = ray(*t);

        return (std::abs(p.x) <= half_width) && (std::abs(p.z) <= half_height);
}

void rectangle::get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const
{
        *pos = top_left + horizontal * sample.x + vertical * sample.y;
        *norm = normal;
}
}
