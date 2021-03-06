#ifndef LUMEN_SCENE_H
#define LUMEN_SCENE_H

#include "parameter.h"

namespace lumen {
// geometry types
extern const char* RECTANGLE;
extern const char* SPHERE;
extern const char* MESH;

// geometry parameters
extern const char* MESHNAME;

// light types
extern const char* DIRECTIONAL;
extern const char* POINT;
extern const char* ENVIRONMENT;

// light parameters
extern const char* COLOR;
extern const char* DIRECTION;
extern const char* POSITION;
extern const char* TEXTURENAME;

// material types
extern const char* MATTE;
extern const char* CHROME;
extern const char* PLASTIC;
extern const char* GLASS;

// material parameters
extern const char* DIFFUSECOLOR;
extern const char* SPECULARCOLOR;
extern const char* EXPONENT;
extern const char* NI;
extern const char* NT;
extern const char* TEXTURENAME;

// main initialization and shutdown
void Begin();
void End();

// display methods
void Display(const std::string& name);
void Resolution(int width, int height);
void PixelSamples(int x, int y);
void DepthOfField(float fstop, float focallength, float focaldistance);

// world initialization and shutdown
void WorldBegin();
void WorldEnd();

// geometry creation
void Geometry(const std::string& name, const parameter_list& params);
void ObjectBegin();
void ObjectEnd();

// light creation
void Light(const std::string& name, const parameter_list& params);

// material creation
void Material(const std::string& name, const parameter_list& params);

// transformation methods
void Projection(float fov);
void Translate(float x, float y, float z);
void Rotate(float degrees, float x, float y, float z);
void Scale(float x, float y, float z);
}

#endif
