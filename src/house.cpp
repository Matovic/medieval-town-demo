//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

#include "house.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> House::mesh;
std::unique_ptr<ppgso::Texture> House::texture;
std::unique_ptr<ppgso::Shader> House::shader;

House::House() {
    // Set scale and rotation
    this->scale *= 0.6f;
    this->rotation ={0.0f, 0.0f, 1.57f};

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/house.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/housemedieval.obj");
}

bool House::update(Scene &scene, float dt) {

    // Rotate the object
    // if (scene.camera_->position.z > -13 && scene.camera_->position.z < -1)
    //   this->rotation += rotMomentum * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void House::render(Scene &scene) {
    this->shader->use();

    // Set up materials
    shader->setUniform("material.ambient",glm::vec3(0.02f, 0.17f, 0.02f));
    shader->setUniform("material.diffuse", glm::vec3(0.07f, 0.61f, 0.07f));
    shader->setUniform("material.specular", glm::vec3(0.63f, 0.73f, 0.63f));
    shader->setUniform("material.shininess", .6);


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

