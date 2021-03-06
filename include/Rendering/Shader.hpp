/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_SHADER_HPP
#define CPP_SHADER_HPP

#include <array>
#include <string_view>

#include "Rendering/OpenGlExternal.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace CraftCpp {

/// @brief handler for glsl shader
struct Shader {
private:
  enum class shader_type { VERTEX, FRAGMENT };

  GLuint id{};

  static unsigned build_shader(std::string_view path, shader_type st);

  static void _setExtraUniform(int u_location, int val);

  static void _setExtraUniform(int u_location, float val);

  static void _setExtraUniform(int u_location, const glm::vec3 &val);

  static void _setExtraUniform(int u_location, const glm::vec2 &val);

  static void _setExtraUniform(int u_location, const glm::mat4 &val);

  static void check_compile_errors(GLuint shader, const std::string &type);

  void set_pi() const;

public:
  /**
   * @brief builds a Shader object using vertex and fragment code files
   * @param[in] vs_path path of vertex shader code
   * @param[in] fs_path path of fragment shader code
   */
  Shader(std::string_view vs_path, std::string_view fs_path);

  /// @brief set this as active shader
  void use() const;

  /// @return OpenGL' s shader id
  [[nodiscard]] GLuint getId() const;

  /**
   * @brief sets camera position uniform for this shader
   * @param[in] camera_pos camera 3D coordinates
   */
  void setCamera(const glm::vec3 &camera_pos) const;
  /**
   * @brief sets sampler uniform using indexed texture
   * @param[in] sampler texture index
   */
  void setSampler(int sampler) const;
  /**
   * @brief set timer uniform
   * @param[in] timer timer value
   */
  void setTimer(float timer) const;
  /**
   * @brief set viewproj matrix uniform
   * @param[in] m viewproj matrix
   * @note matrix should be obtained with GameView::getProjMatrix
   */
  void setViewProj(const glm::mat4 &m) const;

  /**
   *
   * @tparam GLtype type used in shader
   * @param[in] uniform_name uniform id
   * @param[in] value uniform value
   */
  template <typename GLtype>
  void setExtraUniform(std::string_view uniform_name,
                       const GLtype &value) const;
};
} // namespace CraftCpp

#endif // CPP_SHADER_HPP
