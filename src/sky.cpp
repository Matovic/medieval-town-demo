#include "sky.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>


Sky::Sky(const bool firstScene) {

  // Reset texture if we change the scenes
  if (texture && !firstScene) texture.reset();

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
  if (!texture && firstScene) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/sky.bmp"));
  if (!texture && !firstScene) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/sky_night.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/quad.obj");
}

bool Sky::update(Scene &scene, float dt) {
  // Offset for UV mapping, creates illusion of scrolling
  //textureOffset.y -= dt/5;

  generateModelMatrix();

  // background is always visible, therefore we do not return false only true
  return true;
}

void Sky::render(Scene &scene) {
  // Disable writing to the depth buffer so we render a "background"
  // Do not need depth mask, because it is background, where we do not need depth,
  // background is always the most far away object in the scene
  glDepthMask(GL_FALSE);

  // NOTE: this object does not use camera, just renders the entire quad as is
  shader->use();

  // Pass UV mapping offset to the shader
  //shader->setUniform("TextureOffset", textureOffset);

  // Render mesh, not using any projections, we just render in 2D
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("ViewMatrix", glm::mat4{1.0f});
  shader->setUniform("ProjectionMatrix", glm::mat4{1.0f});
  shader->setUniform("Texture", *texture);
  mesh->render();

  glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Sky::mesh;
std::unique_ptr<ppgso::Shader> Sky::shader;
std::unique_ptr<ppgso::Texture> Sky::texture;
