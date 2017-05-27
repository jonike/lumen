#ifndef LUMEN_RECTANGLE_H
#define LUMEN_RECTANGLE_H

#include <geometry.h>

namespace lumen {
class rectangle : public geometry {
public:
        rectangle(const nex::matrix& world, const bsdf_ptr& bsdf);

        virtual float pdf() const;
private:
        virtual bool intersect(const nex::ray& ray, float* t, nex::vector* norm, nex::point* texcoord) const;
        virtual void get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const;

        const float half_width;
        const float half_height;
        const float inv_area;
        const nex::vector normal;
        const nex::point top_left;
        const nex::vector horizontal;
        const nex::vector vertical;
};
}

#endif
