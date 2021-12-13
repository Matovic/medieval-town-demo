#include <glm/gtc/random.hpp>
#include "scene.h"
#include "explosion.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// static resources
std::unique_ptr<ppgso::Mesh> Explosion::mesh;
std::unique_ptr<ppgso::Texture> Explosion::texture;
std::unique_ptr<ppgso::Shader> Explosion::shader;

Explosion::Explosion() {
  // Random rotation and momentum
  this->rotation = glm::ballRand(ppgso::PI)*3.0f;
  this->rotMomentum_ = glm::ballRand(ppgso::PI)*3.0f;
  this->speed_ = {0.0f, 0.0f, 0.0f};

  // Initialize static resources if needed
  if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
  if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/explosion.bmp"));
  if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/sphere.obj");
}

void Explosion::render(Scene &scene) {
  this->shader->use();

  // Transparency, interpolate from 1.0f -> 0.0f
  this->shader->setUniform("Transparency", 1.0f - this->age_ / this->maxAge_);

  // use camera
  this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
  this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

  // render mesh
  this->shader->setUniform("ModelMatrix", this->modelMatrix);
  this->shader->setUniform("Texture", *texture);

  // Disable depth testing
  glDisable(GL_DEPTH_TEST);

  // Enable blending
  glEnable(GL_BLEND);
  // Additive blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  this->mesh->render();

  // Disable blending
  glDisable(GL_BLEND);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
}

bool Explosion::update(Scene &scene, float dt) {
  // Update scale and rotation
  this->scale = this->scale * ( 1.0f + dt * 5.0f);
  this->rotation += this->rotMomentum_ * dt;
  this->position += this->speed_ * dt;

  // Die after reaching maxAge
  this->age_ += dt;
  if (this->age_ > this->maxAge_)
      return false;

  generateModelMatrix();
  return true;
}
