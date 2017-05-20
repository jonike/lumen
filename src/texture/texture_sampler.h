#ifndef LUMEN_TEXTURESAMPLER_H
#define LUMEN_TEXTURESAMPLER_H

#include <texture.h>
#include "nex\color.h"

namespace lumen {
enum address_mode {
        AM_CLAMP,
        AM_WRAP,
};

class texture_sampler {
public:
        texture_sampler(const texture_ptr&, address_mode u, address_mode v);
        virtual ~texture_sampler();

        nex::color sample(float u, float v) const;
        nex::color sample(float u, float v, float w) const;
protected:
        nex::color fetch_2d(int x, int y) const;
        nex::color fetch_cube(int face, int x, int y) const;

        int texture_width() const;
        int texture_height() const;
private:
        virtual nex::color sample_2d(int x, int y, float u, float v) const = 0;
        virtual nex::color sample_cube(int face, int x, int y, float u, float v) const = 0;

        texture_ptr texture_;
        const address_mode address_mode_u;
        const address_mode address_mode_v;
};

typedef std::shared_ptr<texture_sampler> texture_sampler_ptr;
}

#endif
