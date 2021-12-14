#ifndef SUN_H
#define SUN_H

#include <ppgso/ppgso.h>
#include "scene.h"
#include "object.h"
#include <string>

class Sun final : public Object {
private:
  // Static resources (Shared between instances)
  // every object needs mesh, shader to render and texture
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;

public:
    Sun();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    glm::vec3 speed = {0.0f, 0.0f, 0.0f};
    glm::vec3 colorSpeed = {0.0f, 0.0f, 0.0f};
    glm::vec3 color = {1.0f, 1.0f, 0.0f};
    float age = 0.0f;
    float final_age = -1.0f;
};


#endif //SUN_H
