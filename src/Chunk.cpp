//
// Created by lucas on 10/02/2026.
//

#include "Chunk.h"

#include <cassert>

void Chunk::SetVoxel(const int x, const int y, const int z, const uint8_t type) {

    const int voxelIndex = GetIndex(x,y,z);

    #ifdef NDEBUG
        voxels[voxelIndex].type = type;
    #else
        assert(InBounds(x,y,z));
        voxels[voxelIndex].type = type;
    #endif
}

Voxel Chunk::GetVoxel(const int x, const int y, const int z) const {

    const int voxelIndex = GetIndex(x,y,z);
    return voxels[voxelIndex];
}

const std::vector<float> & Chunk::GetVertices() const
{
    return vertices;
}

const std::vector<unsigned int> & Chunk::GetIndices() const
{
    return indices;
}

void Chunk::CreateMesh() {

    vertices.clear();
    indices.clear();

    vertices.reserve(CHUNK_VOLUME * 24);
    indices.reserve(CHUNK_VOLUME * 36);

    int vertexOffset = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                const int voxelIndex = GetIndex(x,y,z);

                if (voxels[voxelIndex].type == Voxels::AIR_VOXEL)
                    continue;

                //We loop 8 times because a cube has 8 vertices
                for (int i = 0; i < 8; i++)
                {
                    //Each vertex has 3 components: X, Y, Z
                    //We multiply by 3 to jump through the VoxelVertices array correctly
                    float vX = Voxels::VoxelVertices[i * 3 + 0] + x;
                    float vY = Voxels::VoxelVertices[i * 3 + 1] + y;
                    float vZ = Voxels::VoxelVertices[i * 3 + 2] + z;

                    //Push these calculated world positions into the big buffer
                    vertices.emplace_back(vX);
                    vertices.emplace_back(vY);
                    vertices.emplace_back(vZ);
                }

                //Push indices
                for (int i = 0; i < 36; i++)
                {
                    indices.emplace_back(Voxels::VoxelIndices[i] + vertexOffset);
                }

                vertexOffset += 8;
            }
        }
    }
}

bool Chunk::InBounds(const int x, const int y, const int z) {

    return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
}

int Chunk::GetIndex(const int x, const int y, const int z) {

    #ifdef NDEBUG
        return x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE;
    #else
        assert(InBounds(x,y,z));
        return x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE;
    #endif
}
