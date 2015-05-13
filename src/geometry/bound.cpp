#include <algorithm>
#include <bound.h>
#include <ray.h>

namespace lumen {
bound::bound() :
        min(FLT_MAX, FLT_MAX, FLT_MAX),
        max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{
}

bound::bound(const nex::point& min, const nex::point& max) :
        min(std::min(min.x, max.x), std::min(min.y, max.y), std::min(min.z, max.z)),
        max(std::max(min.x, max.x), std::max(min.y, max.y), std::max(min.z, max.z))
{
}

bool bound::intersect(const nex::ray& ray) const
{
        float tmin, tmax;
        return intersect(ray, &tmin, &tmax);
}

bool bound::intersect(const nex::ray& ray, float* tmin, float* tmax) const
{
        // calculate the ray-plane intersection of the x, y, and z slabs
        float inv_d = 1.0f / ray.direction.x;

        float t0 = (min.x - ray.origin.x) * inv_d;
        float t1 = (max.x - ray.origin.x) * inv_d;

        *tmin = std::min(t0, t1);
        *tmax = std::max(t0, t1);

        inv_d = 1.0f / ray.direction.y;

        t0 = (min.y - ray.origin.y) * inv_d;
        t1 = (max.y - ray.origin.y) * inv_d;

        *tmin = std::max(*tmin, std::min(t0, t1));
        *tmax = std::min(*tmax, std::max(t0, t1));

        inv_d = 1.0f / ray.direction.z;

        t0 = (min.z - ray.origin.z) * inv_d;
        t1 = (max.z - ray.origin.z) * inv_d;

        *tmin = std::max(*tmin, std::min(t0, t1));
        *tmax = std::min(*tmax, std::max(t0, t1));

        return (*tmax >= std::max(*tmin, 0.0f)) && (*tmin < ray.max);
}

void bound::transform(const nex::matrix& mat)
{
        nex::point points[8];

        points[0] = nex::point(min.x, max.y, min.z);
        points[1] = nex::point(max.x, max.y, min.z);
        points[2] = nex::point(max.x, min.y, min.z);
        points[3] = nex::point(min.x, max.y, max.z);
        points[4] = nex::point(min.x, min.y, max.z);
        points[5] = nex::point(max.x, min.y, max.z);
        points[6] = min;
        points[7] = max;

        float x[8];
        float y[8];
        float z[8];

        for (int i = 0; i < 8; ++i) {
                points[i] = points[i] * mat;

                x[i] = points[i].x;
                y[i] = points[i].y;
                z[i] = points[i].z;
        }

        std::sort(x, x + 8);
        std::sort(y, y + 8);
        std::sort(z, z + 8);

        min = nex::point(x[0], y[0], z[0]);
        max = nex::point(x[7], y[7], z[7]);
}

void bound::expand(const bound& bound)
{
        const nex::point& p0 = bound.min;
        const nex::point& p1 = bound.max;

        min.x = std::min(min.x, std::min(p0.x, p1.x));
        min.y = std::min(min.y, std::min(p0.y, p1.y));
        min.z = std::min(min.z, std::min(p0.z, p1.z));

        max.x = std::max(max.x, std::max(p0.x, p1.x));
        max.y = std::max(max.y, std::max(p0.y, p1.y));
        max.z = std::max(max.z, std::max(p0.z, p1.z));
}
}
