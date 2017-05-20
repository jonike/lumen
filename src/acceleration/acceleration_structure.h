#ifndef LUMEN_ACCELERATIONSTRUCTURE_H
#define LUMEN_ACCELERATIONSTRUCTURE_H

#include <bound.h>
#include <geometry.h>
#include <memory>
#include <vector>
#include "nex\ray.h"

namespace lumen {
class acceleration_structure {
public:
        acceleration_structure(const std::vector<geometry_ptr>& geometry);
        virtual ~acceleration_structure();

        bool intersect(nex::ray*, surface*) const;
        bool intersect(nex::ray*) const;
protected:
        bound get_world_bound() const;
private:
        virtual bool intersect_geometry(nex::ray*, surface*) const = 0;
        virtual bool intersect_shadow(nex::ray*) const = 0;

        bound world_bound;
};

typedef std::shared_ptr<acceleration_structure> acceleration_structure_ptr;
}

#endif
