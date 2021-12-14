#ifndef Light_H
#define Light_H

#include <ppgso/ppgso.h>
//#include "scene.h"
//#include "object.h"
#include <string>

class Light final {
private:
  // Static resources (Shared between instances)
  // every object needs mesh, shader to render and texture
  //static std::unique_ptr<ppgso::Mesh> mesh;
  //static std::unique_ptr<ppgso::Shader> shader;

public:
    Light();

    bool update(float dt);

    glm::vec3 speed = {0.0f, 0.0f, 0.0f};
    glm::vec3 colorSpeed = {0.0f, 0.0f, 0.0f};
    // lightColor
    glm::vec3 lightColor_{1.0f, 1.0f, 1.0f};

    glm::vec3 lightDirection_{-5.0f, 5.0f, -5.0f};  // position for lightning
    glm::vec3 specular;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    //glm::vec3 color = {1.0f, 1.0f, 0.0f};
    float age = 0.0f;
    float final_age = -1.0f;
};


#endif //Light_H
