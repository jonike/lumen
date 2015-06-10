#include <geometry.h>
#include <point_light.h>
#include <util.h>

namespace lumen {
point_light::point_light(const nex::color& intensity, const nex::point& position) :
        light(),
        intensity(intensity),
        position(position)
{
}

nex::color point_light::sample_light(const sample& sample, const surface& surface,
        nex::vector* wi, nex::ray* vray, float* pdf) const
{
        *wi = position - surface.position;
        float distance = nex::magnitude(*wi);
        *wi /= distance;

        *vray = nex::ray(surface.position, *wi, distance - nex::EPSILON);

        *pdf = 1.0f;

        return intensity / (distance * distance);
}
}
