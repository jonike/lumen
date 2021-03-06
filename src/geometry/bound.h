#ifndef LUMEN_BOUND_H
#define LUMEN_BOUND_H

#include "nex\matrix.h"
#include "nex\point.h"
#include "nex\ray.h"

namespace lumen {
class bound {
public:
        bound();
        bound(const nex::point& min, const nex::point& max);

        bool intersect(const nex::ray&) const;
        bool intersect(const nex::ray&, float* tmin, float* tmax) const;

        void transform(const nex::matrix&);

        void expand(const bound&);

        nex::point min;
        nex::point max;
};
}

#endif
