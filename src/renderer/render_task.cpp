#include <camera.h>
#include <image.h>
#include <render_task.h>
#include <renderer.h>
#include <sampler.h>

namespace lumen {
bool render_task::stop = false;
bool render_task::save_image = true;

render_task::render_task(const renderer* renderer, const camera* camera, sampler* sampler,
        image* image, int spp, int screenx, int screeny, int w, int h) :
        renderer_(renderer),
        camera_(camera),
        sampler_(sampler->clone()),
        image_(image),
        antialiasing_samples(spp),
        screenx(screenx),
        screeny(screeny),
        w(w),
        h(h),
        pixels(w * h)
{
}

void render_task::run()
{
        for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                        if (stop) {
                                save_image = false;
                                return;
                        }

                        shade_pixel(x, y);
                }
        }

        image_->draw(screenx, screeny, w, h, pixels.data());
}

void render_task::shade_pixel(int x, int y)
{
        sampler_->generate_samples(&antialiasing_samples);

        int pixel_idx = x + y * w;

        for (const auto& sample : antialiasing_samples) {
                sampler_->generate_samples();

                pixels[pixel_idx] += shade_sample(sample, x, y);
        }

        pixels[pixel_idx] /= static_cast<float>(antialiasing_samples.size());
}

nex::color render_task::shade_sample(const sample& sample, int x, int y)
{
        nex::ray ray = camera_->generate_ray(
                                static_cast<float>(screenx + x),
                                static_cast<float>(screeny + y),
                                sample);

        return renderer_->radiance(sampler_.get(), &ray);
}
}
