//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>
#include <iostream>
#include <map>
#include <algorithm>

#include "Geometry/Chunk.hpp"
#include "noise/noise.hpp"
#include "Geometry/CubicObject.hpp"

namespace CraftCpp {
Chunk::Chunk(const glm::ivec2 &pqCoordinates)
        : blockMap{}, pq{pqCoordinates}, SuperClass{},
          xzBoundaries{
                  computeXZBoundaries(pqCoordinates)
          } {
};

int Chunk::getHighestBlock() const {
    int highest_y = -1;

    // choose the highest block in the block_map
    for (const auto &block: blockMap) {
        const glm::ivec3 &block_pos{block.first};
        const TileBlock block_type{block.second};

        if (block_type.isObstacle() && block_pos.y > highest_y)
            highest_y = block_pos.y;
    }
    return highest_y;
}

BlockType Chunk::getBlock(const glm::ivec3 &block_pos) const {
    auto it = blockMap.find(block_pos);
    return (it == blockMap.end()) ? BlockType::EMPTY : it->second;
}

void Chunk::computeChunkGeometry() const {
    int n_faces = countExposedFaces();
    // each visible face has INDICES_FACE_COUNT indices that represent the triangle
    localBuffer = std::vector<CubeVertex>(n_faces * INDICES_FACE_COUNT);
    auto v_it = localBuffer.begin();

    for (const auto &kv: blockMap) {
        const glm::ivec3 &block_pos{kv.first};
        const TileBlock &tileBlock{kv.second};

        // generate geometry of actual block (value returned by function is first free position in buffer)
        v_it = generateBlockGeometry(v_it, block_pos, tileBlock, getVisibleFaces(kv.second, kv.first));
    }
#ifndef NDEBUG
    std::cout << "chunk geometry computed\n";
#endif
    localBufferReady = true;
}

int Chunk::countExposedFaces() const {
    int n_faces = 0;

    for (const auto &pair: blockMap) {
        const auto &visible_faces = getVisibleFaces(pair.second, pair.first);
        n_faces += std::accumulate(visible_faces.begin(), visible_faces.end(), 0);
    }
    return n_faces;
}

Chunk::BufferType::iterator
Chunk::generateBlockGeometry(BufferType::iterator vertexIt, const glm::ivec3 &blockPos, TileBlock tileBlock,
                             const std::array<bool, 6> &visibleFaces) const {
    if (tileBlock.isPlant()) {
        Plant plant{
                tileBlock.getBlockType(),
                blockPos,
                simplex2(blockPos.x, blockPos.z, 4, 0.5, 2) * 360,
                vertexIt,
                getLightObstacles(blockPos),
        };
        return plant.end();
    }
    if (tileBlock.isEmpty() || visibleFaces.empty()) {
        return vertexIt;
    }
    Cube cube{tileBlock.getBlockType(), visibleFaces, blockPos, vertexIt, getLightObstacles(blockPos)};
    return cube.end();
}

void Chunk::updateBuffer() const {
    SuperClass::updateBuffer(localBuffer);
    renderReady = true;
#ifndef NDEBUG
    std::cout << "buffer updated\n";
#endif
}

void Chunk::generateBlockmap() {
    // for each block xz position
    for (int dx = 0; dx < Chunk::SIZE; dx++) {
        for (int dz = 0; dz < Chunk::SIZE; dz++) {
            int x = pq.x * Chunk::SIZE + dx;
            int z = pq.y * Chunk::SIZE + dz;

            float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
            float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
            int mh = g * 32 + 16;
            int h = f * mh;
            BlockType w = BlockType::GRASS;
            int t = 12;
            if (h <= t) {
                h = t;
                w = BlockType::SAND;
                setBlock({x, h - 1, z}, static_cast<BlockType>(w));
            } else {
                for (int y = t - 1; y < h; y++) {
                    setBlock({x, y, z}, static_cast<BlockType>(w));
                }
            }
            if (w == BlockType::GRASS) {
                if (SHOW_PLANTS) {
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        blockMap[{x, h, z}] = static_cast<BlockType>(BlockType::TALL_GRASS);
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        // w_f max == 23
                        int w_f = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                        blockMap[{x, h, z}] = static_cast<BlockType>(w_f);
                    }
                }
                bool ok = SHOW_TREES && !(dx - 4 < 0 || dz - 4 < 0 || dx + 4 >= Chunk::SIZE || dz + 4 >= Chunk::SIZE);
                ok = ok && (simplex2(x, z, 6, 0.5, 2) > 0.84);

                if (ok) {
                    for (int y = h + 3; y < h + 8; y++) {
                        for (int ox = -3; ox <= 3; ox++) {
                            for (int oz = -3; oz <= 3; oz++) {
                                int d = (ox * ox) + (oz * oz) +
                                        (y - (h + 4)) * (y - (h + 4));
                                if (d < 11) {
                                    blockMap[{x + ox, y, z + oz}] = BlockType::LEAVES;
                                }
                            }
                        }
                    }
                    for (int y = h; y < h + 7; y++) {
                        blockMap[{x, y, z}] = BlockType::WOOD;
                    }
                }
            }
            if (SHOW_CLOUDS) {
                for (int y = 64; y < 72; y++) {
                    if (simplex3(
                            x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75) {
                        blockMap[{x, y, z}] = BlockType::CLOUD;
                    }
                }
            }
        }
    }
#ifndef NDEBUG
    std::cout << "chunk initialized\n";
#endif
    computeChunkGeometry();
}

bool Chunk::isVisible(const glm::mat4 &viewproj) const {
    // coeff: 0 == in, -1 == out, +1 == out
    auto clip = [](const glm::vec4& worldPoint){
        const glm::vec4 clipPoint = worldPoint / worldPoint.w;
        return glm::ivec2{glm::sign(glm::ivec2{clipPoint.x, clipPoint.z})};
    };

    glm::ivec2 sum{};

    for (const auto &p: xzBoundaries) {
        glm::vec4 cornerPoint = viewproj * glm::vec4{p[0], 0.0f, p[1], 1.0f};
        auto position = clip(cornerPoint);
        if(glm::any(glm::equal(position, {}))){
            return true;
        }
        sum += position;
    }

    return glm::any(glm::equal(sum, {}));
}

std::array<glm::ivec3, 8> Chunk::getChunkBoundaries() const {
    std::array<glm::ivec3, 8> boundaries{};
    auto it{boundaries.begin()};

    it = std::transform(xzBoundaries.begin(), xzBoundaries.end(), it,
                        [](const glm::ivec2& v){return glm::ivec3{v[0], 0, v[1]};});
    it = std::transform(xzBoundaries.begin(), xzBoundaries.end(), it,
                        [](const glm::ivec2& v){return glm::ivec3{v[0], Y_LIMIT, v[1]};});

#ifndef NDEBUG
    assert(it == boundaries.end());
#endif
    return boundaries;
}

void Chunk::setBlock(const glm::ivec3 &position, BlockType w) {
    if (TileBlock(w).isEmpty()) {
        blockMap.erase(position);
        return;
    }
    blockMap[position] = w;
}

int getChunkDistance(const glm::ivec2 &pq1, const glm::ivec2 &pq2) {
    glm::ivec2 delta = pq1 - pq2;
    return glm::max(glm::abs(delta.x), glm::abs(delta.y));
}

int Chunk::chunked(int val) {
    return glm::floor(glm::round(val) / Chunk::SIZE);
}

glm::ivec2 Chunk::chunked(const glm::vec3 &position) {
    return {
            chunked(static_cast<int>(position.x)),
            chunked(static_cast<int>(position.z))
    };
}

void Chunk::renderObject() const {
    if (localBufferReady) {
        if (!renderReady) {
            updateBuffer();
        }
        SuperClass::renderObject();
    }
}

void Chunk::initChunk() {
    initChunkThread = std::thread(&Chunk::generateBlockmap, this);
}

glm::ivec2 Chunk::getMinXz(const glm::ivec2 &pq) {
    return pq * SIZE - 1;
}

glm::ivec2 Chunk::getMaxXz(const glm::ivec2 &pq) {
    return getMinXz(pq) + SIZE + 1;
}

int Chunk::getMinX() const {
    return xzBoundaries[0][0];
}

int Chunk::getMinZ() const {
    return xzBoundaries[0][1];
}

int Chunk::getMaxX() const {
    return xzBoundaries[3][0];
}

int Chunk::getMaxZ() const {
    return xzBoundaries[3][1];
}

bool Chunk::checkBorder(const glm::ivec3 &pos, const ::glm::ivec3 &direction) const {
    const glm::ivec3 new_pos = pos + direction;
    return
            new_pos.x <= getMinX() || new_pos.x >= getMaxX() || new_pos.z <= getMinZ() ||
            new_pos.z >= getMaxZ();
}

std::array<bool, 6> Chunk::getVisibleFaces(TileBlock w, const glm::ivec3 &pos) const {
    static constexpr std::array<glm::ivec3, 6> offsets{{
       {-1, 0, 0},
       {1, 0, 0},
       {0, 1, 0},
       {0, -1, 0},
       {0, 0, -1},
       {0, 0, 1},
}};

    return {
            TileBlock{getBlock(pos + offsets[0])}.isTransparent(),
            TileBlock{getBlock(pos + offsets[1])}.isTransparent(),
            TileBlock{getBlock(pos + offsets[2])}.isTransparent(),
            w == BlockType::CLOUD ||
            w == BlockType::LEAVES, //pos.y > 0 && TileBlock{get_block(pos + offsets[3])}.is_transparent(),
            TileBlock{getBlock(pos + offsets[4])}.isTransparent(),
            TileBlock{getBlock(pos + offsets[5])}.isTransparent(),
    };
}

bool Chunk::isOnBorder(const glm::ivec3 &pos) const {
    return checkBorder(pos, {});
}

std::unordered_map<glm::ivec3, bool> Chunk::getLightObstacles(const glm::ivec3 &blockPos) const {
    std::unordered_map<glm::ivec3, bool> lightObstacles{};

    for (int ox = -1; ox <= 1; ++ox) {
        for (int oy = -1; oy <= 1; ++oy) {
            for (int oz = -1; oz <= 1; ++oz) {
                glm::ivec3 offset{ox, oy, oz};
                lightObstacles.emplace(offset, !TileBlock{getBlock(blockPos + offset)}.isTransparent());
            }
        }
    }

    return lightObstacles;
}

bool Chunk::isLocalBufferReady() const {
    // check if thread has finished buffer modification
    return localBufferReady;
}

void Chunk::waitThread() const {
    initChunkThread.join();
}

std::array<glm::ivec2, 4> Chunk::computeXZBoundaries(const glm::ivec2 &pq) {
    auto min{getMinXz(pq)};
    auto max{getMaxXz(pq)};

    return {{
                    {min[0], min[1]},
                    {max[0], min[1]},
                    {min[0], max[1]},
                    {max[0], max[1]}
            }};
}

bool Chunk::needInitCall() const {
    return !(initChunkThread.joinable() || isLocalBufferReady());
}
}