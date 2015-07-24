#include <bilinear_sampler.h>
#include <bsdf.h>
#include <diffuse_brdf.h>
#include <material_factory.h>
#include <point_sampler.h>
#include <render_context.h>
#include <scene.h>
#include <stdexcept>
#include <texture_cache.h>

namespace lumen {
static bsdf_ptr create_matte(const attributes& attr, int num, const char* tokens[], void* params[]);

bsdf_ptr create_material(const attributes& attr, const char* name,
                        int num, const char* tokens[], void* params[])
{
        if (strcmp(MATTE, name) == 0) {
                return create_matte(attr, num, tokens, params);
        } else {
                throw std::invalid_argument("invalid material " + std::string(name));
        }
}

static bsdf_ptr create_matte(const attributes& attr, int num, const char* tokens[], void* params[])
{
        float color[3] = {1.0f, 1.0f, 1.0f};
        texture_sampler_ptr sampler;

        for (int i = 0; i < num; ++i) {
                if (strcmp(TEXTURENAME, tokens[i]) == 0) {
                        sampler.reset(new point_sampler(
                                texture_cache::load(reinterpret_cast<char*>(params[i])),
                                AM_WRAP, AM_WRAP));
                } else if (strcmp(DIFFUSECOLOR, tokens[i]) == 0) {
                        memcpy(color, reinterpret_cast<float*>(params[i]), sizeof(color));
                }
        }

        bsdf* matte = new bsdf();
        matte->add_bxdf(bxdf_ptr(new diffuse_brdf(nex::color(color[0], color[1], color[2]), sampler)));

        return bsdf_ptr(matte);
}
}
