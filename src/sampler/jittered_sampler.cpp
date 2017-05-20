#include <algorithm>
#include <cassert>
#include <jittered_sampler.h>
#include "nex\util.h"

namespace lumen {
static void jitter(sample_set& set);

sampler* jittered_sampler::clone()
{
        return new jittered_sampler(*this);
}

void jittered_sampler::generate_samples()
{
        for (sample_set& set : samples) {
                jitter(set);
        }
}

void jittered_sampler::generate_samples(sample_set* set) const
{
        jitter(*set);
}

static void jitter(sample_set& set)
{
        size_t num_stratum = static_cast<size_t>(std::sqrt(static_cast<float>(set.size())));

        assert(num_stratum * num_stratum == set.size());

        for (size_t i = 0; i < set.size(); ++i) {
                size_t x = i % num_stratum;
                size_t y = i / num_stratum;

                set[i].x = (x + nex::frand()) / num_stratum;
                set[i].y = (y + nex::frand()) / num_stratum;
        }

        std::random_shuffle(set.begin(), set.end());
}
}
