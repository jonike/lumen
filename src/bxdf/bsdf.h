#ifndef LUMEN_BSDF_H
#define LUMEN_BSDF_H

#include <bxdf.h>
#include <color.h>
#include <memory>
#include <vector>
#include <vector.h>

namespace lumen {
struct sample;

class bsdf {
public:
        void add_bxdf(const bxdf_ptr&);
        const bxdf* get_bxdf(bxdf_type) const;

        void set_emittance(const nex::color&);
        nex::color emittance(const surface& surface, const nex::vector& wo) const;

        nex::color sample(const sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const;
        nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const;
private:
        std::vector<bxdf_ptr> bxdf_terms;
        nex::color emittance_;
};

typedef std::shared_ptr<bsdf> bsdf_ptr;
}

#endif
