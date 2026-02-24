//
// Created by lucas on 19/02/2026.
//

#ifndef OPENGL_STUDY_WORLD_H
#define OPENGL_STUDY_WORLD_H
#include <memory>
#include <unordered_map>

#include "Chunk.h"
#include "ChunkPos.h"
#include "FastNoiseLite.h"
#include "Frustum.h"

class World
{
public:
    ChunkPos GetChunkPosFromWorld(const glm::vec3 playerPos);
    Chunk* GetChunkFromChunkPos(const ChunkPos pos);

    void UpdateChunks(glm::vec3 playerPos);
    void CleanupChunks(glm::vec3 playerPos);
    void Render(const Frustum& frustum);

private:
    FastNoiseLite fastNoiseLite;
    static constexpr int renderDistance = 16;
    std::unordered_map<ChunkPos, std::unique_ptr<Chunk>> chunks_map;

private:
    void GenerateNoiseVoxels(std::unique_ptr<Chunk>& chunk, const ChunkPos& chunkPos);

};


#endif //OPENGL_STUDY_WORLD_H