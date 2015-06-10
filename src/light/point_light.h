#ifndef LUMEN_POINTLIGHT_H
#define LUMEN_POINTLIGHT_H

#include <light.h>

namespace lumen {
class point_light : public light {
public:
        point_light(const nex::color& intensity, const nex::point& position);

        virtual nex::color sample_light(const sample& sample, const surface& surface,
                nex::vector* wi, nex::ray* vray, float* pdf) const;
private:
        const nex::color intensity;
        const nex::point position;
};
}

#endif
