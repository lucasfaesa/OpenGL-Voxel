//
// Created by lucas on 19/02/2026.
//

#include "World.h"

#include "Logger.h"

ChunkPos World::GetChunkPosFromWorld(const glm::vec3 playerPos)
{
    //bit shifting, same as playerPos.x / static_cast<float>(Chunk::CHUNK_SIZE)) but faster
    int chunkX = static_cast<int>(playerPos.x) >> 4;
    int chunkZ= static_cast<int>(playerPos.z) >> 4;

    return {chunkX, chunkZ};
}

Chunk* World::GetChunkFromChunkPos(const ChunkPos pos)
{
    auto iterator = chunks_map.find(pos);

    if (iterator != chunks_map.end())
    {
        // return the raw pointer managed by the unique_ptr
        return iterator->second.get();
    }

    LOG_ERROR("Could not find chunk");
    return nullptr; //no chunks found at this address
}

void World::UpdateChunks(glm::vec3 playerPos)
{
    ChunkPos playerCurrentChunkPos = GetChunkPosFromWorld(playerPos);

    //looping through the square area around the player
    for (int x = -renderDistance; x <= renderDistance; x++)
    {
        for (int z = -renderDistance; z < renderDistance; z++)
        {
            //specific address we looking at
            ChunkPos targetPos = {playerCurrentChunkPos.x + x, playerCurrentChunkPos.z + z};

            //check if we already have this chunk
            if (chunks_map.find(targetPos) == chunks_map.end())
            {
                //it doesnt exist, lets build it
                //std::unique_ptr<Chunk> newChunk = std::make_unique<Chunk>();
                std::unique_ptr<Chunk> newChunk = RequestChunk();

                newChunk->SetPosition(targetPos);

                GenerateNoiseVoxels(newChunk, targetPos);


                newChunk->CreateMesh();

                //putting on the map
                chunks_map[targetPos] = std::move(newChunk);
            }
        }
    }
}

std::unique_ptr<Chunk> World::RequestChunk()
{
    if (chunk_pool.empty())
    {
        LOG_INFO("Pool empty. Allocating NEW chunk memory.");
        return std::make_unique<Chunk>();
    }
    else
    {
        LOG_INFO("Pool hit! Reusing chunk from graveyard. Current Pool Size: %d", (int)chunk_pool.size() - 1);
        std::unique_ptr<Chunk> chunk = std::move(chunk_pool.back());
        chunk_pool.pop_back();

        chunk->ClearData();

        return chunk;
    }
}

void World::RecycleChunk(std::unique_ptr<Chunk> chunk)
{
    chunk_pool.push_back(std::move(chunk));
    LOG_INFO("Chunk recycled to pool. Current Pool Size: %d", (int)chunk_pool.size());
}

void World::GenerateNoiseVoxels(std::unique_ptr<Chunk>& chunk, const ChunkPos &chunkPos)
{
    // 1. Horizontal loops (The "Floor Plan")
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {

            // Calculate Global World Position (Topic 1)
            int voxelWorldPosX = (chunkPos.x * Chunk::CHUNK_SIZE) + x;
            int voxelWorldPosZ = (chunkPos.z * Chunk::CHUNK_SIZE) + z;

            //making the world look a bit better
            fastNoiseLite.SetSeed(157239);
            fastNoiseLite.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
            fastNoiseLite.SetFrequency(0.01f);
            fastNoiseLite.SetFractalType(FastNoiseLite::FractalType_FBm);
            fastNoiseLite.SetFractalOctaves(3);
            fastNoiseLite.SetFractalGain(0.15f);

            float noiseValue = fastNoiseLite.GetNoise((float)voxelWorldPosX, (float)voxelWorldPosZ);
            // 2. Normalize to 0.0 ... 1.0
            float normalizedNoise = (noiseValue + 1.0f) * 0.5f;
            // 3. Apply redistribution (Power function)
            // Squaring it makes the low areas (valleys) much broader
            // and the high areas (peaks) more sudden.
            float shapedNoise = std::pow(normalizedNoise, 2.0f);

            // 4. Scale to chunk height, so it doesnt overflow our 16 voxels height
            int finalHeight = static_cast<int>(shapedNoise * (Chunk::CHUNK_SIZE - 1));

            // Safety floor, so it doesnt go below 1 and make craters on the floor
            if (finalHeight < 1) finalHeight = 1;

            // 3. THE Y LOOP (The "Construction")
            // This fills the volume from 0 up to the height
            for (int y = 0; y < finalHeight; y++) {
                chunk->SetVoxel(x, y, z, Voxels::SOLID_VOXEL);
            }
        }
    }
}

void World::CleanupChunks(glm::vec3 playerPos)
{
    ChunkPos playerChunkPtr = GetChunkPosFromWorld(playerPos);

    auto iterator = chunks_map.begin();

    while (iterator != chunks_map.end())
    {
        ChunkPos pos = iterator->first;

        // Calculate distance from the player to this chunk
        int distX = std::abs(pos.x - playerChunkPtr.x);
        int distZ = std::abs(pos.z - playerChunkPtr.z);

        //Check if its outside the interest zone, renderDistance +1 as a safety buffer (just for it to not unload when the player flickers between chunks)
        if (distX > renderDistance +1 || distZ > renderDistance +1)
        {
            std::unique_ptr<Chunk> chunkToPool = std::move(iterator->second);

            iterator = chunks_map.erase(iterator); //erase() returns the iterator to the NEXT element

            RecycleChunk(std::move(chunkToPool));
        }else
        {
            ++iterator;
        }
    }
}

void World::Render(const Frustum& frustum)
{
    for (auto&[pos, chunk] : chunks_map)
    {
        glm::vec3 minBound = glm::vec3(pos.x * Chunk::CHUNK_SIZE, 0, pos.z * Chunk::CHUNK_SIZE);
        glm::vec3 maxBound = minBound + glm::vec3(Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE);

        if (frustum.IsBoxVisible(minBound, maxBound))
        {
            Mesh* mesh = chunk->GetMesh();
            if (mesh)
            {
                mesh->Draw();
            }
        }

    }
}

void World::Cleanup()
{
    chunks_map.clear();
    chunk_pool.clear();
}



