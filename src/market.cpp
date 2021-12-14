//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

#include "market.h"
#include "explosion.h"
#include "fountain.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Market::mesh;
std::unique_ptr<ppgso::Texture> Market::texture;
std::unique_ptr<ppgso::Shader> Market::shader;

Market::Market() {
    // Set scale and rotation
    this->scale *= 1.0f;
    this->rotation ={0.0f, 0.0f, 1.5f};
    this->final_age_ = 10.0f;

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/market.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/market-stand.obj");
}

bool Market::update(Scene &scene, float dt) {
    this->age += dt;
    if (this->final_age_ > 0 && this->age > this->final_age_)
    {
        // Generate explosion
        auto explosion = std::make_unique<Explosion>();
        explosion->position = this->position;
        explosion->scale = this->scale * 3.0f;
        //explosion->speed_ = speed / 2.0f;
        scene.objects_.push_back(move(explosion));

        // delete object
        return false;
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    // keep object alive
    return true;
}

void Market::render(Scene &scene) {
    this->shader->use();

    // Set up materials
    shader->setUniform("material.ambient",glm::vec3(0.2f, 0.2f, 0.2f));
    shader->setUniform("material.diffuse", glm::vec3(10.0f, 10.0f, 10.0f));
    shader->setUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.shininess", 0.21794872f * 128.f * 16);


    // Set up light
    shader->setUniform("viewPos", scene.camera_->position);
    size_t index = 0;
    for (auto& obj : scene.lights_)
    {
        /*glm::vec3 lighColor;
        lighColor.x = sin((glfwGetTime() + index) * 2.0f);
        lighColor.y = sin((glfwGetTime() + index) * 0.7f);
        lighColor.z = sin((glfwGetTime() + index) * 1.3f);

        shader->setUniform("fluorescentColor", lighColor);

        //obj->color = lighColor;
        obj->lightColor_ = lighColor;*/
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

