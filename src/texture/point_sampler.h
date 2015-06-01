#ifndef LUMEN_POINTSAMPLER_H
#define LUMEN_POINTSAMPLER_H

#include <texture_sampler.h>

namespace lumen {
class point_sampler : public texture_sampler {
public:
        explicit point_sampler(const texture_ptr&, address_mode u, address_mode v);
private:
        virtual nex::color sample_2d(int x, int y, float u, float v) const;
        virtual nex::color sample_cube(int face, int x, int y, float u, float v) const;
};
}

#endif
