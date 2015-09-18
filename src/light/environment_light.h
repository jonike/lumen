#ifndef LUMEN_ENVIRONMENTLIGHT_H
#define LUMEN_ENVIRONMENTLIGHT_H

#include <light.h>
#include <texture_sampler.h>

namespace lumen {
class environment_light : public light {
public:
        environment_light(const nex::color& radiance, const texture_sampler_ptr& envmap);

        virtual nex::color sample_light(const sample& sample, const surface& surface,
                nex::vector* wi, nex::ray* vray, float* pdf) const;

        virtual nex::color emission(const nex::vector& wi);
private:
        const nex::color radiance;
        texture_sampler_ptr envmap;
};
}

#endif
