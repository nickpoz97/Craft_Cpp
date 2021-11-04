//
// Created by ultimatenick on 04/11/21.
//

#ifndef CPPCRAFT_ATTRIBUTESSIZE_HPP
#define CPPCRAFT_ATTRIBUTESSIZE_HPP
namespace CraftCpp {

template<typename VertexType, std::enable_if_t<std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int getNormalAttributeSize() {
    return decltype(VertexType::normal)::length();
}

template<typename VertexType, std::enable_if_t<!std::is_base_of_v < NormalVertex, VertexType>, bool> = true>

constexpr int getNormalAttributeSize() {
    return 0;
}

template<typename VertexType, std::enable_if_t<
        std::is_base_of_v < Uv2DVertex, VertexType> || std::is_base_of_v <Uv3DVertex, VertexType>, bool> = true>

constexpr int getUvAttributeSize() {
    return decltype(VertexType::uv)::length();
}

template<typename VertexType, std::enable_if_t<
        !std::is_base_of_v < Uv2DVertex, VertexType> && !std::is_base_of_v <Uv3DVertex, VertexType>, bool> = true>

constexpr int getUvAttributeSize() {
    return 0;
}

template<typename VertexType, std::enable_if_t<std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
constexpr int getAoAttributeSize() {
    return 1;
}

template<typename VertexType, std::enable_if_t<!std::is_base_of_v < CubeVertex, VertexType>, bool> = true>

constexpr int getAoAttributeSize() {
    return 0;
}

template<typename VertexType>
constexpr int getPosAttributeSize() {
    return decltype(VertexType::position)::length();
}

}
#endif //CPPCRAFT_ATTRIBUTESSIZE_HPP
