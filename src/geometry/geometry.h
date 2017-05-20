#ifndef LUMEN_GEOMETRY_H
#define LUMEN_GEOMETRY_H

#include <bound.h>
#include <bsdf.h>
#include "nex\matrix.h"
#include "nex\ray.h"

namespace lumen {
struct sample;

struct surface {
        nex::point position;
        nex::vector normal;
        nex::point texcoord;
        const bsdf* bsdf;
};

class geometry {
public:
        geometry(const nex::matrix& world, const bsdf_ptr& bsdf);
        virtual ~geometry() {};

        bool intersect(nex::ray* ray, surface* surface) const;
        bool intersect(nex::ray* ray) const;

        void sample_surface(const sample& sample, nex::point* pos, nex::vector* norm) const;
        virtual float pdf() const = 0;

        const bound& world_bound() const;
protected:
        void set_bound(const nex::point& min, const nex::point& max);
private:
        virtual bool intersect(const nex::ray& ray, float* t, nex::vector* norm) const = 0;
        virtual bool intersect_shadow(const nex::ray& ray, float* t) const;
        virtual void get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const = 0;

        const nex::matrix world;
        const nex::matrix world_inverse;
        const nex::matrix normal_mat;
        bound world_bound_;
        const bsdf_ptr bsdf;
};

typedef std::shared_ptr<geometry> geometry_ptr;
}

#endif
