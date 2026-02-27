//
// Created by lucas on 10/02/2026.
//

#ifndef OPENGL_STUDY_CHUNK_H
#define OPENGL_STUDY_CHUNK_H
#include <vector>
#include <array>
#include <memory>

#include "ChunkPos.h"
#include "Mesh.h"
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

    float GetFaceLight(Voxels::FaceDirection direction);

    void ClearData();
    void SetPosition(const ChunkPos pos);
    Mesh* GetMesh() const;
    void CreateMesh();

private:
    ChunkPos position;
    std::unique_ptr<Mesh> mesh = nullptr;

    std::vector<Voxel> voxels;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    static bool InBounds(int x, int y, int z);
    static int GetIndex(int x, int y, int z);
    const NeighborsData GetNeighborsData(const int x, const int y, const int z) const;
    void AddFaceData(const float faceTemplate[12], const float color[3], const std::array<uint8_t, 4> aoValues,Voxels::FaceDirection direction, int x, int y, int z, unsigned int& offset);
    const std::array<uint8_t, 4> CheckAmbientOcclusion(const int x, const int y, const int z, Voxels::FaceDirection dir) const;
    bool IsSolid (const int x, const int y, const int z) const;
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