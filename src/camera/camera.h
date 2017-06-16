#ifndef LUMEN_CAMERA_H
#define LUMEN_CAMERA_H

#include <memory>
#include "nex\ray.h"

namespace lumen {
class film;
class renderer;
class sampler;
struct options;
struct sample;

class camera {
public:
        camera();
        virtual ~camera();

        void render(const renderer* renderer, sampler* sampler, film* film, const options& options);

        virtual nex::ray generate_ray(float x, float y, const sample& s) const = 0;
};

typedef std::shared_ptr<camera> camera_ptr;
}

#endif
