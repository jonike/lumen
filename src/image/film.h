#ifndef LUMEN_FILM_H
#define LUMEN_FILM_H

#include <mutex>
#include <string>
#include <SDL.h>
#include "nex\color.h"

namespace lumen {
extern const float GAMMA_VALUE;

class film {
public:
        film(int w, int h);
        virtual ~film();

        virtual bool is_open();
        virtual void draw(int x, int y, int w, int h, const nex::color* pixels);
        void save(const std::string& file);
protected:
        SDL_Surface* render_surface;
private:
        std::mutex pixel_mutex;
};
}

#endif
