#include <cmath>
#include <geometry.h>
#include <specular_reflection_brdf.h>

namespace lumen {
specular_reflection_brdf::specular_reflection_brdf(const nex::color& reflectance) :
        bxdf(),
        reflectance(reflectance)
{
}

nex::color specular_reflection_brdf::sample(const lumen::sample& sample, const surface& surface,
        const nex::vector& wo, nex::vector* wi, float* pdf) const
{
        *wi = reflect(wo, surface.normal);
        *pdf = 1.0f;

        return reflectance / std::abs(nex::dot(surface.normal, *wi));
}

nex::color specular_reflection_brdf::evaluate(const surface& surface,
        const nex::vector& wo, const nex::vector& wi) const
{
        return nex::color::black();
}

int specular_reflection_brdf::type() const
{
        return BXDF_REFLECTION | BXDF_SPECULAR;
}
}
