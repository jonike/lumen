#include <area_light.h>
#include <util.h>

namespace lumen {
area_light::area_light(const nex::color& radiance, const geometry_ptr& emitter) :
        light(),
        radiance(radiance),
        emitter(emitter)
{
}

nex::color area_light::sample_light(const sample& sample, const surface& surface,
        nex::vector* wi, nex::ray* vray, float* pdf) const
{
        // sample the geometry
        nex::point light_pos;
        nex::vector light_norm;
        emitter->sample_surface(sample, &light_pos, &light_norm);

        // set the incident light direction
        *wi = light_pos - surface.position;
        float distance = nex::magnitude(*wi);
        *wi /= distance;

        // set the visibility ray
        *vray = nex::ray(surface.position, *wi, distance - nex::EPSILON);

        // calculate partial geometry term between emitter and receiver
        float area = emitter->pdf();
        float g = std::max(nex::dot(light_norm, -(*wi)) / (distance * distance), 0.0f);
        *pdf = area / g;

        return (nex::dot(light_norm, -(*wi)) > 0.0f) ? radiance : nex::color::black();
}
}
