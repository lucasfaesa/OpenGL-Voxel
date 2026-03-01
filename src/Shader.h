//
// Created by lucas on 08/02/2026.
//
#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    Shader(const char* vertexSource, const char* fragmentSource);

    void Bind() const;
    void SetMat4(const char* name, const glm::mat4& mat);
    void SetInt(const std::string& name, int value);

    ~Shader() {
        glDeleteProgram(ID);
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

private:
    unsigned int ID;
};