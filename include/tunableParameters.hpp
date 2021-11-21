// @file
// Boolean and Integers constants that changes performance and details of the 3d
// world

#ifndef CPP_COSTANTS_HPP
#define CPP_COSTANTS_HPP

namespace CraftCpp {

/**
 * @brief number of block per Chunk side
 * @note see Chunk::SIZE
 * @warning value should be positive
 */
static constexpr int CHUNK_SIZE = 64;

/**
 * @defgroup ChunkHyperParameters Chunk HyperParameters
 * @note a value of 1 represents 9 chunks (1 front, 1 back, 1 left, 1 right, 1
 * where tha Camera is located and 4 on the angles)
 * @note value of 0 represents 1 Chunk only (the Chunk where the Camera is
 * located)
 * @note in practise each of these values indicates a square of Chunks centered
 * on Camera::getPq() where each side is 2*n+1
 * @warning each value should be positive
 * @{
 */

/// @brief number of chunks to be generated from Camera position (see
/// Scene::loadAndRenderChunks) at each Scene loop
/// @warning in debugging mode use lowest possible value (0) for performance
#ifdef NDEBUG
static constexpr int CREATE_CHUNK_RADIUS = 3;
#else
static constexpr int CREATE_CHUNK_RADIUS = 0;
#endif

/// @brief number of chunks to be rendered from Camera position (see
/// Chunk::isVisible) at each scene loop
/// @note for a lower value you have a stronger fog effect
static constexpr int RENDER_CHUNK_RADIUS =
    (CREATE_CHUNK_RADIUS > 0) ? CREATE_CHUNK_RADIUS - 1 : 1;

/// @brief number of chunks to be deleted from Camera position (see
/// Scene::loadAndRenderChunks) at each scene loop
/// @note with an higher value there are more chunks in memory but deletion
/// efficiency increases
static constexpr int DELETE_CHUNK_RADIUS = CREATE_CHUNK_RADIUS + 3;
/** @} */

/**
 * @defgroup TextAlign 2D text align constants
 * @{
 */
static constexpr int ALIGN_LEFT = 0;
static constexpr int ALIGN_CENTER = 1;
static constexpr int ALIGN_RIGHT = 2;
/// @}

/**
 * @defgroup RenderingFlags Rendering Flags
 * @{
 */
static constexpr bool SHOW_PLANTS = true;
static constexpr bool SHOW_TREES = true;
static constexpr bool SHOW_CLOUDS = true;
static constexpr bool SHOW_CROSSHAIRS = true;
static constexpr bool SHOW_INFO_TEXT = true;

/// @}

/**
 * @defgroup WindowSettings Window Settings
 * @{
 */
static constexpr int WINDOW_WIDTH = 1024; ///< Window width in pixels
static constexpr int WINDOW_HEIGHT = 768; ///< Window height in pixels
static constexpr bool VSYNC = false;      ///< Vsync flag
static constexpr float INITIAL_FOV = 45;  ///< Initial fog value
/// @}
} // namespace CraftCpp

#endif // CPP_COSTANTS_HPP
