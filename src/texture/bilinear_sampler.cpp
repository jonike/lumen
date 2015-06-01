#include <bilinear_sampler.h>

namespace lumen {
static nex::color bilinear_interpolate(const nex::color& s0, const nex::color& s1,
        const nex::color& s2, const nex::color& s3, float u, float v);

bilinear_sampler::bilinear_sampler(const texture_ptr& tex, address_mode u, address_mode v) :
        texture_sampler(tex, u, v)
{
}

nex::color bilinear_sampler::sample_2d(int x, int y, float u, float v) const
{
        nex::color s0 = fetch_2d(x, y);
        nex::color s1 = fetch_2d(x + 1, y);
        nex::color s2 = fetch_2d(x, y + 1);
        nex::color s3 = fetch_2d(x + 1, y + 1);

        return bilinear_interpolate(s0, s1, s2, s3, u, v);
}

nex::color bilinear_sampler::sample_cube(int face, int x, int y, float u, float v) const
{
        nex::color s0 = fetch_cube(face, x, y);
        nex::color s1 = fetch_cube(face, x + 1, y);
        nex::color s2 = fetch_cube(face, x, y + 1);
        nex::color s3 = fetch_cube(face, x + 1, y + 1);

        return bilinear_interpolate(s0, s1, s2, s3, u, v);
}

static nex::color bilinear_interpolate(const nex::color& s0, const nex::color& s1,
        const nex::color& s2, const nex::color& s3, float u, float v)
{
        // calculate the interpolation amounts
        float null;
        float tu = modf(u, &null);
        float tv = modf(v, &null);

        // linearly interpolate along the top and bottom edges
        nex::color x0 = tu * s1 + (1 - tu) * s0;
        nex::color x1 = tu * s3 + (1 - tu) * s2;

        // linearly interpolate between the resulting samples
        return tv * x1 + (1 - tv) * x0;
}
}
