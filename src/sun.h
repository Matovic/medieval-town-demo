#pragma once

#ifndef SUN_H
#define SUN_H

#include "./ppgso/ppgso.h"
#include "./object.h"
#include "scene.h"
#include "object.h"
#include <string>

using namespace ppgso;
using namespace glm;

class Sun final : public Object {
private:
  // Static resources (Shared between instances)
  // every object needs mesh, shader to render and texture
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

public:
    Sun();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    bool animate(Scene &scene, float dt) override;

    virtual bool movementFront(Scene &scene, float dTime) override;

    virtual bool movementBack(Scene &scene, float dTime) override;

    virtual bool floating(Scene &scene, float dt) override;
    
        struct {
        float constant;
        float linear;
        float quadratic;
        
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 position;
    } pointLight;

    struct {
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;

        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    } spotLight;


    struct {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    } dirLight;
    glm::vec3 color = {1.0, 1.0, 1.0};

//    void update_keyframe(Scene &scene, float dt) override;
};


#endif //SUN_H
