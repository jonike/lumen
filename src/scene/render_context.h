#ifndef LUMEN_RENDERCONTEXT_H
#define LUMEN_RENDERCONTEXT_H

#include <bsdf.h>
#include <camera.h>
#include <geometry.h>
#include <light.h>
#include <memory>
#include <sampler.h>
#include <stack>
#include <string>
#include <vector>
#include "nex\color.h"
#include "nex\matrix.h"
#include "nex\vector.h"
#include "parameter.h"

namespace lumen {
struct attributes {
        attributes();

        bsdf_ptr material;
        nex::matrix object_to_world;
};

struct options {
        options();

        std::string display_name;
        int horizontal_resolution;
        int vertical_resolution;
        float aspect_ratio;
        int samplesx;
        int samplesy;
        float fstop;
        float focal_length;
        float focal_distance;
        nex::matrix camera_projection;
        nex::matrix world_to_camera;
};

class render_context {
public:
        void Begin();
        void End();

        void Display(const std::string& name);
        void Resolution(int width, int height);
        void PixelSamples(int x, int y);
        void DepthOfField(float fstop, float focallength, float focaldistance);

        void WorldBegin();
        void WorldEnd();

        void Geometry(const std::string& name, const parameter_list& params);
        void ObjectBegin();
        void ObjectEnd();

        void Light(const std::string& name, const parameter_list& params);

        void Material(const std::string& name, const parameter_list& params);

        void Projection(float fov);
        void Translate(float x, float y, float z);
        void Rotate(float degrees, float x, float y, float z);
        void Scale(float x, float y, float z);
private:
        std::auto_ptr<camera> camera;
        std::vector<geometry_ptr> geometry;
        std::vector<light_ptr> lights;
        std::stack<nex::matrix> transform_stack;
        options cur_options;
        std::stack<options> options_stack;
        attributes cur_attributes;
        std::stack<attributes> attributes_stack;
};
}

#endif
