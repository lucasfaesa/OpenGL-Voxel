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
                std::unique_ptr<Chunk> newChunk = std::make_unique<Chunk>();

                newChunk->SetPosition(targetPos);

                //pyramid base
                for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
                    for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                        newChunk->SetVoxel(x, 0, z, Voxels::SOLID_VOXEL);
                    }
                }

                // 2. Build the pyramid layers starting from y=1
                for (int y = 1; y < Chunk::CHUNK_SIZE/2; y++) {
                    int start = y;          // The higher we go, the further in we start
                    int end = Chunk::CHUNK_SIZE - y;       // The higher we go, the earlier we stop

                    for (int x = start; x < end; x++) {
                        for (int z = start; z < end; z++) {
                            newChunk->SetVoxel(x, y, z, Voxels::SOLID_VOXEL);
                        }
                    }
                }

                newChunk->CreateMesh();

                //putting on the map
                chunks_map[targetPos] = std::move(newChunk);
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
        if (distX > renderDistance || distZ > renderDistance)
        {
            iterator = chunks_map.erase(iterator); //erase() returns the iterator to the NEXT element
        }else
        {
            ++iterator;
        }
    }
}

void World::Render()
{
    for (auto&[pos, chunk] : chunks_map)
    {
        Mesh* mesh = chunk->GetMesh();
        if (mesh)
        {
            mesh->Draw();
        }
    }
}
