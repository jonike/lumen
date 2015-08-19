#ifndef LUMEN_FRESNELREFLECTIONBRDF_H
#define LUMEN_FRESNELREFLECTIONBRDF_H

#include <bxdf.h>

namespace lumen {
class fresnel_reflection_brdf : public bxdf {
public:
        fresnel_reflection_brdf(const nex::color& reflectance, float ni, float nt);

        virtual nex::color sample(const lumen::sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const;
        virtual nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const;

        virtual int type() const;
private:
        const nex::color reflectance;

        const float ni;
        const float nt;
};
}

#endif
