#ifndef LUMEN_GLOSSYREFLECTIONBRDF_H
#define LUMEN_GLOSSYREFLECTIONBRDF_H

#include <bxdf.h>

namespace lumen {
class glossy_reflection_brdf : public bxdf {
public:
        glossy_reflection_brdf(const nex::color& reflectance, float exponent);
		
        virtual nex::color sample(const lumen::sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const;
        virtual nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const;

        virtual int type() const;
private:
        const nex::color reflectance;
        const float phong_exponent;
};
}

#endif
