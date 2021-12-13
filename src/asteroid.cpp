#include <glm/gtc/random.hpp>
#include "asteroid.h"
//#include "projectile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Asteroid::mesh;
std::unique_ptr<ppgso::Texture> Asteroid::texture;
std::unique_ptr<ppgso::Shader> Asteroid::shader;

Asteroid::Asteroid() {
  // Set random scale speed and rotation
  scale *= glm::linearRand(1.0f, 3.0f);
  speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 0.0f};
  rotation = glm::ballRand(ppgso::PI);
  rotMomentum = glm::ballRand(ppgso::PI);

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/tower.bmp"));//("textures/tower.bmp"));//("asteroid.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("asteroid.obj");//("objects/tower.obj");
}

bool Asteroid::update(Scene &scene, float dt) {
  // Count time alive
  age += dt;

  // Animate position according to time
  position += speed * dt;

  // Rotate the object
  rotation += rotMomentum * dt;

  // Delete when alive longer than 10s or out of visibility
  if (age > 10.0f || position.y < -10) return false;

  // Collide with scene
  for (auto &obj : scene.objects_) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto asteroid = dynamic_cast<Asteroid*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
    //auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
    if (!asteroid) //&& !projectile)
            continue;

    // When colliding with other asteroids make sure the object is older than .5s
    // This prevents excessive collisions when asteroids explode.
    if (asteroid && age < 0.5f) continue;

    // Compare distance to approximate size of the asteroid estimated from scale.
    if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
      int pieces = 3;

      // Too small to split into pieces
      if (scale.y < 0.5) pieces = 0;

      // The projectile will be destroyed
      //if (projectile) projectile->destroy();

      // Generate smaller asteroids
      explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

      // Destroy self
      return false;
    }
  }

  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}

void Asteroid::explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces) {
  // Generate explosion
  auto explosion = std::make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed_ = speed / 2.0f;
  scene.objects_.push_back(move(explosion));

  // Generate smaller asteroids
  for (int i = 0; i < pieces; i++) {
    auto asteroid = std::make_unique<Asteroid>();
    asteroid->speed = speed + glm::vec3(glm::linearRand(-3.0f, 3.0f), glm::linearRand(0.0f, -5.0f), 0.0f);;
    asteroid->position = position;
    asteroid->rotMomentum = rotMomentum;
    float factor = (float) pieces / 2.0f;
    asteroid->scale = scale / factor;
    scene.objects_.push_back(move(asteroid));
  }
}

void Asteroid::render(Scene &scene) {
  shader->use();

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
      glm::vec3 lighColor;
      lighColor.x = sin((glfwGetTime() + index) * 2.0f);
      lighColor.y = sin((glfwGetTime() + index) * 0.7f);
      lighColor.z = sin((glfwGetTime() + index) * 1.3f);

      shader->setUniform("fluorescentColor", lighColor);

      //obj->color = lighColor;
      obj->lightColor_ = lighColor;
      shader->setUniform("light[" + std::to_string(index) + "].direction", obj->lightDirection_);
      shader->setUniform("light[" + std::to_string(index) + "].ambient", obj->ambient);
      shader->setUniform("light[" + std::to_string(index) + "].diffuse", obj->diffuse);
      shader->setUniform("light[" + std::to_string(index) + "].specular", obj->specular);
      shader->setUniform("light[" + std::to_string(index) + "].color", obj->specular);
      ++index;
  }

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Asteroid::onClick(Scene &scene) {
  std::cout << "Asteroid clicked!" << std::endl;
  explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
  age = 10000;
}

