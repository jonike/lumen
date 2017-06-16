#ifndef LUMEN_RENDERWINDOW_H
#define LUMEN_RENDERWINDOW_H

#include "film.h"

namespace lumen {
extern const float GAMMA_VALUE;

class render_window : public film {
public:
        render_window(int w, int h);
        virtual ~render_window();

        virtual bool is_open();
        virtual void draw(int x, int y, int w, int h, const nex::color* pixels);
private:
        SDL_Window* window;
        SDL_Surface* window_surface;
};
}

#endif
