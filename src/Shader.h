//
// Created by lucas on 08/02/2026.
//
#pragma once
#include <glm/glm.hpp>


class Shader {
public:
    Shader();
    void Bind() const;
    void SetMat4(const char* name, const glm::mat4& mat);

private:
    unsigned int ID;
};
