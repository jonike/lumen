#include <texture_sampler.h>
#include <util.h>

namespace lumen {
static void get_texcoords_cube(int* face, float* u, float* v, float* w);

texture_sampler::texture_sampler(const texture_ptr& tex, address_mode u, address_mode v) :
        texture_(tex),
        address_mode_u(u),
        address_mode_v(v)
{
}

texture_sampler::~texture_sampler()
{
}

nex::color texture_sampler::sample(float u, float v) const
{
        int x = static_cast<int>(u * texture_->width());
        int y = static_cast<int>(v * texture_->height());

        return sample_2d(x, y, u, v);
}

nex::color texture_sampler::sample(float u, float v, float w) const
{
        int face;
        get_texcoords_cube(&face, &u, &v, &w);

        int x = static_cast<int>(u * texture_->width());
        int y = static_cast<int>(v * texture_->height());

        return sample_cube(face, x, y, u, v);
}

nex::color texture_sampler::fetch_2d(int x, int y) const
{
        int width = texture_->width();
        int height = texture_->height();

        switch (address_mode_u) {
        case AM_CLAMP:
                x = nex::clamp(x, 0, width - 1);
                break;
        case AM_WRAP:
                x = x % width;
                break;
        }

        switch (address_mode_v) {
        case AM_CLAMP:
                y = nex::clamp(y, 0, height - 1);
                break;
        case AM_WRAP:
                y = y % height;
                break;
        }

        return texture_->fetch_2d(x, y);
}

nex::color texture_sampler::fetch_cube(int face, int x, int y) const
{
        x = nex::clamp(x, 0, texture_->width() - 1);
        y = nex::clamp(y, 0, texture_->height() - 1);

        return texture_->fetch_cube(face, x, y);
}

void get_texcoords_cube(int* face, float* u, float* v, float* w)
{
        float tu = 0.0f;
        float tv = 0.0f;

        if ((abs(*u) >= abs(*v)) && (abs(*u) >= abs(*w))) {
                if (*u > 0.0f) {
                        *face = 0;
                        tu = 0.5f * *w / abs(*u) + 0.5f;
                        tv = 1.0f - (0.5f * *v / abs(*u) + 0.5f);
                } else {
                        *face = 1;
                        tu = 1.0f - (0.5f * *w / abs(*u) + 0.5f);
                        tv = 1.0f - (0.5f * *v / abs(*u) + 0.5f);
                }
        } else if ((abs(*v) >= abs(*u)) && (abs(*v) >= abs(*w))) {
                if (*v > 0.0f) {
                        *face = 2;
                        tu = 0.5f * *u / *v + 0.5f;
                        tv = 1.0f - (0.5f * *w / *v + 0.5f);
                } else {
                        *face = 3;
                        tu = 1.0f - (0.5f * *u / *v + 0.5f);
                        tv = 1.0f - (0.5f * *w / *v + 0.5f);
                }
        } else {
                if (*w > 0.0f) {
                        *face = 5;
                        tu = 1.0f - (0.5f * *u / *w + 0.5f);
                        tv = 1.0f - (0.5f * *v / *w + 0.5f);
                } else {
                        *face = 4;
                        tu = 1.0f - (0.5f * *u / *w + 0.5f);
                        tv = (0.5f * *v / *w + 0.5f);
                }
        }

        *u = tu;
        *v = tv;
}
}
