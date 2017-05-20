#include <algorithm>
#include <cassert>
#include <sampler.h>
#include <triangle.h>
#include "nex\util.h"

namespace lumen {
triangle::triangle(const nex::matrix& world, const bsdf_ptr& bsdf,
        const vertex& v0, const vertex& v1, const vertex& v2) :
        geometry(world, bsdf),
        normal(nex::cross(v1.position - v0.position, v2.position - v0.position)),
        area(2.0f / nex::magnitude(normal))
{
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;

        nex::normalize(normal);

        float minx = std::min(std::min(v0.position.x, v1.position.x), v2.position.x);
        float miny = std::min(std::min(v0.position.y, v1.position.y), v2.position.y);
        float minz = std::min(std::min(v0.position.z, v1.position.z), v2.position.z);

        float maxx = std::max(std::max(v0.position.x, v1.position.x), v2.position.x);
        float maxy = std::max(std::max(v0.position.y, v1.position.y), v2.position.y);
        float maxz = std::max(std::max(v0.position.z, v1.position.z), v2.position.z);

        set_bound(nex::point(minx, miny, minz), nex::point(maxx, maxy, maxz));
}

float triangle::pdf() const
{
        return area;
}

bool triangle::intersect(const nex::ray& ray, float* t, nex::vector* norm) const
{
        // compute the determinant and check for division by zero
        nex::vector b = vertices[1].position - vertices[0].position;
        nex::vector c = vertices[2].position - vertices[0].position;

        nex::vector d_cross_c = nex::cross(ray.direction, c);

        float det = nex::dot(b, d_cross_c);

        if (std::abs(det) < nex::EPSILON) {
                return false;
        }

        float one_over_det = 1.0f / det;

        // compute the u barycentric coordinate
        nex::vector a = ray.origin - vertices[0].position;

        float u = one_over_det * nex::dot(a, d_cross_c);

        if ((u < 0.0f) || (u > 1.0f)) {
                return false;
        }

        // compute the v barycentric coordinate
        nex::vector a_cross_b = nex::cross(a, b);

        float v = one_over_det * nex::dot(ray.direction, a_cross_b);

        if ((v < 0.0f) || ((u + v) > 1.0f)) {
                return false;
        }

        // compute the ray intersection parameter
        *t = one_over_det * nex::dot(c, a_cross_b);

        *norm = vertices[0].normal * (1.0f - u - v) + vertices[1].normal * u + vertices[2].normal * v;

        return true;
}

void triangle::get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const
{
        *pos = (1.0f - sample.x - sample.y) * vertices[0].position +
               sample.x * vertices[1].position +
               sample.y * vertices[2].position;

        *norm = (1.0f - sample.x - sample.y) * vertices[0].normal +
                sample.x * vertices[1].normal +
                sample.y * vertices[2].normal;
}
}
