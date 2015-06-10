#ifndef LUMEN_PATHTRACER_H
#define LUMEN_PATHTRACER_H

#include <acceleration_structure.h>
#include <renderer.h>

namespace lumen {
class path_tracer : public renderer {
public:
        path_tracer(const acceleration_structure_ptr& acceleration,
                const std::vector<light_ptr>& lights,
                sampler* sampler, int max_depth, int num_light_samples);

        virtual nex::color radiance(const sampler*, nex::ray*) const;
private:
        nex::color radiance(const sampler*, nex::ray*, bool emittance) const;
        nex::color direct_illumination(const sampler*, const nex::ray*, const surface&) const;
        nex::color indirect_illumination(const sampler*, const nex::ray*, const surface&) const;

        const int max_depth;
        const size_t light_index;
        const size_t bsdf_index;
};
}

#endif
