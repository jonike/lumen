#include <algorithm>
#include <vector>
#include "nex\util.h"
#include "film.h"

namespace lumen {
static nex::color gamma_correct(const nex::color& color, float gamma);
static int quantize(const nex::color& color);

const float GAMMA_VALUE = 2.2f;

film::film(int w, int h) :
        render_surface(nullptr),
        pixel_mutex()
{
        try {
                if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                        throw std::runtime_error(SDL_GetError());
                }

                int flags = 0;
                int depth = 32;
                Uint32 rmask = 0x000000ff;
                Uint32 gmask = 0x0000ff00;
                Uint32 bmask = 0x00ff0000;
                Uint32 amask = 0xff000000;
                render_surface = SDL_CreateRGBSurface(flags, w, h, depth, rmask, gmask, bmask, amask);

                if (!render_surface) {
                        throw std::runtime_error(SDL_GetError());
                }
        } catch (const std::exception& e) {
                SDL_Quit();
                throw e;
        }
}

film::~film()
{
        SDL_FreeSurface(render_surface);
        SDL_Quit();
}

bool film::is_open()
{
        return true;
}

void film::draw(int x, int y, int w, int h, const nex::color* pixels)
{
        if ((x > render_surface->w) || (y > render_surface->h)) {
                return;
        }

        int num_pixels = w * h;

        std::vector<uint32_t> src(num_pixels, 0);
        for (int i = 0; i < num_pixels; ++i) {
                src[i] = quantize(gamma_correct(pixels[i], GAMMA_VALUE));
        }

        std::lock_guard<std::mutex> lock(pixel_mutex);

        if (SDL_LockSurface(render_surface) != 0) {
                throw std::runtime_error(SDL_GetError());
        }

        uint32_t* dst = static_cast<uint32_t*>(render_surface->pixels);

        for (int i = 0; i < h; ++i) {
                memcpy(&dst[x + (y + i) * render_surface->w],
                        &src[i * w],
                        sizeof(uint32_t) * w);
        }

        SDL_UnlockSurface(render_surface);
}

void film::save(const std::string& file)
{
        if (SDL_SaveBMP(render_surface, file.c_str()) != 0) {
                throw std::runtime_error(SDL_GetError());
        }
}

static nex::color gamma_correct(const nex::color& color, float gamma)
{
        float power = 1.0f / gamma;

        return nex::color(std::pow(std::max(color.r, 0.0f), power),
                          std::pow(std::max(color.g, 0.0f), power),
                          std::pow(std::max(color.b, 0.0f), power),
                          1.0f);
}

static int quantize(const nex::color& color)
{
        int r = nex::clamp(static_cast<int>(color.r * 255.0f + 0.5f), 0, 255);
        int g = nex::clamp(static_cast<int>(color.g * 255.0f + 0.5f), 0, 255);
        int b = nex::clamp(static_cast<int>(color.b * 255.0f + 0.5f), 0, 255);
        int a = 255;

        return (a << 24) | (b << 16) | (g << 8) | r;
}
}
