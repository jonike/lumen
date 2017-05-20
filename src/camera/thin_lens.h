#ifndef LUMEN_THINLENS_H
#define LUMEN_THINLENS_H

#include <camera.h>
#include "nex\matrix.h"
#include "nex\ray.h"

namespace lumen {
class thin_lens : public camera {
public:
        thin_lens(const nex::matrix& view, const nex::matrix& projection,
                int hres, int vres, float lens_radius, float focal_distance);

        virtual nex::ray generate_ray(float x, float y, const sample& s) const;
private:
        void create_view_ray(float x, float y, nex::point* origin, nex::vector* direction) const;
        void depth_of_field(const sample& s, nex::point* origin, nex::vector* direction) const;

        const nex::matrix view;
        const nex::matrix projection;

        const int hres;
        const int vres;

        const float lens_radius;
        const float focal_distance;
};
}

#endif
