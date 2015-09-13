#include <geometry_factory.h>
#include <mesh.h>
#include <plane.h>
#include <rectangle.h>
#include <render_context.h>
#include <scene.h>
#include <sphere.h>
#include <stdexcept>

namespace lumen {
static geometry_ptr create_mesh(const attributes& attr, const char* name,
        int num, const char* tokens[], void* params[]);

geometry_ptr create_geometry(const attributes& attr, const char* name,
                                int num, const char* tokens[], void* params[])
{
        if (strcmp(PLANE, name) == 0) {
                return geometry_ptr(new plane(attr.object_to_world, attr.material));
        } else if (strcmp(RECTANGLE, name) == 0) {
                return geometry_ptr(new rectangle(attr.object_to_world, attr.material));
        } else if (strcmp(SPHERE, name) == 0) {
                return geometry_ptr(new sphere(attr.object_to_world, attr.material));
        } else if (strcmp(MESH, name) == 0) {
                return create_mesh(attr, name, num, tokens, params);
        } else {
                throw std::invalid_argument("invalid geometry: " + std::string(name));
        }
}

static geometry_ptr create_mesh(const attributes& attr, const char* name,
        int num, const char* tokens[], void* params[])
{
        std::string filename;

        for (int i = 0; i < num; ++i) {
                if (strcmp(MESHNAME, tokens[i]) == 0) {
                        filename = static_cast<const char*>(params[i]);
                }
        }

        return geometry_ptr(new mesh(attr.object_to_world, attr.material, filename));
}
}
