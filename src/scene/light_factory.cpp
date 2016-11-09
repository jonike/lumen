#include <bilinear_sampler.h>
#include <directional_light.h>
#include <environment_light.h>
#include <light_factory.h>
#include <point_light.h>
#include <render_context.h>
#include <scene.h>
#include <stdexcept>
#include <texture_cache.h>
#include <texture_sampler.h>

namespace lumen {
static light_ptr create_point_light(const parameter_list& params);
static light_ptr create_directional_light(const parameter_list& params);
static light_ptr create_environment_light(const parameter_list& params);

light_ptr create_light(const std::string& name, const parameter_list& params)
{
        if (name == POINT) {
                return create_point_light(params);
        } else if (name == DIRECTIONAL) {
                return create_directional_light(params);
        } else if (name == ENVIRONMENT) {
                return create_environment_light(params);
        } else {
                throw std::invalid_argument("invalid light: " + name);
        }
}

static light_ptr create_point_light(const parameter_list& params)
{
        nex::color color = nex::color::white();
        nex::point pos;

        for (const parameter& p : params) {
                if (p.name == COLOR) {
                        color = p.color();
                } else if (p.name == POSITION) {
                        pos = p.point();
                }
        }

        return light_ptr(new point_light(color, pos));
}

static light_ptr create_directional_light(const parameter_list& params)
{
        nex::color color = nex::color::white();
        nex::vector dir(0.0f, -1.0f, 0.0f);

        for (const parameter& p : params) {
                if (p.name == COLOR) {
                        color = p.color();
                } else if (p.name == DIRECTION) {
                        dir = p.vector();
                        nex::normalize(dir);
                }
        }

        return light_ptr(new directional_light(color, dir));
}

static light_ptr create_environment_light(const parameter_list& params)
{
        nex::color color = nex::color::white();
        texture_sampler_ptr envmap;

        for (const parameter& p : params) {
                if (p.name == TEXTURENAME) {
                        envmap.reset(new bilinear_sampler(texture_cache::load(p.strval), AM_CLAMP, AM_CLAMP));
                } else if (p.name == COLOR) {
                        color = p.color();
                }
        }

        return light_ptr(new environment_light(color, envmap));
}
}
