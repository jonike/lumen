#ifndef LUMEN_PLANE_H
#define LUMEN_PLANE_H

#include <geometry.h>

namespace lumen {
class plane : public geometry {
public:
        plane(const nex::matrix& world, const bsdf_ptr& bsdf);

        virtual float pdf() const;
private:
        virtual bool intersect(const nex::ray& ray, float* t, nex::vector* norm, nex::point* texcoord) const;
        virtual void get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const;

        const nex::vector normal;
};
}

#endif
