#ifndef LUMEN_FRESNELREFRACTIONBTDF_H
#define LUMEN_FRESNELREFRACTIONBTDF_H

#include <bxdf.h>

namespace lumen {
class fresnel_refraction_btdf : public bxdf {
public:
        fresnel_refraction_btdf(const nex::color& transmittance, float ni, float nt);

        virtual nex::color sample(const lumen::sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const;
        virtual nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const;

        virtual int type() const;
private:
        const nex::color transmittance;

        const float ni;
        const float nt;
};
}

#endif
