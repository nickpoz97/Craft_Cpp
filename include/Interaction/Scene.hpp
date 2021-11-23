/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_SCENE_HPP
#define CPP_SCENE_HPP

#include "Geometry/Chunk.hpp"
#include "Interaction/Camera.hpp"
#include "Interaction/CameraControl.hpp"
#include "Interaction/GameView.hpp"
#include "Rendering/Shader.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <Geometry/Crosshair.hpp>

namespace CraftCpp {

/**
 * @brief enum representing abstract IDs for shader types
 */
enum class ShaderName { BLOCK_SHADER, TEXT_SHADER };

/**
 * @brief enum representing IDs for texture types
 */
enum class TextureName { FONT, GENERAL };

/**
 *  @brief paths of fragment and vertex code of a shader
 */
struct ShaderFilesPaths {
  const std::string vertexCode;
  const std::string fragmentCode;
};

/**
 *  @brief settings for game view
 */
struct GameViewSettings {
  glm::ivec2 windowSize; ///< width and height
  float fov;             ///< field of view
};

/// @brief map where the key is the shader' s abstract ID and value is it' s
/// code
using ShaderNamesMap = std::unordered_map<ShaderName, ShaderFilesPaths>;

/// @brief Singleton wrapper for Camera, CameraControl and GameView that handles
/// model update and rendering
/// @warning scene is a class used to help building main, so it cannot be tested
/// not interactively (run app executable in debug mode to view coverage)
class Scene {
private:
  static inline Scene *actualInstance{nullptr};
  std::unique_ptr<GameView> gameView;
  std::unique_ptr<CameraControl> cameraControl;
  Camera camera;
  std::unordered_map<ShaderName, Shader> shaders{};
  std::array<unsigned, 2> textureSamplers;
  ChunkMap chunkMap{};

  void renderText(int justify, const glm::vec2 &position, float n,
                  std::string_view text) const;

  void showInfoText() const;

  void loadChunkNeighborhood();

  void deleteDistantChunks();

  void waitThreads() const;
  Crosshair c;

  Scene(const GameViewSettings &gvs, const glm::vec3 &cameraPos,
        const glm::vec2 &cameraRotation, const ShaderNamesMap &snm);

public:
  /**
   * @brief Set the unique instance of Scene
   * @param[in] gvs settings for GameView
   * @param[in] cameraPos camera position in 3D coordinates
   * @param[in] cameraRotation camera rotation using yaw and pitch angles
   * (degrees)
   * @param[in] snm shader paths map
   * @return reference and ownership of Scene object
   * @note calling this method while there is an instance does nothing and
   * return a nullptr
   */
  static std::unique_ptr<Scene> setInstance(const GameViewSettings &gvs,
                                            const glm::vec3 &cameraPos,
                                            const glm::vec2 &cameraRotation,
                                            const ShaderNamesMap &snm);

  /**
   * @brief rendering and Chunk loading loop
   * @details call this to start Scene rendering loop that calls
   * loadAndRenderChunks, removes the distant chunks and renders the UI.
   * @note It stop when window receive a "should close" signal
   */
  void loop();

  /**
   *
   * @param path texture file path relative to executable path or absolute
   * @param textureName used as unique id for texture
   * @param clampType clamp type for outer uv coordinates
   * @return 0 if texture is loaded, negative value if there was an error
   */
  int load_texture(std::string_view path, TextureName textureName,
                   GLint clampType = GL_REPEAT);
  /**
   * @brief waits Chunk loading threads and makes possible to set new Scene
   * instance
   */
  ~Scene();

  /**
   * loads chunks near the camera (see CREATE_CHUNK_RADIUS) and renders them
   * (see RENDER_CHUNK_RADIUS)
   */
  void loadAndRenderChunks();
};
} // namespace CraftCpp

#endif // CPP_SCENE_HPP
