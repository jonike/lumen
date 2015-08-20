#include <acceleration_structure.h>
#include <bsdf.h>
#include <cfloat>
#include <geometry.h>
#include <light.h>
#include <path_tracer.h>
#include <sampler.h>
#include <util.h>

namespace lumen {
path_tracer::path_tracer(const acceleration_structure_ptr& acceleration,
        const std::vector<light_ptr>& lights, sampler* sampler,
        int max_depth, int num_light_samples) :
        renderer(acceleration, lights),
        max_depth(max_depth),
        light_index(sampler->request_samples(num_light_samples)),
        bsdf_index(sampler->request_samples(max_depth))
{
}

nex::color path_tracer::radiance(const sampler* sampler, nex::ray* ray) const
{
        return radiance(sampler, ray, true);
}

nex::color path_tracer::radiance(const sampler* sampler, nex::ray* ray, bool emittance) const
{
        surface surface;
        if ((ray->depth >= max_depth) || !acceleration->intersect(ray, &surface)) {
                return nex::color::black();
        }

        nex::color radiance;

        if (emittance) {
                radiance += surface.bsdf->emittance(surface, ray->direction);
        }

        radiance += direct_illumination(sampler, ray, surface);
        radiance += indirect_illumination(sampler, ray, surface);

        return radiance;
}

nex::color path_tracer::direct_illumination(const sampler* sampler, const nex::ray* ray,
        const surface& surface) const
{
        nex::color radiance;
        nex::vector wi;
        nex::ray vray;

        size_t num_lights = lights.size();

        const sample_set& samples = sampler->get_samples(light_index);
        size_t num_samples = samples.size();

        for (size_t i = 0; i < num_samples; ++i) {
                // select a light source
                size_t index = static_cast<size_t>(nex::frand() * num_lights + 0.5f);
                index = std::min(index, num_lights - 1);

                const light_ptr& light = lights[index];

                // add the light's contribution
                float pdf = 0.0f;
                nex::color l = light->sample_light(samples[i], surface, &wi, &vray, &pdf);

                if ((pdf > 0.0f) && !acceleration->intersect(&vray)) {
                        float cos = nex::dot(surface.normal, wi);

                        if (cos > 0.0f) {
                                nex::color f = surface.bsdf->evaluate(surface, -ray->direction, wi);

                                radiance += l * f * cos / pdf;
                        }
                }
        }

        // a uniform pdf was selected for choosing a light source. The pdf is
        // equal to 1 / N, where N is the number of lights.
        radiance *= (static_cast<float>(num_lights) / static_cast<float>(num_samples));

        return radiance;
}

nex::color path_tracer::indirect_illumination(const sampler* sampler, const nex::ray* ray,
        const surface& surface) const
{
        nex::vector wi;
        float pdf = 0.0f;

        nex::color f = surface.bsdf->sample(sampler->get_samples(bsdf_index)[ray->depth],
                                                surface, -ray->direction, &wi, &pdf);

        if ((f == nex::color::black()) || (pdf <= 0.0f)) {
                return nex::color::black();
        }

        nex::ray reflected_ray(surface.position + nex::EPSILON * wi, wi, FLT_MAX, ray->depth + 1);

        bool emittance = (surface.bsdf->get_bxdf(BXDF_SPECULAR) != nullptr);

        float cos = std::abs(nex::dot(surface.normal, wi));

        return radiance(sampler, &reflected_ray, emittance) * f * cos / pdf;
}
}
