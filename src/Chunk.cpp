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

float Chunk::GetFaceLight(Voxels::FaceDirection direction)
{
    switch (direction) {
        case Voxels::FaceDirection::Up:    return 1.0f;  // Top (Direct Sun)
        case Voxels::FaceDirection::Front:
        case Voxels::FaceDirection::Back:  return 0.85f; // Side A
        case Voxels::FaceDirection::Left:
        case Voxels::FaceDirection::Right: return 0.70f; // Side B
        case Voxels::FaceDirection::Down:  return 0.45f; // Bottom (Darkest)
        default: return 1.0f;
    }
}

void Chunk::ClearData()
{
    // Reset all voxels to AIR
    std::fill(voxels.begin(), voxels.end(), Voxel{Voxels::AIR_VOXEL});

    vertices.clear();
    indices.clear();

    if (mesh)
    {
        mesh->UpdateData(vertices, indices);
    }

}

void Chunk::SetPosition(const ChunkPos pos)
{
    position = pos;
}

Mesh* Chunk::GetMesh() const
{
    return mesh.get();
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
                std::array<uint8_t, 4> aoValues {};

                if (!currentVoxelNeighbors.hasTopNeighbor)
                {
                    aoValues = CheckAmbientOcclusion(x,y,z, Voxels::FaceDirection::Up);
                    AddFaceData(Voxels::TopFace, Voxels::TopColor, aoValues, Voxels::FaceDirection::Up, x, y, z, vertexOffset);
                }
                if (!currentVoxelNeighbors.hasBottomNeighbor)
                {
                    aoValues = CheckAmbientOcclusion(x,y,z, Voxels::FaceDirection::Down);
                    AddFaceData(Voxels::BottomFace, Voxels::BottomColor, aoValues, Voxels::FaceDirection::Down, x, y, z, vertexOffset);
                }
                if (!currentVoxelNeighbors.hasLeftNeighbor)
                {
                    aoValues = CheckAmbientOcclusion(x,y,z, Voxels::FaceDirection::Left);
                    AddFaceData(Voxels::LeftFace, Voxels::LeftColor, aoValues, Voxels::FaceDirection::Left, x, y, z, vertexOffset);
                }
                if (!currentVoxelNeighbors.hasRightNeighbor)
                {
                    aoValues = CheckAmbientOcclusion(x,y,z, Voxels::FaceDirection::Right);
                    AddFaceData(Voxels::RightFace, Voxels::RightColor, aoValues, Voxels::FaceDirection::Right, x, y, z, vertexOffset);
                }
                if (!currentVoxelNeighbors.hasFrontNeighbor)
                {
                    aoValues = CheckAmbientOcclusion(x,y,z, Voxels::FaceDirection::Front);
                    AddFaceData(Voxels::FrontFace, Voxels::FrontColor, aoValues, Voxels::FaceDirection::Front, x, y, z, vertexOffset);
                }
                if (!currentVoxelNeighbors.hasBackNeighbor)
                {
                    aoValues = CheckAmbientOcclusion(x,y,z, Voxels::FaceDirection::Back);
                    AddFaceData(Voxels::BackFace, Voxels::BackColor, aoValues, Voxels::FaceDirection::Back, x, y, z, vertexOffset);
                }
            }
        }
    }

    if (!mesh)
    {
        mesh = std::make_unique<Mesh>();
    }

    mesh->UpdateData(vertices, indices);
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

void Chunk::AddFaceData(const float faceTemplate[12], const float color[3], const std::array<uint8_t, 4> aoValues,Voxels::FaceDirection direction, int x, int y, int z, unsigned int& offset)
{
    // Determine the light intensity based on face direction
    float faceLight = 1.0f;
    if (direction == Voxels::FaceDirection::Up)         faceLight = 1.0f;  // Direct sunlight
    else if (direction == Voxels::FaceDirection::Front) faceLight = 0.8f;  // Sun hitting the front
    else if (direction == Voxels::FaceDirection::Left)  faceLight = 0.7f;  // Sun hitting the side
    else if (direction == Voxels::FaceDirection::Right) faceLight = 0.6f;  // Shadow side
    else if (direction == Voxels::FaceDirection::Back)  faceLight = 0.4f;  // Shadow side
    else if (direction == Voxels::FaceDirection::Down)  faceLight = 0.2f;  // Bottom

    for (int i = 0; i < 4; i++)
    {
        float worldX = static_cast<float>(x + position.x * CHUNK_SIZE);
        float worldZ = static_cast<float>(z + position.z * CHUNK_SIZE);

        // 1-3. Position
        vertices.emplace_back(faceTemplate[i*3+0] + worldX);
        vertices.emplace_back(faceTemplate[i*3+1] + static_cast<float>(y));
        vertices.emplace_back(faceTemplate[i*3+2] + worldZ);

        // 4. Ambient Occlusion (normalized)
        vertices.emplace_back(static_cast<float>(aoValues[i]) / 3.0f);

        // 5. Face Lighting (New Float)
        vertices.emplace_back(faceLight);
    }

    // Indices logic (same as your original)
    if (aoValues[0] + aoValues[2] > aoValues[1] + aoValues[3]) {
        indices.emplace_back(offset + 0);
        indices.emplace_back(offset + 1);
        indices.emplace_back(offset + 2);
        indices.emplace_back(offset + 2);
        indices.emplace_back(offset + 3);
        indices.emplace_back(offset + 0);
    } else {
        indices.emplace_back(offset + 1);
        indices.emplace_back(offset + 2);
        indices.emplace_back(offset + 3);
        indices.emplace_back(offset + 3);
        indices.emplace_back(offset + 0);
        indices.emplace_back(offset + 1);
    }
    offset += 4;
}

const std::array<uint8_t, 4> Chunk::CheckAmbientOcclusion(const int x, const int y, const int z, Voxels::FaceDirection dir) const
{
    bool n_back, n_front, n_left, n_right;
    bool n_backLeft, n_backRight, n_frontLeft, n_frontRight;
    bool n_up, n_down;
    bool n_upLeft, n_upRight, n_downLeft, n_downRight;

    auto ao = [&](bool side1, bool side2, bool corner) -> uint8_t
    {
        return (side1 && side2) ? 0 : (3 - (side1 + side2 + corner));
    };

    switch (dir)
    {
        case Voxels::FaceDirection::Up:
        {
            n_back       = IsSolid(x,     y + 1, z + 1);
            n_front      = IsSolid(x,     y + 1, z - 1);
            n_left       = IsSolid(x - 1, y + 1, z);
            n_right      = IsSolid(x + 1, y + 1, z);
            n_backLeft   = IsSolid(x - 1, y + 1, z + 1);
            n_backRight  = IsSolid(x + 1, y + 1, z + 1);
            n_frontLeft  = IsSolid(x - 1, y + 1, z - 1);
            n_frontRight = IsSolid(x + 1, y + 1, z - 1);

            return {
                ao(n_left,  n_back,  n_backLeft),
                ao(n_back,  n_right, n_backRight),
                ao(n_front, n_right, n_frontRight),
                ao(n_front, n_left,  n_frontLeft)
            };
        }

        case Voxels::FaceDirection::Down:
        {
            n_back       = IsSolid(x,     y - 1, z + 1);
            n_front      = IsSolid(x,     y - 1, z - 1);
            n_left       = IsSolid(x - 1, y - 1, z);
            n_right      = IsSolid(x + 1, y - 1, z);
            n_backLeft   = IsSolid(x - 1, y - 1, z + 1);
            n_backRight  = IsSolid(x + 1, y - 1, z + 1);
            n_frontLeft  = IsSolid(x - 1, y - 1, z - 1);
            n_frontRight = IsSolid(x + 1, y - 1, z - 1);

            return {
                ao(n_left,  n_front, n_frontLeft), // 0: (0,0,0)
                ao(n_front, n_right, n_frontRight),// 1: (1,0,0)
                ao(n_right, n_back,  n_backRight), // 2: (1,0,1)
                ao(n_back,  n_left,  n_backLeft)   // 3: (0,0,1)
            };
        }

        case Voxels::FaceDirection::Back: //+Z
        {
            // All neighbors MUST be at z + 1 to check the "air" in front of the face
            n_left  = IsSolid(x - 1, y,     z + 1);
            n_right = IsSolid(x + 1, y,     z + 1);
            n_up    = IsSolid(x,     y + 1, z + 1);
            n_down  = IsSolid(x,     y - 1, z + 1);

            bool n_upLeft    = IsSolid(x - 1, y + 1, z + 1);
            bool n_upRight   = IsSolid(x + 1, y + 1, z + 1);
            bool n_downLeft  = IsSolid(x - 1, y - 1, z + 1);
            bool n_downRight = IsSolid(x + 1, y - 1, z + 1);

            return {
                ao(n_left,  n_down, n_downLeft),  // 0: Bottom-Left
                ao(n_left,  n_up,   n_upLeft),    // 1: Top-Left
                ao(n_up,    n_right, n_upRight),   // 2: Top-Right
                ao(n_down,  n_right, n_downRight)  // 3: Bottom-Right
            };
        }

        case Voxels::FaceDirection::Front: // -Z
        {
            // These neighbors are relative to the block at (x, y, z-1)
            n_left       = IsSolid(x - 1, y,     z - 1);
            n_right      = IsSolid(x + 1, y,     z - 1);
            n_up         = IsSolid(x,     y + 1, z - 1);
            n_down       = IsSolid(x,     y - 1, z - 1);

            // Corners
            n_upLeft    = IsSolid(x - 1, y + 1, z - 1);
            n_upRight   = IsSolid(x + 1, y + 1, z - 1);
            n_downLeft  = IsSolid(x - 1, y - 1, z - 1);
            n_downRight = IsSolid(x + 1, y - 1, z - 1);

            return {
                ao(n_right, n_down, n_downRight), // 0: Bottom-Right (1,0,1)
                ao(n_right, n_up,   n_upRight),    // 1: Top-Right (1,1,1)
                ao(n_left,  n_up,   n_upLeft),     // 2: Top-Left (0,1,1)
                ao(n_left,  n_down, n_downLeft)    // 3: Bottom-Left (0,0,1)
            };
        }

        case Voxels::FaceDirection::Right:
        {
            n_back  = IsSolid(x + 1, y,     z + 1);
            n_front = IsSolid(x + 1, y,     z - 1);
            n_up    = IsSolid(x + 1, y + 1, z);
            n_down  = IsSolid(x + 1, y - 1, z);

            bool n_upBack    = IsSolid(x + 1, y + 1, z + 1);
            bool n_upFront   = IsSolid(x + 1, y + 1, z - 1);
            bool n_downBack  = IsSolid(x + 1, y - 1, z + 1);
            bool n_downFront = IsSolid(x + 1, y - 1, z - 1);

            return {
                ao(n_front, n_down, n_downFront), // 0: Front-Bottom (1,0,0)
                ao(n_front, n_up,   n_upFront),   // 1: Front-Top (1,1,0)
                ao(n_back,  n_up,   n_upBack),    // 2: Back-Top (1,1,1)
                ao(n_back,  n_down, n_downBack)   // 3: Back-Bottom (1,0,1)
            };
        }

        case Voxels::FaceDirection::Left:
        {
            n_back  = IsSolid(x - 1, y,     z + 1);
            n_front = IsSolid(x - 1, y,     z - 1);
            n_up    = IsSolid(x - 1, y + 1, z);
            n_down  = IsSolid(x - 1, y - 1, z);

            bool n_upBack    = IsSolid(x - 1, y + 1, z + 1);
            bool n_upFront   = IsSolid(x - 1, y + 1, z - 1);
            bool n_downBack  = IsSolid(x - 1, y - 1, z + 1);
            bool n_downFront = IsSolid(x - 1, y - 1, z - 1);

            return {
                ao(n_back,  n_down, n_downBack),  // 0: Back-Bottom (0,0,1)
                ao(n_back,  n_up,   n_upBack),    // 1: Back-Top (0,1,1)
                ao(n_front, n_up,   n_upFront),   // 2: Front-Top (0,1,0)
                ao(n_front, n_down, n_downFront)  // 3: Front-Bottom (0,0,0)
            };
        }
    }

    return {3, 3, 3, 3};
}


bool Chunk::IsSolid(const int x, const int y, const int z) const
{
    if (!InBounds(x,y,z)) return false;

    return GetVoxel(x,y,z).type == Voxels::SOLID_VOXEL;
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
