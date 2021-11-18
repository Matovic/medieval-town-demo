#include "scene.h"

/*!
 * Update all objects in the scene by calling them
 * @param time
 */
void Scene::update(float time) {
  this->camera_->update();

  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(this->objects_);

  while (i != std::end(this->objects_)) {
    // Update and remove from list if needed
    auto obj = i->get();

    // erase object from container if it should be deleted
    if (!obj->update(*this, time))
      i = this->objects_.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
}

/*!
 * Render all objects in the scene
 */
void Scene::render() {
  // Simply render all objects
  for ( auto& obj : this->objects_ )
    obj->render(*this);
}

/*!
 * Pick objects using a ray
 * @param position - Position in the scene to pick object from
 * @param direction - Direction to pick objects from
 * @return Objects - Vector of pointers to intersected objects
 */
std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : this->objects_) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  // return vector of pointers of intersected objects
  return intersected;
}
