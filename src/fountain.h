#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple fountain object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 * It will move down along the Y axis and self delete when reaching below -10
 */
class Fountain final : public Object {
private:
  // Static resources (Shared between instances)
  // every object needs mesh, shader to render and texture
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

  // Age of the object in seconds
  //float age{0.0f};

  // Speed and rotational momentum
  //glm::vec3 speed;
  glm::vec3 rotMomentum;

public:
  /*!
   * Create new asteroid
   */
  Fountain();

  /*!
   * Update tower
   * @param scene Scene to interact with
   * @param dt Time delta for animation purposes
   * @return
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render tower
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;
};
#endif // FOUNTAIN_H
