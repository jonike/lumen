#ifndef LUMEN_TRIANGLE_H
#define LUMEN_TRIANGLE_H

#include <geometry.h>

namespace lumen {
struct vertex {
        nex::point position;
        nex::vector texcoord;
        nex::vector normal;
        nex::vector tangent;
};

class triangle : public geometry {
public:
        triangle(const nex::matrix& world, const bsdf_ptr& bsdf,
                const vertex& v0, const vertex& v1, const vertex& v2);

        virtual float pdf() const;
private:
        virtual bool intersect(const nex::ray& ray, float* t, nex::vector* norm) const;
        virtual void get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const;

        vertex vertices[3];
        nex::vector normal;
        const float area;
};
}

#endif
