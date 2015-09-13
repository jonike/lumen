#include <algorithm>
#include <cassert>
#include <kd_tree.h>
#include <mesh.h>
#include <stdexcept>
#include <triangle.h>
#include <nex\src\resource\mesh.h>

namespace lumen {
mesh::mesh(const nex::matrix& world, const bsdf_ptr& bsdf, const std::string& filename) :
        geometry(world, bsdf),
        acceleration_(nullptr)
{
        nex::mesh mesh;
        if (mesh.load(filename) != nex::SUCCESS) {
                throw std::invalid_argument("failed to load mesh: " + std::string(filename));
        }

        unsigned int* indices = mesh.indices();
        nex::mesh::vertex* vertices = mesh.vertices();

        unsigned int num_triangles = mesh.index_count() / 3;
        std::vector<geometry_ptr> triangles(num_triangles);

        float minx = FLT_MAX;
        float miny = FLT_MAX;
        float minz = FLT_MAX;

        float maxx = -FLT_MAX;
        float maxy = -FLT_MAX;
        float maxz = -FLT_MAX;

        for (unsigned int i = 0; i < num_triangles; ++i) {
                const nex::mesh::vertex& v0 = vertices[indices[i * 3 + 0]];
                const nex::mesh::vertex& v1 = vertices[indices[i * 3 + 1]];
                const nex::mesh::vertex& v2 = vertices[indices[i * 3 + 2]];

                // calculate the min and max values of the set
                minx = std::min(std::min(v0.position.x, v1.position.x), std::min(minx, v2.position.x));
                miny = std::min(std::min(v0.position.y, v1.position.y), std::min(miny, v2.position.y));
                minz = std::min(std::min(v0.position.z, v1.position.z), std::min(minz, v2.position.z));

                maxx = std::max(std::max(v0.position.x, v1.position.x), std::max(maxx, v2.position.x));
                maxy = std::max(std::max(v0.position.y, v1.position.y), std::max(maxy, v2.position.y));
                maxz = std::max(std::max(v0.position.z, v1.position.z), std::max(maxz, v2.position.z));

                // add the triangle
                vertex t0;
                t0.position = v0.position;
                t0.normal = v0.normal;

                vertex t1;
                t1.position = v1.position;
                t1.normal = v1.normal;

                vertex t2;
                t2.position = v2.position;
                t2.normal = v2.normal;

                triangles[i] = geometry_ptr(new triangle(world, bsdf, t0, t1, t2));
        }

        acceleration_.reset(new kd_tree(triangles, 16, 32));

        set_bound(nex::point(minx, miny, minz), nex::point(maxx, maxy, maxz));
}

float mesh::pdf() const
{
        assert(!"NOT IMPLEMENTED");
        return 0.0f;
}

bool mesh::intersect(const nex::ray& ray, float* t, nex::vector* norm) const
{
        nex::ray tmp_ray = ray;
        surface s;

        bool intersect = acceleration_->intersect(&tmp_ray, &s);

        if (intersect) {
                *t = tmp_ray.max;
                *norm = s.normal;
        }

        return intersect;
}

void mesh::get_surface_sample(const sample& sample, nex::point* pos, nex::vector* norm) const
{
        assert(!"NOT IMPLEMENTED");
}
}
