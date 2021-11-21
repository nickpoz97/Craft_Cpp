//
// Created by ultimatenick on 15/09/21.
//

#ifndef CPP_GAMEVIEW_HPP
#define CPP_GAMEVIEW_HPP

#define GLFW_INCLUDE_NONE

#include <Geometry/Chunk.hpp>
#include <memory>

#include "Rendering/OpenGlExternal.hpp"
#include "glm/mat4x4.hpp"
#include "tunableParameters.hpp"

namespace CraftCpp {

/// @brief Singleton class that handles the game Window
class GameView {
private:
  static inline GameView *actualInstance{nullptr};
  GLFWwindow *window;

  int width;
  int height;
  int scale;
  int ortho;
  float fov;

  static int computeScaleFactor(int width, int height);

  GLFWwindow *createWindow(bool is_fullscreen);

  GameView(int width, int height, float fov, int ortho = false,
           bool isFullscreen = false);

public:
  /// @brief near clipping plane distance
  static constexpr float z_near = 0.125f;
  /// @brief far clipping plane distance
  static constexpr float z_far =
      static_cast<float>(RENDER_CHUNK_RADIUS) * Chunk::SIZE;

  /// @brief IDs for projection types
  /// @note UI is a special type of orthogonal projection
  enum ProjType { PERSP, UI, ORTHO_3D };

  /**
   * @brief set the unique instance of GameView
   * @param[in] width width of game window in pixels
   * @param[in] height height of game window in pixels
   * @param[in] fov field of view used in view space in degrees
   * @return reference and ownership of GameView object
   * @note calling this method while there is an instance does nothing and
   * return a nullptr
   */
  static std::unique_ptr<GameView> setInstance(int width, int height,
                                               float fov);

  /**
   * @defgroup GameViewGetters GameView getters
   * @return attribute value
   * @{
   */
  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;
  [[nodiscard]] int getScale() const;
  [[nodiscard]] int getFov() const;
  [[nodiscard]] int getOrtho() const;
  [[nodiscard]] static GLFWwindow *getWindow();
  /// @}

  /// @return width / height
  [[nodiscard]] float getRatio() const;
  /// @return projection matrix depending on clipping planes distance,
  /// projection type, fov and ratio
  [[nodiscard]] glm::mat4 getProjMatrix(ProjType pt) const;
  /// @return true iff GameView singleton has already been instantiated
  [[nodiscard]] static bool isInstantiated();
  /// @return reference to active instance
  [[nodiscard]] static GameView *getActualInstance();
  /**
   * @defgroup GameView attributes setters
   * @{
   */
  void setOrtho(int ortho_size);
  void setFov(int fov_degrees);
  /// @}

  /// @brief update width and height with actual GLFWwindow parameters
  void update();

  /// @brief makes possible new GameView instance and frees GLFW resources
  ~GameView();
};
} // namespace CraftCpp

#endif // CPP_GAMEVIEW_HPP
