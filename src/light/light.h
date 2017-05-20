#ifndef LUMEN_LIGHT_H
#define LUMEN_LIGHT_H

#include <memory>
#include "nex\color.h"
#include "nex\ray.h"
#include "nex\vector.h"

namespace lumen {
struct sample;
struct surface;

class light {
public:
        virtual nex::color sample_light(const sample& sample, const surface& surface,
                nex::vector* wi, nex::ray* vray, float* pdf) const = 0;

        virtual nex::color emission(const nex::vector& wi);

        virtual ~light() {}
};

typedef std::shared_ptr<light> light_ptr;
}

#endif
