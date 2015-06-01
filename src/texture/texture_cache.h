#ifndef LUMEN_TEXTURECACHE_H
#define LUMEN_TEXTURECACHE_H

#include <map>
#include <texture.h>

namespace lumen {
class texture_cache {
public:
        static void init();
        static void shutdown();

        static texture_ptr load(const std::string& file);
private:
        static std::map<std::string, texture_ptr> textures;
};
}

#endif
