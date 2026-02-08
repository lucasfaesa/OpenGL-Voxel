//
// Created by lucas on 07/02/2026.
//
#pragma once

class Mesh {
public:
    Mesh();
    void Draw() const;

private:
    unsigned int VAO, VBO, EBO;
};

