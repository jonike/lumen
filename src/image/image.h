#ifndef LUMEN_IMAGE_H
#define LUMEN_IMAGE_H

#include <mutex>
#include <string>
#include <SDL.h>
#include "nex\color.h"

namespace lumen {
extern const float GAMMA_VALUE;

class image {
public:
        image(int w, int h);
        ~image();

        bool is_open();
        void draw(int x, int y, int w, int h, const nex::color* pixels);
        void save(const std::string& file);
private:
        SDL_Window* window;
        SDL_Surface* window_surface;
        SDL_Surface* render_surface;
        std::mutex pixel_mutex;
};
}

#endif
