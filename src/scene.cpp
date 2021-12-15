#include "scene.h"

/*!
 * Update all objects in the scene by calling them
 * @param time
 */
void Scene::update(float time) {
  // camera returns if scene was done or not
  bool changeScene = this->camera_->update(time);

  auto light_i = std::begin(this->lights_);
  while (light_i != std::end(this->lights_))
  {
      auto obj = light_i->get();
      // erase object from container if it should be deleted
      if (!obj->update(time))
        light_i = this->lights_.erase(light_i); // NOTE: no need to call destructors as we store shared pointers in the scene
      else
        ++light_i;
  }

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

  if (changeScene)
  {
      this->firstScene_ = false;
      return;
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
