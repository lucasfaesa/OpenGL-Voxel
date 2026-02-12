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

    unsigned int vertexOffset = 0;

    for (int x = 0; x < CHUNK_SIZE; x++){
        for (int y = 0; y < CHUNK_SIZE; y++){
            for (int z = 0; z < CHUNK_SIZE; z++){

                if (GetVoxel(x,y,z).type == Voxels::AIR_VOXEL)
                    continue;

                NeighborsData currentVoxelNeighbors = GetNeighborsData(x,y,z);

                if (!currentVoxelNeighbors.hasTopNeighbor)
                    AddFaceData(Voxels::TopFace, Voxels::TopColor, x, y, z, vertexOffset);

                if (!currentVoxelNeighbors.hasBottomNeighbor)
                    AddFaceData(Voxels::BottomFace, Voxels::BottomColor, x, y, z, vertexOffset);

                if (!currentVoxelNeighbors.hasLeftNeighbor)
                    AddFaceData(Voxels::LeftFace, Voxels::LeftColor, x, y, z, vertexOffset);

                if (!currentVoxelNeighbors.hasRightNeighbor)
                    AddFaceData(Voxels::RightFace, Voxels::RightColor, x, y, z, vertexOffset);

                if (!currentVoxelNeighbors.hasFrontNeighbor)
                    AddFaceData(Voxels::FrontFace, Voxels::FrontColor, x, y, z, vertexOffset);

                if (!currentVoxelNeighbors.hasBackNeighbor)
                    AddFaceData(Voxels::BackFace, Voxels::BackColor, x, y, z, vertexOffset);
            }
        }
    }
}

const NeighborsData Chunk::GetNeighborsData(const int x, const int y, const int z) const
{
    NeighborsData data;

    if (InBounds(x, y+1, z) && GetVoxel(x,y+1,z).type == Voxels::SOLID_VOXEL)
        data.hasTopNeighbor = true;

    if (InBounds(x, y-1, z) && GetVoxel(x,y-1,z).type == Voxels::SOLID_VOXEL)
        data.hasBottomNeighbor = true;

    if (InBounds(x-1, y, z) && GetVoxel(x-1,y,z).type == Voxels::SOLID_VOXEL)
        data.hasLeftNeighbor = true;

    if (InBounds(x+1, y, z) && GetVoxel(x+1,y,z).type == Voxels::SOLID_VOXEL)
        data.hasRightNeighbor = true;

    if (InBounds(x, y, z-1) && GetVoxel(x,y,z-1).type == Voxels::SOLID_VOXEL)
        data.hasFrontNeighbor = true;

    if (InBounds(x, y, z+1) && GetVoxel(x,y,z+1).type == Voxels::SOLID_VOXEL)
        data.hasBackNeighbor = true;

    return data;
}

void Chunk::AddFaceData(const float faceTemplate[12], const float color[3], int x, int y, int z, unsigned int &offset)
{
    //1. Add the 4 vertices for this face, offset by the block's position
    for (int i=0; i<4; i++)
    {
        vertices.emplace_back(faceTemplate[i*3+0] + static_cast<float>(x));
        vertices.emplace_back(faceTemplate[i*3+1] + static_cast<float>(y));
        vertices.emplace_back(faceTemplate[i*3+2] + static_cast<float>(z));

        /*vertices.emplace_back(color[0]);
        vertices.emplace_back(color[1]);
        vertices.emplace_back(color[2]);*/
    }

    //2. add the 6 indices for this face
    for (int i=0; i < 6; i++)
    {
        indices.emplace_back(Voxels::FaceIndices[i] + offset);
    }

    offset += 4;
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
