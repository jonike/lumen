#include <sampler.h>
#include <thin_lens.h>

namespace lumen {
thin_lens::thin_lens(const nex::matrix& view, const nex::matrix& projection,
        int hres, int vres, float lens_radius, float focal_distance) :
        camera(),
        view(view),
        projection(projection),
        hres(hres),
        vres(vres),
        lens_radius(lens_radius),
        focal_distance(focal_distance)
{
}

nex::ray thin_lens::generate_ray(float x, float y, const sample& s) const
{
        nex::point origin;
        nex::vector direction;
        create_view_ray(x + s.x, y + s.y, &origin, &direction);

        if (lens_radius > 0.0f) {
                depth_of_field(s, &origin, &direction);
        }

        return nex::ray(origin * view, direction * view);
}

void thin_lens::create_view_ray(float x, float y, nex::point* origin, nex::vector* direction) const
{
        origin->x = 0.0f;
        origin->y = 0.0f;
        origin->z = 0.0f;

        // convert screen-space coordinates to clip space and apply inverse of projection transform
        direction->x = (2.0f * x / hres - 1.0f) / projection.m00;
        direction->y = (-2.0f * y / vres + 1.0f) / projection.m11;
        direction->z = 1.0f;

        nex::normalize(*direction);
}

void thin_lens::depth_of_field(const sample& s, nex::point* origin, nex::vector* direction) const
{
        float ratio = focal_distance / direction->z;
        nex::point focal_point(direction->x * ratio, direction->y * ratio, focal_distance);

        nex::point lens_sample = sample_disk(s);

        *origin = nex::point(lens_sample.x * lens_radius, lens_sample.y * lens_radius, 0.0f);
        *direction = focal_point - *origin;

        nex::normalize(*direction);
}
}
