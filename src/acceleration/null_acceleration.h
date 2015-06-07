#ifndef LUMEN_NULLACCELERATION_H
#define LUMEN_NULLACCELERATION_H

#include <acceleration_structure.h>
#include <geometry.h>
#include <vector>

namespace lumen {
class null_acceleration : public acceleration_structure {
public:
        explicit null_acceleration(const std::vector<geometry_ptr>& geometry);
private:
        virtual bool intersect_geometry(nex::ray*, surface*) const;
        virtual bool intersect_shadow(nex::ray*) const;

        const std::vector<geometry_ptr> geometry;
};
}

#endif
