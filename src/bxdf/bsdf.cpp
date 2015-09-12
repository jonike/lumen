#include <algorithm>
#include <bsdf.h>
#include <geometry.h>
#include <util.h>

namespace lumen {
void bsdf::add_bxdf(const bxdf_ptr& bxdf)
{
        bxdf_terms.push_back(bxdf);
}

const bxdf* bsdf::get_bxdf(bxdf_type type) const
{
        for (const bxdf_ptr& bxdf : bxdf_terms) {
                if (bxdf->type() & type) {
                        return bxdf.get();
                }
        }

        return nullptr;
}

void bsdf::set_emittance(const nex::color& emittance)
{
        emittance_ = emittance;
}

nex::color bsdf::emittance(const surface& surface, const nex::vector& wo) const
{
        // only emit light from the front surface
        return (nex::dot(surface.normal, wo) < 0.0f) ? emittance_ : nex::color::black();
}

nex::color bsdf::sample(const struct sample& sample, const surface& surface,
        const nex::vector& wo, nex::vector* wi, float* pdf) const
{
        // select a bxdf to sample
        size_t num_bxdf = bxdf_terms.size();
        size_t idx = std::min(static_cast<size_t>(nex::frand() * num_bxdf + 0.5f), num_bxdf - 1);

        return bxdf_terms[idx]->sample(sample, surface, wo, wi, pdf);
}

nex::color bsdf::evaluate(const surface& surface,
        const nex::vector& wo, const nex::vector& wi) const
{
        nex::color reflectance;

        for (const bxdf_ptr& bxdf : bxdf_terms) {
                reflectance += bxdf->evaluate(surface, wo, wi);
        }

        return reflectance;
}
}
