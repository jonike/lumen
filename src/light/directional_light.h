#ifndef LUMEN_DIRECTIONALLIGHT_H
#define LUMEN_DIRECTIONALLIGHT_H

#include <light.h>

namespace lumen {
class directional_light : public light {
public:
        directional_light(const nex::color& intensity, const nex::vector& direction);

        virtual nex::color sample_light(const sample& sample, const surface& surface,
                nex::vector* wi, nex::ray* vray, float* pdf) const;
private:
        const nex::color intensity;
        const nex::vector direction;
};
}

#endif
