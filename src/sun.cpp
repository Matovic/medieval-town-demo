#include "sun.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Sun::mesh;
std::unique_ptr<ppgso::Shader> Sun::shader;

Sun::Sun()
{
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/sphere.obj");
}

bool Sun::update(Scene &scene, float dt) {
    this->position += this->speed * dt;
    this->color += this->colorSpeed * dt;
    //scene.lightDirection_ = this->position;
    //scene.ambientStrength_ = 0.5f;
    //scene.specularStrength_ = 0.1f;
    generateModelMatrix();
    return true;
}

void Sun::render(Scene &scene) {
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
}
