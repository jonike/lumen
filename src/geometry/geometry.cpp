#include <geometry.h>
#include <util.h>

namespace lumen {
geometry::geometry(const nex::matrix& world, const bsdf_ptr& bsdf) :
        world(world),
        world_inverse(nex::inverse(world)),
        normal_mat(nex::transpose(world_inverse)),
        world_bound_(),
        bsdf(bsdf)
{
}

bool geometry::intersect(nex::ray* ray, surface* surface) const
{
        nex::ray object_ray(ray->origin * world_inverse, ray->direction * world_inverse, ray->max, ray->depth);

        float t;
        nex::vector normal;
        if (intersect(object_ray, &t, &normal) && (t > nex::EPSILON) && (t <= ray->max)) {
                surface->position = (*ray)(t);
                surface->normal = normal * normal_mat;
                nex::normalize(surface->normal);
                surface->texcoord = nex::point(0.0f, 0.0f, 0.0f);
                surface->bsdf = bsdf.get();

                ray->max = t;
                return true;
        }

        return false;
}

bool geometry::intersect(nex::ray* ray) const
{
        nex::ray object_ray(ray->origin * world_inverse, ray->direction * world_inverse, ray->max, ray->depth);

        float t;
        if (intersect_shadow(object_ray, &t) && (t > nex::EPSILON) && (t <= ray->max)) {
                ray->max = t;
                return true;
        }

        return false;
}

void geometry::sample_surface(const sample& sample, nex::point* pos, nex::vector* norm) const
{
        get_surface_sample(sample, pos, norm);

        *pos = *pos * world;

        *norm = *norm * normal_mat;
        nex::normalize(*norm);
}

const bound& geometry::world_bound() const
{
        return world_bound_;
}

void geometry::set_bound(const nex::point& min, const nex::point& max)
{
        world_bound_ = bound(min, max);
        world_bound_.transform(world);
}

bool geometry::intersect_shadow(const nex::ray& ray, float* t) const
{
        nex::vector norm;
        return intersect(ray, t, &norm);
}
}
