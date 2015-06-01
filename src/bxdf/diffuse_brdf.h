#ifndef LUMEN_DIFFUSEBRDF_H
#define LUMEN_DIFFUSEBRDF_H

#include <bxdf.h>
#include <texture_sampler.h>

namespace lumen {
class diffuse_brdf : public bxdf {
public:
        diffuse_brdf(const nex::color& reflectance, const texture_sampler_ptr& diffuse_map);

        virtual nex::color sample(const lumen::sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const;
        virtual nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const;

        virtual int type() const;
private:
        const nex::color reflectance;
        texture_sampler_ptr diffuse_map;
};
}

#endif
