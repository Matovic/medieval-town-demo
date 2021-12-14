#include "light.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
/*std::unique_ptr<ppgso::Mesh> Light::mesh;
std::unique_ptr<ppgso::Shader> Light::shader;*/

Light::Light() 
{
    /*if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/sphere.obj");*/
}

bool Light::update(float dt) {
    //this->position += this->speed * dt;
    this->lightDirection_ += this->speed * dt;
    this->lightColor_ += this->colorSpeed * dt;
    //this->color += this->colorSpeed * dt;
    //scene.lightColor_ = this->lightColor_;
    //scene.lightDirection_ = this->position;
    //scene.ambientStrength_ = 0.5f;
    //scene.specularStrength_ = 0.1f;
    //generateModelMatrix();
    /*if (this->lightDirection_.z > 100)
        this->lightDirection_ = {0.0f, -5.0f, 0.0f};*/
    //std::cout << lightDirection_.x << ' ' << lightDirection_.y << ' ' << lightDirection_.z << ' ' << std::endl;
    return true;
}

/*void Light::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("OverallColor", this->color);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
    shader->setUniform("ViewPosition", scene.camera_->position);
    shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", this->modelMatrix);

    mesh->render();
}*/

/*bool Light::movementFront(Scene &scene, float dt) {
    return false;
}

bool Light::floating(Scene &scene, float dt) {
    return false;
}

bool Light::movementBack(Scene &scene, float dt) {
    return false;
}*/


//void Light::update_keyframe(Scene &scene, float dt) {}
