//
// Created by lucas on 19/02/2026.
//

#ifndef OPENGL_STUDY_CHUNKPOS_H
#define OPENGL_STUDY_CHUNKPOS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>

struct ChunkPos
{
    int x;
    int z;

    bool operator==(ChunkPos const& other) const
    {
        return this->x == other.x && this->z == other.z;
    }

    bool operator!=(ChunkPos const& other) const
    {
        return this->x != other.x || this->z != other.z;
    }
};

namespace std
{
    template<> struct hash<ChunkPos>
    {
        size_t operator()(const ChunkPos& p) const // This 'operator()' allows the struct to be used like a function: myHash(p)
        {
            // 1. We pack your two ints (x, z) into a single GLM vector.
            // 2. We then use GLM's built-in hash logic for that vector.
            // 3. This 'mixes' the bits of X and Z together to create one unique ID.
            return hash<glm::ivec2>{}(glm::ivec2(p.x, p.z));
        }
    };
}

#endif //OPENGL_STUDY_CHUNKPOS_H