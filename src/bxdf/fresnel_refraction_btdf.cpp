#include <cmath>
#include <fresnel_refraction_btdf.h>
#include <geometry.h>

namespace lumen {
fresnel_refraction_btdf::fresnel_refraction_btdf(const nex::color& transmittance, float ni, float nt) :
        bxdf(),
        transmittance(transmittance),
        ni(ni),
        nt(nt)
{
}

nex::color fresnel_refraction_btdf::sample(const lumen::sample& sample, const surface& surface,
        const nex::vector& wo, nex::vector* wi, float* pdf) const
{
        float eta = ni / nt;

        *wi = refract(wo, surface.normal, &eta);
        *pdf = 1.0f;

        // check for total internal reflection
        if (*wi == nex::vector(0.0f, 0.0f, 0.0f)) {
                return nex::color::black();
        }

        return (1.0f - fresnel(ni, nt, nex::dot(surface.normal, wo))) *
               transmittance / (eta * eta) / std::abs(nex::dot(surface.normal, *wi));
}

nex::color fresnel_refraction_btdf::evaluate(const surface& surface,
        const nex::vector& wo, const nex::vector& wi) const
{
        return nex::color::black();
}

int fresnel_refraction_btdf::type() const
{
        return BXDF_TRANSMISSION | BXDF_SPECULAR;
}
}
