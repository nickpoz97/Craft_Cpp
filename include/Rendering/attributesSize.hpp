/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPPCRAFT_ATTRIBUTESSIZE_HPP
#define CPPCRAFT_ATTRIBUTESSIZE_HPP

#include <Geometry/Vertex.hpp>
#include <type_traits>

namespace CraftCpp {

/**
 * @defgroup VertexAttributesDimsGetters Vertex attributes' size Getters
 * @brief used to obtain the number of coefficients of each struct member in
 * Vertex.hpp
 * @tparam VertexType struct of Vertex.hpp of which you want attribute length
 * @return number of dimensions of the attribute
 * @note every attribute is a float vector or a float scalar
 * @note 0 means that attribute is absent from that struct
 * @{
 */
template <
    typename VertexType,
    std::enable_if_t<std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int getNormalAttributeSize() {
  return decltype(VertexType::normal)::length();
}

template <
    typename VertexType,
    std::enable_if_t<!std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int getNormalAttributeSize() {
  return 0;
}

template <typename VertexType,
          std::enable_if_t<std::is_base_of_v<Uv2DVertex, VertexType> ||
                               std::is_base_of_v<Uv3DVertex, VertexType>,
                           bool> = true>

constexpr int getUvAttributeSize() {
  return decltype(VertexType::uv)::length();
}

template <typename VertexType,
          std::enable_if_t<!std::is_base_of_v<Uv2DVertex, VertexType> &&
                               !std::is_base_of_v<Uv3DVertex, VertexType>,
                           bool> = true>

constexpr int getUvAttributeSize() {
  return 0;
}

template <
    typename VertexType,
    std::enable_if_t<std::is_base_of_v<BlockVertex, VertexType>, bool> = true>
constexpr int getAoAttributeSize() {
  return 1;
}

template <
    typename VertexType,
    std::enable_if_t<!std::is_base_of_v<BlockVertex, VertexType>, bool> = true>

constexpr int getAoAttributeSize() {
  return 0;
}

template <typename VertexType> constexpr int getPosAttributeSize() {
  return decltype(VertexType::position)::length();
}
///@}

} // namespace CraftCpp
#endif // CPPCRAFT_ATTRIBUTESSIZE_HPP
