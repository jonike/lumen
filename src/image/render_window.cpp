#include <exception>
#include "render_window.h"

namespace lumen {
render_window::render_window(int w, int h) :
        film(w, h),
        window(nullptr),
        window_surface(nullptr)
{
        try {
                window = SDL_CreateWindow("lumen",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                w, h,
                                SDL_WINDOW_SHOWN);

                if (!window) {
                        throw std::runtime_error(SDL_GetError());
                }

                window_surface = SDL_GetWindowSurface(window);

                if (!window_surface) {
                        throw std::runtime_error(SDL_GetError());
                }

                if (SDL_UpdateWindowSurface(window) != 0) {
                        throw std::runtime_error(SDL_GetError());
                }
        } catch (const std::exception& e) {
                SDL_DestroyWindow(window);
                SDL_Quit();
                throw e;
        }
}

render_window::~render_window()
{
        SDL_DestroyWindow(window);
}

bool render_window::is_open()
{
        while (true) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                return false;
                        }
                }
        }
}

void render_window::draw(int x, int y, int w, int h, const nex::color* pixels)
{
        film::draw(x, y, w, h, pixels);

        SDL_Rect rect = {x, y, w, h};

        if (SDL_BlitSurface(render_surface, &rect, window_surface, &rect) != 0) {
                throw std::runtime_error(SDL_GetError());
        }

        if (SDL_UpdateWindowSurface(window) != 0) {
                throw std::runtime_error(SDL_GetError());
        }
}
}
