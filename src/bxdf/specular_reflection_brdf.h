#ifndef LUMEN_SPECULARREFLECTIONBRDF_H
#define LUMEN_SPECULARREFLECTIONBRDF_H

#include <bxdf.h>

namespace lumen {
class specular_reflection_brdf : public bxdf {
public:
        explicit specular_reflection_brdf(const nex::color& reflectance);

        virtual nex::color sample(const lumen::sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const;
        virtual nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const;

        virtual int type() const;
private:
        const nex::color reflectance;
};
}

#endif
