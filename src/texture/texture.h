#ifndef LUMEN_TEXTURE_H
#define LUMEN_TEXTURE_H

#include <color.h>
#include <memory>
#include <string>
#include <vector>

namespace lumen {
class texture {
public:
        explicit texture(const std::string& file);

        nex::color fetch_2d(int x, int y);
        nex::color fetch_cube(int face, int x, int y);

        int width() const;
        int height() const;
private:
        std::vector<std::vector<nex::color>> texels;
        int width_;
        int height_;
};

typedef std::shared_ptr<texture> texture_ptr;
}

#endif
