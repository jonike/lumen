#include <cmath>
#include <fresnel_reflection_brdf.h>
#include <geometry.h>

namespace lumen {
fresnel_reflection_brdf::fresnel_reflection_brdf(const nex::color& reflectance, float ni, float nt) :
        bxdf(),
        reflectance(reflectance),
        ni(ni),
        nt(nt)
{
}

nex::color fresnel_reflection_brdf::sample(const lumen::sample& sample, const surface& surface,
        const nex::vector& wo, nex::vector* wi, float* pdf) const
{
        *wi = reflect(wo, surface.normal);
        *pdf = 1.0f;

        return fresnel(ni, nt, nex::dot(surface.normal, wo)) * reflectance /
                std::abs(nex::dot(surface.normal, *wi));
}

nex::color fresnel_reflection_brdf::evaluate(const surface& surface,
        const nex::vector& wo, const nex::vector& wi) const
{
        return nex::color::black();
}

int fresnel_reflection_brdf::type() const
{
        return BXDF_REFLECTION | BXDF_SPECULAR;
}
}
