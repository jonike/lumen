#ifndef LUMEN_JITTEREDSAMPLER_H
#define LUMEN_JITTEREDSAMPLER_H

#include <sampler.h>

namespace lumen {
class jittered_sampler : public sampler {
public:
        virtual sampler* clone();

        virtual void generate_samples();
        virtual void generate_samples(sample_set* set) const;
};
}

#endif
