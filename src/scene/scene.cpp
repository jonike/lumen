#include <cstdarg>
#include <vector>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#endif

namespace lumen {
struct parameter_list {
        std::vector<const char*> tokens;
        std::vector<void*> values;
};

template<typename T>
static parameter_list build_parameter_list(T& last_param, ...);

#define BUILD_PARAMETER_LIST(p) \
        parameter_list params = build_parameter_list(p)

#define SET_PARAMETER_LIST \
        params.tokens.size(), \
        (params.tokens.size() > 0) ? params.tokens.data() : nullptr, \
        (params.values.size() > 0) ? params.values.data() : nullptr

const char* PLANE     = "plane";
const char* RECTANGLE = "rectangle";
const char* SPHERE    = "sphere";

const char* POINT       = "point";
const char* DIRECTIONAL = "directional";

const char* COLOR     = "color";
const char* DIRECTION = "direction";
const char* POSITION  = "position";

const char* MATTE = "matte";

const char* DIFFUSECOLOR = "diffusecolor";
const char* TEXTURENAME  = "texturename";

void Begin()
{
#if _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void End()
{
}

void Display(const char* name)
{
}

void Resolution(int width, int height)
{
}

void PixelSamples(int x, int y)
{
}

void DepthOfField(float fstop, float focallength, float focaldistance)
{
}

void WorldBegin()
{
}

void WorldEnd()
{
}

void Geometry(const char* name, ...)
{
        BUILD_PARAMETER_LIST(name);
}

void ObjectBegin()
{
}

void ObjectEnd()
{
}

void Light(const char* name, ...)
{
        BUILD_PARAMETER_LIST(name);
}

void Material(const char* name, ...)
{
        BUILD_PARAMETER_LIST(name);
}

void Projection(float fov)
{
}

void Translate(float x, float y, float z)
{
}

void Rotate(float degrees, float x, float y, float z)
{
}

void Scale(float x, float y, float z)
{
}

template<typename T>
static parameter_list build_parameter_list(T& last_param, ...)
{
        parameter_list params;

        va_list list;
        va_start(list, last_param);

        const char* token = va_arg(list, const char*);
        void* value = va_arg(list, void*);

        while (token != nullptr) {
                params.tokens.push_back(token);
                params.values.push_back(value);

                token = va_arg(list, const char*);
                value = va_arg(list, void*);
        }

        va_end(list);

        return params;
}
}
