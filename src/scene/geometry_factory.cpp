#include <geometry_factory.h>
#include <plane.h>
#include <rectangle.h>
#include <render_context.h>
#include <scene.h>
#include <sphere.h>
#include <stdexcept>

namespace lumen {
geometry_ptr create_geometry(const attributes& attr, const char* name,
                                int num, const char* tokens[], void* params[])
{
        if (strcmp(PLANE, name) == 0) {
                return geometry_ptr(new plane(attr.object_to_world, attr.material));
        } else if (strcmp(RECTANGLE, name) == 0) {
                return geometry_ptr(new rectangle(attr.object_to_world, attr.material));
        } else if (strcmp(SPHERE, name) == 0) {
                return geometry_ptr(new sphere(attr.object_to_world, attr.material));
        } else {
                throw std::invalid_argument("invalid geometry: " + std::string(name));
        }
}
}
