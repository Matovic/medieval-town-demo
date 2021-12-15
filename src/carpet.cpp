#include "carpet.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "explosion.h"


// Static resources
std::unique_ptr<ppgso::Mesh> Carpet::mesh;
std::unique_ptr<ppgso::Texture> Carpet::texture;
std::unique_ptr<ppgso::Shader> Carpet::shader;

Carpet::Carpet()
{
    this->position = {1.f, 1.0f, 25.0f};
    this->rotation = {0.0f, 0.0f, ppgso::PI * 1.f};
    this->scale = {5.0f, 0.01f, 5.0f};

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/stars.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/cube.obj");
}

bool Carpet::update(Scene &scene, float dt) {
    this->age += dt;
    if (age > 20)
    {
        auto explosion = std::make_unique<Explosion>();
        explosion->position = this->position;
        explosion->scale = {5.0f, 5.0f, 5.0f};
        //explosion->speed_ = this->speed / 5.0f;
        scene.objects_.push_back(move(explosion));
        return false;
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    // floor is always visible, therefore we do not return false only true
    return true;
}

void Carpet::render(Scene &scene) {
    this->shader->use();

    // Set bloom
    this->shader->setUniform("bloom", true);

    // Set up materials
    shader->setUniform("material.ambient",glm::vec3(0.2f, 0.2f, 0.2f));
    shader->setUniform("material.diffuse", glm::vec3(5.0f, 5.0f, 5.0f));
    shader->setUniform("material.specular", glm::vec3(0.01f, 0.01f, 0.01f));
    shader->setUniform("material.shininess", 0.25f);


    // Set up light
    shader->setUniform("viewPos", scene.camera_->position);
    size_t index = 0;
    for (auto& obj : scene.lights_)
    {
        shader->setUniform("lights[" + std::to_string(index) + "].direction", obj->lightDirection_);
        shader->setUniform("lights[" + std::to_string(index) + "].ambient", obj->ambient);
        shader->setUniform("lights[" + std::to_string(index) + "].diffuse", obj->diffuse);
        shader->setUniform("lights[" + std::to_string(index) + "].specular", obj->specular);
        shader->setUniform("lights[" + std::to_string(index) + "].color", obj->lightColor_);
        ++index;
    }

    // use camera
    this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
    this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

    // render mesh
    this->shader->setUniform("ModelMatrix", modelMatrix);
    this->shader->setUniform("Texture", *texture);
    this->mesh->render();
}

