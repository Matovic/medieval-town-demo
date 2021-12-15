#include "tower.h"
#include "light.h"
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

    // Set up materials
    shader->setUniform("material.ambient",glm::vec3(0.02f, 0.02f, 0.01f));
    shader->setUniform("material.diffuse", glm::vec3(1.5f, 1.8f, 1.5f));
    shader->setUniform("material.specular", glm::vec3(0.1f, 0.1f, 0.1f));
    shader->setUniform("material.shininess", 0.5);


    // Set up light
    shader->setUniform("viewPos", scene.camera_->position);
    size_t index = 0;
    for (auto& obj : scene.lights_)
    {
        shader->setUniform("lights[" + std::to_string(index) + "].direction", obj->lightDirection_);
        shader->setUniform("lights[" + std::to_string(index) + "].ambient", obj->ambient);
        shader->setUniform("lights[" + std::to_string(index) + "].diffuse", obj->diffuse);
        shader->setUniform("lights[" + std::to_string(index) + "].specular", obj->specular);
        shader->setUniform("lights[" + std::to_string(index) + "].color", obj->lightColor_);
        ++index;
    }

  // use camera
  this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
  this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

  // render mesh
  this->shader->setUniform("ModelMatrix", modelMatrix);
  this->shader->setUniform("Texture", *texture);
  this->mesh->render();
}

