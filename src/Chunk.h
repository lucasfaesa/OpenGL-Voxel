//
// Created by lucas on 10/02/2026.
//

#ifndef OPENGL_STUDY_CHUNK_H
#define OPENGL_STUDY_CHUNK_H
#include <vector>

#include "Voxel.h"


class Chunk {
public:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    Chunk() {
        // Pre-allocate the full size immediately
        voxels.resize(CHUNK_VOLUME);
    }

    void SetVoxel(int x, int y, int z, uint8_t type);
    Voxel GetVoxel(int x, int y, int z) const;

    const std::vector<float>& GetVertices() const;
    const std::vector<unsigned int>& GetIndices() const;

    void CreateMesh();

private:
    std::vector<Voxel> voxels;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    static bool InBounds(int x, int y, int z);
    static int GetIndex(int x, int y, int z);
};


#endif //OPENGL_STUDY_CHUNK_H