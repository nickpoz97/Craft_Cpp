//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <vec2.hpp>

#include "BlockMap.hpp"
#include "list"
#include "Sign.hpp"
#include "Attrib.hpp"
#include "OpenglBuffer.hpp"
#include "Vertex.hpp"
#include "costants.hpp"
#include "Model.hpp"

class Chunk {
private:
    //OpenglBuffer<Uv3DVertex> sign_buffer;
    OpenglBuffer<CubeVertex> gpu_buffer;
    std::vector<CubeVertex> local_buffer;

    const Model& model;

    const glm::ivec2 pq;
    static int min_y;
    static int max_y;

    BlockMap map{};
    //BlockMap lights{};
    //std::list<Sign> sign_list{};

    int faces; // n of vertices
    //int sign_faces;

    bool dirty;

    const OpenglBuffer<CubeVertex> buffer;
    //const Attrib attrib;

    static constexpr int XZ_SIZE = Chunk::size * 3 + 2;
    static constexpr int XZ_LO = Chunk::size;
    static constexpr int XZ_HI = Chunk::size * 2 + 1;
    static constexpr int Y_SIZE = 258;
    static constexpr auto XYZ = [](int x, int y, int z){ return y * XZ_SIZE * XZ_SIZE + x * XZ_SIZE + z; };
    static constexpr auto XZ = [](int x, int z){ return XYZ(x,0,z); };

    //static void void light_fill(std::vector<char>& opaque, std::vector<char>& light, const glm::vec3& v, int w, bool force);
public:
    static constexpr int size = CHUNK_SIZE;

    Chunk(const WorkerItem& wi)
    void draw();
    static constexpr int getSize();
    static int getMinY();
    static int getMaxY();
    Tile getHighestBlock() const;
    const glm::ivec2 &getPq() const;
    Tile get_block(const glm::ivec3& block_pos) const;
    bool operator!() const;
    void gen_sign_buffer();
    bool has_lights();
    void set_dirt();

    void populate_opaque(const WorkerItem &wi, const glm::ivec3 &o, const std::vector<bool>& opaque, const std::vector<char>& highest) const;
    //void flood_fill(const WorkerItem& wi, const glm::ivec3& o, std::vector<char>& opaque, std::vector<char>& light);

    void count_exposed_faces(const BlockMap& map, std::vector<bool> opaque, const glm::ivec3& o);
    void generate_geometry(const std::vector<char>& opaque);

    static void Chunk::occlusion(const std::array<char, 27>& neighbors,
                                 const std::array<char, 27>& lights,
                                 const std::array<char, 27>& shades,
                                 std::array<std::array<float,4>, 6>& ao,
                                 std::array<std::array<float,4>, 6>& light));

    template<typename FT>
    void map_for_each(const BlockMap& map, FT function);
};


#endif //CPP_CHUNK_HPP
