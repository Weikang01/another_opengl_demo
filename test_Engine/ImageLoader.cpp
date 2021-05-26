#include "ImageLoader.h"
#include "stb_image.h"
#include "Errors.h"
#include <iostream>

namespace test_Engine
{
    GLTexture ImageLoader::loadImage(const GLchar* path)
    {
        GLTexture texture = {};

        int t_width, t_height, t_nrChannels;
        unsigned char* data = stbi_load(path, &t_width, &t_height, &t_nrChannels, 0);

        if (data)
        {
            GLenum colorChannelArray[]{ GL_FALSE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
            glGenTextures(1, &texture.id);
            glBindTexture(GL_TEXTURE_2D, texture.id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArray[t_nrChannels], t_width, t_height, 0,
                colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            fatalError("Failed to load image " + std::string(path));
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }
}