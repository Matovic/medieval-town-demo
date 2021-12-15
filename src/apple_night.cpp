#include "apple_night.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include "explosion.h"


// Static resources
std::unique_ptr<ppgso::Mesh> AppleNight::mesh;
std::unique_ptr<ppgso::Texture> AppleNight::texture;
std::unique_ptr<ppgso::Shader> AppleNight::shader;

AppleNight::AppleNight()
    : cauldron{std::make_unique<CauldronNight>()}
{
    // Set scale and rotation
    this->position = {3.20f, 1.31f, 1.0f};
    this->scale *= 0.002f;
    this->rotation ={0.0f, 0.0f, ppgso::PI * 1.f};

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/apple.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/Apple.obj");
}

bool AppleNight::update(Scene &scene, float dt) {
    // update parent
    this->cauldron->update(scene, dt);
    this->age += dt;
    if (age > 20)
    {
        return false;
    }

    // Generate modelMatrix from position, rotation and scale
    this->modelMatrix =
            glm::translate(glm::mat4(1.0f), this->position * this->cauldron->position * this->cauldron->carpet->position)
            * glm::orientate4(this->rotation)
            * glm::scale(glm::mat4(1.0f), this->scale);

    return true;
}

void AppleNight::render(Scene &scene) {
    this->shader->use();

    // Set bloom
    this->shader->setUniform("bloom", true);

    // Set up materials
    shader->setUniform("material.ambient",glm::vec3(0.2f, 0.2f, 0.2f));
    shader->setUniform("material.diffuse", glm::vec3(10.0f, 10.0f, 10.0f));
    shader->setUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.shininess", 0.2);

    // Set up light
    shader->setUniform("viewPos", scene.camera_->position);
    size_t index = 0;
    for (auto& obj : scene.lights_)
    {
        shader->setUniform("light[" + std::to_string(index) + "].direction", obj->lightDirection_);
        shader->setUniform("light[" + std::to_string(index) + "].ambient", obj->ambient);
        shader->setUniform("light[" + std::to_string(index) + "].diffuse", obj->diffuse);
        shader->setUniform("light[" + std::to_string(index) + "].specular", obj->specular);
        shader->setUniform("light[" + std::to_string(index) + "].color", obj->specular);
        ++index;
    }


    // use camera
    this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
    this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

    // render parent
    this->cauldron->render(scene);

    // render mesh
    this->shader->setUniform("ModelMatrix", this->modelMatrix);
    this->shader->setUniform("Texture", *texture);
    this->mesh->render();
}

