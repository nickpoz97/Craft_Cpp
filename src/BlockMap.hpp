//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_BLOCKMAP_HPP
#define CPP_BLOCKMAP_HPP

#include <vec3.hpp>
#include <map>
#include "map_utils.hpp"
#include "item.hpp"

class BlockMap{
private:
    using BaseMap = std::map<glm::ivec3, Item, y_coord_comparator>;
    BaseMap map;
    glm::ivec3 delta;

public:
    explicit BlockMap(const glm::ivec3& delta);
    Item get_item(const glm::ivec3& key) const;
    void set_item(const glm::ivec3& key, Item item);
    const glm::ivec3& get_delta() const;

    class Iterator{
    public:
        using iterator_category = std::output_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<glm::ivec3, Item>;
        using pointer = const value_type*;
        using reference = const value_type&;

        Iterator(const BaseMap::const_iterator& it, const glm::ivec3& delta);
        value_type operator*();
        BaseMap::const_iterator operator->();
        Iterator operator++();
        friend bool operator==(const Iterator& a, const Iterator& b);
        friend bool operator!=(const Iterator& a, const Iterator& b);

    private:
        BaseMap::const_iterator map_it;
        const glm::ivec3& delta;
    };

    Iterator begin();
    Iterator end();
};


#endif //CPP_BLOCKMAP_HPP
