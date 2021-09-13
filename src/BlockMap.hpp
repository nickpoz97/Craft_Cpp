//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_BLOCKMAP_HPP
#define CPP_BLOCKMAP_HPP

#include <vec3.hpp>
#include <map>
#include "map_utils.hpp"
#include "TileBlock.hpp"

using BaseMap = std::unordered_map<glm::ivec3, TileBlock>;

class BlockMap : public BaseMap {
private:
    glm::ivec3 delta;
public:
    explicit BlockMap(const glm::ivec2 &pq);
    TileBlock at(const glm::ivec3& key) const;
    TileBlock operator[](const glm::ivec3& key) = delete;
    void set_block(const glm::ivec3& position, BlockType block_type);
    [[nodiscard]] const glm::ivec3& get_delta() const;

    using value_type = std::pair<glm::ivec3, TileBlock>;
    class Iterator : std::iterator<std::input_iterator_tag, value_type>{
    public:
        /*using iterator_category = std::output_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;*/

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
};


#endif //CPP_BLOCKMAP_HPP
