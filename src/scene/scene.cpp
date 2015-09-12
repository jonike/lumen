#include <cstdarg>
#include <vector>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#endif

#include <render_context.h>

namespace lumen {
#define BUILD_PARAMETER_LIST(p) \
        std::vector<const char*> tokens; \
        std::vector<void*> values; \
        va_list list; \
        va_start(list, p); \
        const char* token = va_arg(list, const char*); \
        void* value = va_arg(list, void*); \
        while (token != nullptr) { \
                tokens.push_back(token); \
                values.push_back(value); \
                token = va_arg(list, const char*); \
                value = va_arg(list, void*); \
        } \
        va_end(list)

#define SET_PARAMETER_LIST \
        tokens.size(), \
        (tokens.size() > 0) ? tokens.data() : nullptr, \
        (values.size() > 0) ? values.data() : nullptr

const char* PLANE     = "plane";
const char* RECTANGLE = "rectangle";
const char* SPHERE    = "sphere";

const char* POINT       = "point";
const char* DIRECTIONAL = "directional";

const char* COLOR     = "color";
const char* DIRECTION = "direction";
const char* POSITION  = "position";

const char* MATTE   = "matte";
const char* CHROME  = "chrome";
const char* PLASTIC = "plastic";
const char* GLASS   = "glass";

const char* DIFFUSECOLOR  = "diffusecolor";
const char* SPECULARCOLOR = "specularcolor";
const char* EXPONENT      = "exponent";
const char* NI            = "ni";
const char* NT            = "nt";
const char* TEXTURENAME   = "texturename";

static render_context context;

void Begin()
{
#if _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        context.Begin();
}

void End()
{
        context.End();
}

void Display(const char* name)
{
        context.Display(name);
}

void Resolution(int width, int height)
{
        context.Resolution(width, height);
}

void PixelSamples(int x, int y)
{
        context.PixelSamples(x, y);
}

void DepthOfField(float fstop, float focallength, float focaldistance)
{
        context.DepthOfField(fstop, focallength, focaldistance);
}

void WorldBegin()
{
        context.WorldBegin();
}

void WorldEnd()
{
        context.WorldEnd();
}

void Geometry(const char* name, ...)
{
        BUILD_PARAMETER_LIST(name);

        context.Geometry(name, SET_PARAMETER_LIST);
}

void ObjectBegin()
{
        context.ObjectBegin();
}

void ObjectEnd()
{
        context.ObjectEnd();
}

void Light(const char* name, ...)
{
        BUILD_PARAMETER_LIST(name);

        context.Light(name, SET_PARAMETER_LIST);
}

void Material(const char* name, ...)
{
        BUILD_PARAMETER_LIST(name);

        context.Material(name, SET_PARAMETER_LIST);
}

void Projection(float fov)
{
        context.Projection(fov);
}

void Translate(float x, float y, float z)
{
        context.Translate(x, y, z);
}

void Rotate(float degrees, float x, float y, float z)
{
        context.Rotate(degrees, x, y, z);
}

void Scale(float x, float y, float z)
{
        context.Scale(x, y, z);
}
}
