#include <cstdarg>
#include <vector>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#endif

#include <render_context.h>

namespace lumen {
const char* RECTANGLE = "rectangle";
const char* SPHERE    = "sphere";
const char* MESH      = "mesh";

const char* MESHNAME = "meshname";

const char* POINT       = "point";
const char* DIRECTIONAL = "directional";
const char* ENVIRONMENT = "environment";

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

void Display(const std::string& name)
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

void Geometry(const std::string& name, const parameter_list& params)
{
        context.Geometry(name, params);
}

void ObjectBegin()
{
        context.ObjectBegin();
}

void ObjectEnd()
{
        context.ObjectEnd();
}

void Light(const std::string& name, const parameter_list& params)
{
        context.Light(name, params);
}

void Material(const std::string& name, const parameter_list& params)
{
        context.Material(name, params);
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
