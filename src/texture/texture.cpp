#include <cmath>
#include <IL\il.h>
#include <IL\ilu.h>
#include <image.h>
#include <stdexcept>
#include <texture.h>

namespace lumen {
texture::texture(const std::string& file) :
        texels(6),
        width_(0),
        height_(0)
{
        if (!ilLoadImage(file.c_str())) {
                throw std::invalid_argument(iluErrorString(ilGetError()));
        }

        ilConvertImage(IL_RGB, IL_FLOAT);
        iluBuildMipmaps();

        ILuint img = ilGetInteger(IL_CUR_IMAGE);
        ILint width = ilGetInteger(IL_IMAGE_WIDTH);
        ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
        ILint num_faces = ilGetInteger(IL_NUM_FACES);

        std::unique_ptr<ILfloat[]> data(new ILfloat[width * height * 3]);

        int stride = 3 * width;

        for (ILint face = 0; face <= num_faces; ++face) {
                ilBindImage(img);
                ilActiveFace(face);

                texels[face].reserve(width * height);

                ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_FLOAT, data.get());

                for (ILint i = 0; i < height; ++i) {
                        for (ILint j = 0; j < width; ++j) {
                                int offset = i * stride + 3 * j;

                                // texture colors are already gamma corrected so each color
                                // needs to have the inverse gamma applied otherwise the image
                                // will gamma correct the colors a second time and saturate
                                // the textures
                                nex::color texel(
                                        std::powf(data[offset + 0], GAMMA_VALUE),
                                        std::powf(data[offset + 1], GAMMA_VALUE),
                                        std::powf(data[offset + 2], GAMMA_VALUE),
                                        1.0f);

                                texels[face].push_back(texel);
                        }
                }
        }

        ilDeleteImages(1, &img);

        width_ = width;
        height_ = height;
}

nex::color texture::fetch_2d(int x, int y)
{
        return texels[0][x + y * width_];
}

nex::color texture::fetch_cube(int face, int x, int y)
{
        return texels[face][x + y * width_];
}

int texture::width() const
{
        return width_;
}

int texture::height() const
{
        return height_;
}
}
