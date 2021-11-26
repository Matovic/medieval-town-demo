//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

#include "tower.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tower::mesh;
std::unique_ptr<ppgso::Texture> Tower::texture;
std::unique_ptr<ppgso::Shader> Tower::shader;

Tower::Tower() {
  // Set scale and rotation
  this->scale *= 0.09f;
  this->rotation ={0.0f, 0.0f, 1.5f};

  // Initialize static resources if needed
  if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/tower.bmp"));
  if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/tower.obj");
}

bool Tower::update(Scene &scene, float dt) {

  // Rotate the object
  if (scene.camera_->position.z > -13 && scene.camera_->position.z < -1)
    this->rotation += rotMomentum * dt;

  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}

void Tower::render(Scene &scene) {
  this->shader->use();

  // Set up light
  this->shader->setUniform("LightDirection", scene.lightDirection_);
  this->shader->setUniform("LightDirection2", scene.lightDirection2_);

  // use camera
  this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
  this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

  // render mesh
  this->shader->setUniform("ModelMatrix", modelMatrix);
  this->shader->setUniform("Texture", *texture);
  this->mesh->render();
}

