#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "asteroid.h"

bool Generator::update(Scene &scene, float dt) {
  // Accumulate time
  this->time_ += dt;

  // Add object to scene when time reaches certain level
  if (this->time_ > .3) {
    auto obj = std::make_unique<Asteroid>();
    obj->position = position;
    obj->position.x += glm::linearRand(-20.0f, 20.0f);
    scene.objects_.push_back(move(obj));
    this->time_ = 0;
  }

  return true;
}

void Generator::render(Scene &scene) {
  // Generator will not be rendered
}
