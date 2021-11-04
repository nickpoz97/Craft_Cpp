//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <array>
#include <thread>
#include <unordered_map>
#include <list>

#include "glm/vec2.hpp"
#include "Geometry/map_utils.hpp"
#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"
#include "tunableParameters.hpp"

namespace CraftCpp {
class Chunk;

using ChunkMap = std::unordered_map<glm::ivec2, Chunk>;

class Chunk : public RenderableEntity<CubeVertex> {
private:
    mutable std::thread initChunkThread{};
    using SuperClass = RenderableEntity<CubeVertex>;
    using BufferType = std::vector<CubeVertex>;

    using BlockMap = std::unordered_map<glm::ivec3, BlockType>;
    mutable BufferType localBuffer;
    BlockMap blockMap{};

    mutable bool renderReady{false};
    mutable bool localBufferReady{false};

    int countExposedFaces() const;

    BufferType::iterator
    generateBlockGeometry(BufferType::iterator vertexIt, const glm::ivec3 &blockPos, TileBlock tileBlock,
                          const std::array<bool, 6> &visibleFaces) const;

    void computeChunkGeometry() const;

    std::array<glm::ivec3, 8> getChunkBoundaries() const;

    const std::array<glm::ivec2, 4> xzBoundaries;

    bool checkBorder(const glm::ivec3 &pos, const ::glm::ivec3 &direction) const;

    std::array<bool, 6> getVisibleFaces(TileBlock w, const glm::ivec3 &pos) const;

public:
    explicit Chunk(const glm::ivec2 &pqCoordinates);
    static constexpr int SIZE = CHUNK_SIZE;
    static constexpr int Y_LIMIT = 256;

    const glm::ivec2 pq;

    int getMinX() const;

    int getMinZ() const;

    int getMaxX() const;

    int getMaxZ() const;

    static std::array<glm::ivec2, 4> computeXZBoundaries(const glm::ivec2 &pq);

    static glm::ivec2 getMinXz(const glm::ivec2 &pq);

    static glm::ivec2 getMaxXz(const glm::ivec2 &pq);

    int getHighestBlock() const;

    BlockType getBlock(const glm::ivec3 &block_pos) const;

    void setBlock(const glm::ivec3 &position, BlockType w);

    bool isVisible(const glm::mat4 &viewproj) const;

    void updateBuffer() const;

    bool isLocalBufferReady() const;

    // thread not running and not initialized
    bool needInitCall() const;

    static glm::ivec2 chunked(const glm::vec3 &position);

    static int chunked(int val);

    void renderObject() const;

    void initChunk();

    void generateBlockmap();

    // first is top-left, going counterclockwise
    std::unordered_map<glm::ivec3, bool> getLightObstacles(const glm::ivec3 &blockPos) const;

    void waitThread() const;

    bool isOnBorder(const glm::ivec3 &pos) const;
};

int getChunkDistance(const glm::ivec2 &pq1, const glm::ivec2 &pq2);

}

#endif //CPP_CHUNK_HPP
