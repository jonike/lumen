#include <directional_light.h>
#include <light_factory.h>
#include <point_light.h>
#include <render_context.h>
#include <scene.h>
#include <stdexcept>

namespace lumen {
static light_ptr create_point_light(int num, const char* tokens[], void* params[]);
static light_ptr create_directional_light(int num, const char* tokens[], void* params[]);

light_ptr create_light(const char* name, int num, const char* tokens[], void* params[])
{
        if (strcmp(POINT, name) == 0) {
                return create_point_light(num, tokens, params);
        } else if (strcmp(DIRECTIONAL, name) == 0) {
                return create_directional_light(num, tokens, params);
        } else {
                throw std::invalid_argument("invalid light " + std::string(name));
        }
}

static light_ptr create_point_light(int num, const char* tokens[], void* params[])
{
        float color[3] = {1.0f, 1.0f, 1.0f};
        float pos[3] = {0.0f, 0.0f, 0.0f};

        for (int i = 0; i < num; ++i) {
                if (strcmp(COLOR, tokens[i]) == 0) {
                        memcpy(color, reinterpret_cast<float*>(params[i]), sizeof(color));
                } else if (strcmp(POSITION, tokens[i]) == 0) {
                        memcpy(pos, reinterpret_cast<float*>(params[i]), sizeof(pos));
                }
        }

        nex::color lightcolor(color[0], color[1], color[2]);
        nex::point position(pos[0], pos[1], pos[2]);

        return light_ptr(new point_light(lightcolor, position));
}

static light_ptr create_directional_light(int num, const char* tokens[], void* params[])
{
        float color[3] = {1.0f, 1.0f, 1.0f};
        float dir[3] = {0.0f, -1.0f, 0.0f};

        for (int i = 0; i < num; ++i) {
                if (strcmp(COLOR, tokens[i]) == 0) {
                        memcpy(color, reinterpret_cast<float*>(params[i]), sizeof(color));
                } else if (strcmp(DIRECTION, tokens[i]) == 0) {
                        memcpy(dir, reinterpret_cast<float*>(params[i]), sizeof(dir));
                }
        }

        nex::color lightcolor(color[0], color[1], color[2]);

        nex::vector direction(dir[0], dir[1], dir[2]);
        nex::normalize(direction);

        return light_ptr(new directional_light(lightcolor, direction));
}
}
