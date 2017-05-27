#ifndef LUMEN_MESH_H
#define LUMEN_MESH_H

#include <acceleration_structure.h>
#include <geometry.h>
#include <string>

namespace lumen {
class mesh : public geometry {
public:
        mesh(const nex::matrix& world, const bsdf_ptr& bsdf, const std::string& filename);

        virtual float pdf() const;
private:
        virtual bool intersect(const nex::ray& ray, float* t, nex::vector* norm, nex::point* texcoord) const;
        virtual void get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const;

        acceleration_structure_ptr acceleration_;
};
}

#endif
