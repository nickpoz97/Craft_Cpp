#include "Paths.hpp"
#include <Interaction/Scene.hpp>
#include <iostream>

int main() {
  CraftCpp::GameViewSettings gvs{
      {CraftCpp::WINDOW_WIDTH, CraftCpp::WINDOW_HEIGHT}, CraftCpp::INITIAL_FOV};
  CraftCpp::ShaderNamesMap snm{};
  snm.emplace(
      CraftCpp::ShaderName::BLOCK_SHADER,
      CraftCpp::ShaderFilesPaths{shadersDirPath + "block_vertex.glsl",
                                 shadersDirPath + "block_fragment.glsl"});
  snm.emplace(
      CraftCpp::ShaderName::TEXT_SHADER,
      CraftCpp::ShaderFilesPaths{shadersDirPath + "text_vertex.glsl",
                                 shadersDirPath + "text_fragment.glsl"});

  std::unique_ptr<CraftCpp::Scene> scene{
      CraftCpp::Scene::setInstance(gvs, {0, 20, 0}, {-90, -10}, snm)};
  if (scene->load_texture(texturesDirPath + "texture.png",
                          CraftCpp::TextureName::GENERAL, 0) != 0) {
    std::cerr << "general texture not loaded";
    return -1;
  }
  if (scene->load_texture(texturesDirPath + "font.png",
                          CraftCpp::TextureName::FONT, 0) != 0) {
    std::cerr << "font texture not loaded";
    return -1;
  }

  while (!glfwWindowShouldClose(CraftCpp::GameView::getWindow())) {
    scene->loop();
  }
  return 0;
}