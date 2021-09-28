//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_BLOCKMAP_HPP
#define CPP_BLOCKMAP_HPP

#include <vec3.hpp>
#include <unordered_map>
#include "TileBlock.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

std::size_t hash_int(int key);

template<>
struct std::hash<glm::ivec3>{
    std::size_t operator()(const glm::ivec3& v) const;
};

struct y_coord_comparator{
    bool operator()(const glm::vec3& a, const glm::vec3& b) const;
};

template<>
struct std::hash<glm::ivec2>{
    std::size_t operator()(const glm::ivec2& v) const;
};

using BaseMap = std::unordered_map<glm::ivec3, TileBlock>;

class BlockMap : private BaseMap {
private:
    glm::ivec3 delta;
public:
    explicit BlockMap(const glm::ivec2 &pq);
    TileBlock at(const glm::ivec3& key) const;
    TileBlock operator[](const glm::ivec3& key) = delete;
    void set_block(const glm::ivec3& position, BlockType block_type);
    [[nodiscard]] glm::ivec2 get_pq() const;
    [[nodiscard]] glm::ivec3 get_delta() const;

    using value_type = std::pair<glm::ivec3, TileBlock>;
    class Iterator : std::iterator<std::input_iterator_tag, value_type>{
    public:
        Iterator(const const_iterator& it, const glm::ivec3& delta);
        value_type operator*() const;
        const_iterator operator->();
        Iterator& operator++();
        friend bool operator==(const Iterator& a, const Iterator& b);
        friend bool operator!=(const Iterator& a, const Iterator& b);
    private:
        const_iterator map_it;
        const glm::ivec3& delta;
    };

    Iterator begin() const;
    Iterator end() const;

    using BaseMap::empty;
};


#endif //CPP_BLOCKMAP_HPP
