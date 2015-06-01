#include <IL\il.h>
#include <IL\ilu.h>
#include <texture.h>
#include <texture_cache.h>

namespace lumen {
std::map<std::string, texture_ptr> texture_cache::textures;

void texture_cache::init()
{
        ilInit();
        iluInit();
}

void texture_cache::shutdown()
{
}

texture_ptr texture_cache::load(const std::string& file)
{
        auto iter = textures.find(file);

        if (iter != textures.end()) {
                return iter->second;
        }

        texture_ptr tex(new texture(file));

        textures.insert(std::pair<std::string, texture_ptr>(file, tex));

        return tex;
}
}
