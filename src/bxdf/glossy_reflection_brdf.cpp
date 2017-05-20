#include <cmath>
#include <geometry.h>
#include <glossy_reflection_brdf.h>
#include <sampler.h>
#include "nex\util.h"

namespace lumen {
glossy_reflection_brdf::glossy_reflection_brdf(const nex::color& reflectance, float exponent) :
        bxdf(),
        reflectance(reflectance),
        phong_exponent(exponent)
{
}

nex::color glossy_reflection_brdf::sample(const lumen::sample& sample, const surface& surface,
        const nex::vector& wo, nex::vector* wi, float* pdf) const
{
        nex::vector reflection = reflect(wo, surface.normal);

        *wi = sample_phong_lobe(sample, reflection, phong_exponent);

        // reflect the direction if it ends up pointing inside the surface
        if (nex::dot(*wi, surface.normal) < 0.0f) {
                *wi = reflect(*wi, reflection);
        }

        *pdf = (phong_exponent + 1.0f) / nex::TWO_PI *
                std::pow(nex::dot(reflection, *wi), phong_exponent);

        return evaluate(surface, wo, *wi);
}

nex::color glossy_reflection_brdf::evaluate(const surface& surface,
        const nex::vector& wo, const nex::vector& wi) const
{
        nex::vector reflection = reflect(wo, surface.normal);

        return reflectance * (phong_exponent + 2.0f) / nex::TWO_PI *
               std::pow(nex::dot(reflection, wi), phong_exponent);
}

int glossy_reflection_brdf::type() const
{
        return BXDF_REFLECTION | BXDF_GLOSSY;
}
}
