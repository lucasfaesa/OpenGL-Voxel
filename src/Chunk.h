//
// Created by lucas on 10/02/2026.
//

#ifndef OPENGL_STUDY_CHUNK_H
#define OPENGL_STUDY_CHUNK_H
#include <vector>

#include "Voxel.h"

struct NeighborsData;

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
    const NeighborsData GetNeighborsData(const int x, const int y, const int z) const;
    void AddFaceData(const float faceTemplate[12], const float color[3], int x, int y, int z, unsigned int& offset);
};


struct NeighborsData
{
    bool hasTopNeighbor = false;
    bool hasBottomNeighbor = false;
    bool hasFrontNeighbor = false;
    bool hasBackNeighbor = false;
    bool hasLeftNeighbor = false;
    bool hasRightNeighbor = false;
};


#endif //OPENGL_STUDY_CHUNK_H