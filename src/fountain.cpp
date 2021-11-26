//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
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


