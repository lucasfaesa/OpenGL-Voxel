//
// Created by lucas on 27/02/2026.
//

#ifndef OPENGL_STUDY_TEXTURE_H
#define OPENGL_STUDY_TEXTURE_H

#include <string>
#include <glad/glad.h>

class Texture {
public:
    unsigned int ID;
    Texture(const std::string& path);
    void Bind(unsigned int unit = 0);
};


#endif //OPENGL_STUDY_TEXTURE_H