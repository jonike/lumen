#ifndef LUMEN_CAMERA_H
#define LUMEN_CAMERA_H

#include <memory>
#include <ray.h>

namespace lumen {
struct sample;

class camera {
public:
        virtual nex::ray generate_ray(float x, float y, const sample& s) const = 0;

        virtual ~camera() {}
};

typedef std::shared_ptr<camera> camera_ptr;
}

#endif
