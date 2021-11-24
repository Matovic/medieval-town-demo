//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

#include "floor.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Floor::mesh;
std::unique_ptr<ppgso::Texture> Floor::texture;
std::unique_ptr<ppgso::Shader> Floor::shader;

Floor::Floor() {
    // Set random floor's scale
    this->scale = {50.0f, 0.01f, 50.0f};

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/ground.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/cube.obj");
}

bool Floor::update(Scene &scene, float dt) {
    // Delete when alive longer than 10s or out of visibility
    //if (this->age > 10.0f || this->position.y < -10)
    //    return false;

    // Collide with scene
    //for (auto &obj : scene.objects_)
        // Ignore self in scene
    //    if (obj.get() == this)
    //        continue;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    // floor is always visible, therefore we do not return false only true
    return true;
}

void Floor::render(Scene &scene) {
    this->shader->use();

    // Set up light
    this->shader->setUniform("LightDirection", scene.lightDirection_);

    // use camera
    this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
    this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

    // render mesh
    this->shader->setUniform("ModelMatrix", modelMatrix);
    this->shader->setUniform("Texture", *texture);
    this->mesh->render();
}

