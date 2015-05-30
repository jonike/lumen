#ifndef LUMEN_SPHERE_H
#define LUMEN_SPHERE_H

#include <geometry.h>

namespace lumen {
class sphere : public geometry {
public:
        sphere(const nex::matrix& world, const bsdf_ptr& bsdf);

        virtual float pdf() const;
private:
        virtual bool intersect(const nex::ray& ray, float* t, nex::vector* norm) const;
        virtual void get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const;

        const float inv_area;
};
}

#endif
