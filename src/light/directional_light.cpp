#include <directional_light.h>
#include <geometry.h>

namespace lumen {
directional_light::directional_light(const nex::color& intensity, const nex::vector& direction) :
        light(),
        intensity(intensity),
        direction(-direction)
{
}

nex::color directional_light::sample_light(const sample& sample, const surface& surface,
        nex::vector* wi, nex::ray* vray, float* pdf) const
{
        *wi = direction;
        *vray = nex::ray(surface.position, *wi);
        *pdf = 1.0f;

        return intensity;
}
}
