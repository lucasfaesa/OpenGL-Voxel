//
// Created by lucas on 08/02/2026.
//
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>


class Shader {
public:
    Shader();
    void Bind() const;
    void SetMat4(const char* name, const glm::mat4& mat);

    ~Shader() {
        glDeleteProgram(ID);
    }

    //delete copy constructor, "Objects of thus type are not allowed to be copied"
    Shader(const Shader&) = delete;

    //changing copy assignment operator "="
    //Once deleted, any attempt to copy the object is a compile-time error, not a runtime one.
    Shader& operator=(const Shader&) = delete;

private:
    unsigned int ID;
};
