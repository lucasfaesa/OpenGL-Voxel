//
// Created by lucas on 07/02/2026.
//
#pragma once
#include <vector>
#include <glad/glad.h>

class Mesh {
public:
    Mesh();
    void Draw() const;

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void UpdateData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    //no copy constructor
    Mesh(const Mesh&) = delete;
    //no copy assignment
    Mesh& operator=(const Mesh&) = delete;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount = 36;
};

