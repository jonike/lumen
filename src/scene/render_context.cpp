#include <area_light.h>
#include <bxdf.h>
#include <cmath>
#include <diffuse_brdf.h>
#include <geometry_factory.h>
#include <image.h>
#include <iostream>
#include <jittered_sampler.h>
#include <light_factory.h>
#include <material_factory.h>
#include <null_acceleration.h>
#include <path_tracer.h>
#include <render_context.h>
#include <render_task.h>
#include <texture_cache.h>
#include <thin_lens.h>
#include <thread_pool.h>

namespace lumen {
attributes::attributes() :
        material(nullptr),
        object_to_world()
{
}

options::options() :
        display_name("lumen.bmp"),
        horizontal_resolution(640),
        vertical_resolution(480),
        aspect_ratio(static_cast<float>(horizontal_resolution) / vertical_resolution),
        samplesx(2),
        samplesy(2),
        fstop(1.0f),
        focal_length(0.0f),
        focal_distance(0.0f),
        camera_projection(),
        world_to_camera()
{
}

void render_context::Begin()
{
#ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        texture_cache::init();
}

void render_context::End()
{
        texture_cache::shutdown();
}

void render_context::Display(const std::string& name)
{
        cur_options.display_name = name;
}

void render_context::Resolution(int width, int height)
{
        cur_options.horizontal_resolution = width;
        cur_options.vertical_resolution = height;
        cur_options.aspect_ratio = static_cast<float>(width) / height;
}

void render_context::PixelSamples(int x, int y)
{
        cur_options.samplesx = x;
        cur_options.samplesy = y;
}

void render_context::DepthOfField(float fstop, float focallength, float focaldistance)
{
        cur_options.fstop = fstop;
        cur_options.focal_length = focallength;
        cur_options.focal_distance = focaldistance;
}

void render_context::WorldBegin()
{
        camera.reset(new thin_lens(cur_attributes.object_to_world,
                                cur_options.camera_projection,
                                cur_options.horizontal_resolution,
                                cur_options.vertical_resolution,
                                cur_options.focal_length / cur_options.fstop / 2.0f,
                                cur_options.focal_distance));

        cur_attributes.object_to_world = nex::matrix();
}

void render_context::WorldEnd()
{
        const lumen::sampler_ptr sampler(new jittered_sampler);
        const lumen::acceleration_structure_ptr acceleration(new null_acceleration(geometry));
        path_tracer renderer(acceleration, lights, sampler.get(), 4, 1);
        image image(cur_options.horizontal_resolution, cur_options.vertical_resolution);

        int hres = cur_options.horizontal_resolution;
        int vres = cur_options.vertical_resolution;

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
                                        &renderer, camera.get(),
                                        sampler.get(), &image,
                                        cur_options.samplesx * cur_options.samplesy,
                                        x, y,
                                        std::min(block_dim, hres - x),
                                        std::min(block_dim, vres - y)));

                        pool.add_task(tasks[index].get());
                }
        }

        pool.run();

        while (image.is_open()) {
        }

        render_task::stop = true;

        pool.wait();

        if (render_task::save_image) {
                image.save(cur_options.display_name);
        }
}

void render_context::Geometry(const std::string& name, const parameter_list& params)
{
        geometry.push_back(create_geometry(cur_attributes, name, params));
}

void render_context::ObjectBegin()
{
        transform_stack.push(cur_attributes.object_to_world);
        attributes_stack.push(cur_attributes);
}

void render_context::ObjectEnd()
{
        cur_attributes.object_to_world = transform_stack.top();
        transform_stack.pop();

        cur_attributes = attributes_stack.top();
        attributes_stack.pop();
}

void render_context::Light(const std::string& name, const parameter_list& params)
{
        lights.push_back(create_light(name, params));
}

void render_context::Material(const std::string& name, const parameter_list& params)
{
        cur_attributes.material = create_material(cur_attributes, name, params);
}

void render_context::Projection(float fov)
{
        cur_options.camera_projection =
                nex::matrix::perspective(fov, cur_options.aspect_ratio, 1.0f, FLT_MAX);
}

void render_context::Translate(float x, float y, float z)
{
        nex::matrix translation = nex::matrix::translate(x, y, z);

        cur_attributes.object_to_world *= translation;
}

void render_context::Rotate(float degrees, float x, float y, float z)
{
        nex::matrix rotate = nex::matrix::rotate_axis(degrees, x, y, z);

        cur_attributes.object_to_world *= rotate;
}

void render_context::Scale(float x, float y, float z)
{
        nex::matrix scale(x, 0.0f, 0.0f, 0.0f,
                        0.0f, y, 0.0f, 0.0f,
                        0.0f, 0.0f, z, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);

        cur_attributes.object_to_world *= scale;
}
}
