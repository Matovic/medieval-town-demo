//#include <glm/gtc/random.hpp>
//#include "explosion.h"

#include "fountain.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Fountain::mesh;
std::unique_ptr<ppgso::Texture> Fountain::texture;
std::unique_ptr<ppgso::Shader> Fountain::shader;

Fountain::Fountain() {
  // Set random scale speed and rotation
  this->scale *= 0.09f;
  //this->speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 0.0f};
  this->rotation ={0.0f, 0.0f, 1.5f};
  this->rotMomentum = {0.0f, 0.0f, glm::linearRand(-ppgso::PI/4.0f, ppgso::PI/4.0f)};

  // Initialize static resources if needed
  if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/fountainDe.bmp"));
  if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/fountain.obj");
}

bool Fountain::update(Scene &scene, float dt) {
  // Count time alive
  //this->age += dt;

  // Animate position according to time
  //this->position += glm::vec3{0.0f,0.0f,-2.0f} * dt;//speed * dt;

  // Rotate the object
  this->rotation += rotMomentum * dt;

  // Delete when alive longer than 10s or out of visibility
  //if (this->age > 10.0f || this->position.y < -10)
  //    return false;
/*
  // Collide with scene
  for (auto &obj : scene.objects_) {
    // Ignore self in scene
    if (obj.get() == this)
        continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    //auto asteroid = dynamic_cast<Asteroid*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
    //auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
    //if (!asteroid && !projectile) continue;

    // When colliding with other asteroids make sure the object is older than .5s
    // This prevents excessive collisions when asteroids explode.
    //if (asteroid && age < 0.5f) continue;

    // Compare distance to approximate size of the asteroid estimated from scale.
    if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
      int pieces = 3;

      // Too small to split into pieces
      if (scale.y < 0.5) pieces = 0;

      // The projectile will be destroyed
      //if (projectile) projectile->destroy();

      // Generate smaller asteroids
      //explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

      // Destroy self
      return false;
    }
  }
*/
  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}

void Fountain::render(Scene &scene) {
  this->shader->use();

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


