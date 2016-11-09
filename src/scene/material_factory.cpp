#include <bilinear_sampler.h>
#include <bsdf.h>
#include <diffuse_brdf.h>
#include <fresnel_reflection_brdf.h>
#include <fresnel_refraction_btdf.h>
#include <glossy_reflection_brdf.h>
#include <material_factory.h>
#include <point_sampler.h>
#include <render_context.h>
#include <scene.h>
#include <specular_reflection_brdf.h>
#include <stdexcept>
#include <texture_cache.h>

namespace lumen {
static bsdf_ptr create_matte(const attributes& attr, const parameter_list& params);
static bsdf_ptr create_chrome(const attributes& attr, const parameter_list& params);
static bsdf_ptr create_plastic(const attributes& attr, const parameter_list& params);
static bsdf_ptr create_glass(const attributes& attr, const parameter_list& params);

bsdf_ptr create_material(const attributes& attr, const std::string& name, const parameter_list& params)
{
        if (name == MATTE) {
                return create_matte(attr, params);
        } else if (name == CHROME) {
                return create_chrome(attr, params);
        } else if (name == PLASTIC) {
                return create_plastic(attr, params);
        } else if (name == GLASS) {
                return create_glass(attr, params);
        } else {
                throw std::invalid_argument("invalid material: " + name);
        }
}

static bsdf_ptr create_matte(const attributes& attr, const parameter_list& params)
{
        nex::color diffuse = nex::color::white();
        texture_sampler_ptr sampler;

        for (const parameter& p : params) {
                if (p.name == TEXTURENAME) {
                        sampler.reset(new point_sampler(texture_cache::load(p.strval), AM_WRAP, AM_WRAP));
                } else if (p.name == DIFFUSECOLOR) {
                        diffuse = p.color();
                }
        }

        bsdf* matte = new bsdf();
        matte->add_bxdf(bxdf_ptr(new diffuse_brdf(diffuse, sampler)));

        return bsdf_ptr(matte);
}

static bsdf_ptr create_chrome(const attributes& attr, const parameter_list& params)
{
        nex::color specular = nex::color::white();

        for (const parameter& p : params) {
                if (p.name == SPECULARCOLOR) {
                        specular = p.color();
                }
        }

        bsdf* chrome = new bsdf();
        chrome->add_bxdf(bxdf_ptr(new specular_reflection_brdf(specular)));

        return bsdf_ptr(chrome);
}

static bsdf_ptr create_plastic(const attributes& attr, const parameter_list& params)
{
        nex::color diffuse(0.5f, 0.5f, 0.5f);
        nex::color specular(0.5f, 0.5f, 0.5f);
        float exponent = 32.0f;
        texture_sampler_ptr sampler;

        for (const parameter& p : params) {
                if (p.name == TEXTURENAME) {
                        sampler.reset(new point_sampler(texture_cache::load(p.strval), AM_WRAP, AM_WRAP));
                } else if (p.name == DIFFUSECOLOR) {
                        diffuse = p.color();
                } else if (p.name == SPECULARCOLOR) {
                        specular = p.color();
                } else if (p.name == EXPONENT) {
                        exponent = p.floatval;
                }
        }

        bsdf* plastic = new bsdf();
        plastic->add_bxdf(bxdf_ptr(new diffuse_brdf(diffuse, sampler)));
        plastic->add_bxdf(bxdf_ptr(new glossy_reflection_brdf(specular, exponent)));

        return bsdf_ptr(plastic);
}

static bsdf_ptr create_glass(const attributes& attr, const parameter_list& params)
{
        nex::color specular = nex::color::white();
        float ni = 1.0f;
        float nt = 1.5f;

        for (const parameter& p : params) {
                if (p.name == SPECULARCOLOR) {
                        specular = p.color();
                } else if (p.name == NI) {
                        ni = p.floatval;
                } else if (p.name == NT) {
                        nt = p.floatval;
                }
        }

        bsdf* glass = new bsdf();
        glass->add_bxdf(bxdf_ptr(new fresnel_reflection_brdf(specular, ni, nt)));
        glass->add_bxdf(bxdf_ptr(new fresnel_refraction_btdf(specular, ni, nt)));

        return bsdf_ptr(glass);
}
}
