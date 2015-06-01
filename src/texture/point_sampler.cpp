#include <point_sampler.h>

namespace lumen {
point_sampler::point_sampler(const texture_ptr& tex, address_mode u, address_mode v) :
        texture_sampler(tex, u, v)
{
}

nex::color point_sampler::sample_2d(int x, int y, float u, float v) const
{
        return fetch_2d(x, y);
}

nex::color point_sampler::sample_cube(int face, int x, int y, float u, float v) const
{
        return fetch_cube(face, x, y);
}
}
