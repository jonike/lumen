#ifndef LUMEN_BXDF_H
#define LUMEN_BXDF_H

#include <memory>
#include "nex\color.h"
#include "nex\vector.h"

namespace lumen {
struct sample;
struct surface;

enum bxdf_type {
        BXDF_REFLECTION   = (1 << 0),
        BXDF_TRANSMISSION = (1 << 1),
        BXDF_DIFFUSE      = (1 << 2),
        BXDF_SPECULAR     = (1 << 3),
        BXDF_GLOSSY       = (1 << 4),
};

nex::vector reflect(const nex::vector& incident, const nex::vector& normal);
nex::vector refract(const nex::vector& incident, const nex::vector& normal, float* eta);
float fresnel(float ni, float nt, float cosi);

class bxdf {
public:
        virtual nex::color sample(const sample& sample, const surface& surface,
                const nex::vector& wo, nex::vector* wi, float* pdf) const = 0;
        virtual nex::color evaluate(const surface& surface,
                const nex::vector& wo, const nex::vector& wi) const = 0;

        virtual int type() const = 0;

        virtual ~bxdf() {}
};

typedef std::shared_ptr<bxdf> bxdf_ptr;
}

#endif
