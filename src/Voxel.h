//
// Created by lucas on 10/02/2026.
//

#ifndef OPENGL_STUDY_VOXEL_H
#define OPENGL_STUDY_VOXEL_H
#include <cstdint>

namespace Voxels
{
    static constexpr int AIR_VOXEL = 0;
    static constexpr int SOLID_VOXEL = 1;

    static constexpr float VoxelVertices[] = {
        0.0f, 0.0f, 0.0f, // 0: Bottom-front-left
        1.0f, 0.0f, 0.0f, // 1: Bottom-front-right
        1.0f, 1.0f, 0.0f, // 2: Top-front-right
        0.0f, 1.0f, 0.0f, // 3: Top-front-left
        0.0f, 0.0f, 1.0f, // 4: Bottom-back-left
        1.0f, 0.0f, 1.0f, // 5: Bottom-back-right
        1.0f, 1.0f, 1.0f, // 6: Top-back-right
        0.0f, 1.0f, 1.0f  // 7: Top-back-left
    };

    static constexpr unsigned int VoxelIndices[] = {
        // Front face (Z-)
        0, 1, 2,
        2, 3, 0,
        // Right face (X+)
        1, 5, 6,
        6, 2, 1,
        // Back face (Z+)
        7, 6, 5,
        5, 4, 7,
        // Left face (X-)
        4, 0, 3,
        3, 7, 4,
        // Top face (Y+)
        3, 2, 6,
        6, 7, 3,
        // Bottom face (Y-)
        4, 5, 1,
        1, 0, 4
    };
}

struct Voxel {
    std::uint8_t type;
};



#endif //OPENGL_STUDY_VOXEL_H