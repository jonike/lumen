#ifndef LUMEN_RENDERTASK_H
#define LUMEN_RENDERTASK_H

#include <memory>
#include <sampler.h>
#include <task.h>
#include <vector>

namespace lumen {
class camera;
class image;
class renderer;

class render_task : public nex::task {
public:
        render_task(const renderer*, const camera*, sampler*, image*,
                int spp, int screenx, int screeny, int w, int h);

        virtual void run();
private:
        void shade_pixel(int x, int y);
        nex::color shade_sample(const sample& sample, int x, int y);

        const renderer* renderer_;
        const camera* camera_;
        std::unique_ptr<sampler> sampler_;
        image* image_;
        sample_set antialiasing_samples;
        int screenx;
        int screeny;
        int w;
        int h;
        std::vector<nex::color> pixels;
};
}

#endif
