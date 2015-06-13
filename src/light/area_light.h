#ifndef LUMEN_AREALIGHT_H
#define LUMEN_AREALIGHT_H

#include <geometry.h>
#include <light.h>

namespace lumen {
class area_light : public light {
public:
        area_light(const nex::color& radiance, const geometry_ptr& emitter);

        virtual nex::color sample_light(const sample& sample, const surface& surface,
                nex::vector* wi, nex::ray* vray, float* pdf) const;
private:
        const nex::color radiance;
        const geometry_ptr emitter;
};
}

#endif
