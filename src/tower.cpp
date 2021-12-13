//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

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

    // Set up light
    /*this->shader->setUniform("LightDirection", scene.lightDirection_);
    this->shader->setUniform("lightColor", scene.lightColor_);
    this->shader->setUniform("viewPos", scene.camera_->position);
    this->shader->setUniform("ambientStrength", scene.ambientStrength_);
    this->shader->setUniform("specularStrength", scene.specularStrength_);*/

    // Set up materials
    shader->setUniform("material.ambient",glm::vec3(0.2f, 0.2f, 0.2f));
    shader->setUniform("material.diffuse", glm::vec3(10.0f, 10.0f, 10.0f));
    shader->setUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.shininess", 0.21794872f * 128.f * 16);


    // Set up light
    shader->setUniform("viewPos", scene.camera_->position);
    size_t index = 0;
    for (auto& obj : scene.lights_)
    {
        /*glm::vec3 lighColor;
        lighColor.x = sin((glfwGetTime() + index) * 2.0f);
        lighColor.y = sin((glfwGetTime() + index) * 0.7f);
        lighColor.z = sin((glfwGetTime() + index) * 1.3f);

        shader->setUniform("fluorescentColor", lighColor);

        //obj->color = lighColor;
        obj->lightColor_ = lighColor;*/
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

