#ifndef LUMEN_SAMPLER_H
#define LUMEN_SAMPLER_H

#include <memory>
#include <point.h>
#include <vector.h>
#include <vector>

namespace lumen {
struct sample {
        float x;
        float y;
};

typedef std::vector<sample> sample_set;

nex::vector sample_hemisphere(const sample& sample, const nex::vector& normal);
nex::vector sample_phong_lobe(const sample& sample, const nex::vector& normal, float exponent);
nex::point sample_disk(const sample& sample);

class sampler {
public:
        size_t request_samples(size_t n);
        const sample_set& get_samples(size_t set) const;

        virtual sampler* clone() = 0;

        virtual void generate_samples() = 0;
        virtual void generate_samples(sample_set* set) const = 0;

        virtual ~sampler() {}
protected:
        std::vector<sample_set> samples;
};

typedef std::shared_ptr<sampler> sampler_ptr;
}

#endif
