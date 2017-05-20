#include <environment_light.h>
#include <geometry.h>
#include <sampler.h>
#include "nex\util.h"

namespace lumen {
environment_light::environment_light(const nex::color& radiance, const texture_sampler_ptr& envmap) :
        light(),
        radiance(radiance),
        envmap(envmap)
{
}

nex::color environment_light::sample_light(const sample& sample, const surface& surface,
        nex::vector* wi, nex::ray* vray, float* pdf) const
{
        *wi = sample_hemisphere(sample, surface.normal);

        *vray = nex::ray(surface.position, *wi);

        *pdf = nex::dot(surface.normal, *wi) * nex::ONE_OVER_PI;

        return envmap.get() ? envmap->sample(wi->x, wi->y, wi->z) : radiance;
}

nex::color environment_light::emission(const nex::vector& wi)
{
        return envmap.get() ? envmap->sample(wi.x, wi.y, wi.z) : radiance;
}
}
