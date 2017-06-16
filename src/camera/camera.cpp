#include <mutex>
#include "nex\thread_pool.h"
#include "camera.h"
#include "film.h"
#include "jittered_sampler.h"
#include "render_context.h"
#include "render_task.h"

namespace lumen {
camera::camera()
{
}

camera::~camera()
{
}

void camera::render(const renderer* renderer, sampler* sampler, film* film, const options& options)
{
        int hres = options.horizontal_resolution;
        int vres = options.vertical_resolution;

        int block_dim = 32;

        int grid_dimx = static_cast<int>(std::ceil(static_cast<float>(hres) / block_dim));
        int grid_dimy = static_cast<int>(std::ceil(static_cast<float>(vres) / block_dim));

        nex::thread_pool pool;
        std::vector<std::shared_ptr<render_task>> tasks(grid_dimx * grid_dimy);

        for (int j = 0; j < grid_dimy; ++j) {
                for (int i = 0; i < grid_dimx; ++i) {
                        int index = i + j * grid_dimx;

                        int x = i * block_dim;
                        int y = j * block_dim;

                        tasks[index].reset(new render_task(
                                renderer, this, sampler, film,
                                options.samplesx * options.samplesy,
                                x, y,
                                std::min(block_dim, hres - x),
                                std::min(block_dim, vres - y)));

                        pool.add_task(tasks[index].get());
                }
        }

        pool.run();

        if (!film->is_open()) {
                render_task::stop = true;
        }

        pool.wait();

        if (render_task::save_image) {
                film->save(options.display_name);
        }
}
}
