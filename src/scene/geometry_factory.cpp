#include <geometry_factory.h>
#include <mesh.h>
#include <plane.h>
#include <rectangle.h>
#include <render_context.h>
#include <scene.h>
#include <sphere.h>
#include <stdexcept>

namespace lumen {
static geometry_ptr create_mesh(const attributes& attr, const parameter_list& params);

geometry_ptr create_geometry(const attributes& attr, const std::string& name, const parameter_list& params)
{
        if (name == PLANE) {
                return geometry_ptr(new plane(attr.object_to_world, attr.material));
        } else if (name == RECTANGLE) {
                return geometry_ptr(new rectangle(attr.object_to_world, attr.material));
        } else if (name == SPHERE) {
                return geometry_ptr(new sphere(attr.object_to_world, attr.material));
        } else if (name == MESH) {
                return create_mesh(attr, params);
        } else {
                throw std::invalid_argument("invalid geometry: " + name);
        }
}

static geometry_ptr create_mesh(const attributes& attr, const parameter_list& params)
{
        std::string filename;

        for (const parameter& p : params) {
                if (p.name == MESHNAME) {
                        filename = p.strval;
                }
        }

        return geometry_ptr(new mesh(attr.object_to_world, attr.material, filename));
}
}
