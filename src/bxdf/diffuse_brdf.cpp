#include <diffuse_brdf.h>
#include <geometry.h>
#include <sampler.h>
#include <util.h>

namespace lumen {
diffuse_brdf::diffuse_brdf(const nex::color& reflectance, const texture_sampler_ptr& diffuse_map) :
        bxdf(),
        reflectance(reflectance * nex::ONE_OVER_PI),
        diffuse_map(diffuse_map)
{
}

nex::color diffuse_brdf::sample(const lumen::sample& sample, const surface& surface,
        const nex::vector& wo, nex::vector* wi, float* pdf) const
{
        *wi = sample_hemisphere(sample, surface.normal);
        *pdf = nex::dot(surface.normal, *wi) * nex::ONE_OVER_PI;

        return evaluate(surface, wo, *wi);
}

nex::color diffuse_brdf::evaluate(const surface& surface,
        const nex::vector& wo, const nex::vector& wi) const
{
        nex::color kd = diffuse_map.get()
                ? diffuse_map->sample(surface.texcoord.x, surface.texcoord.y)
                : nex::color::white();

        return kd * reflectance;
}

int diffuse_brdf::type() const
{
        return BXDF_REFLECTION | BXDF_DIFFUSE;
}
}
