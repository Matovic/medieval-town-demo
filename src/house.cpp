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

    // Set up light
    this->shader->setUniform("LightDirection", scene.lightDirection_);
    this->shader->setUniform("LightDirection2", scene.lightDirection2_);

    // use camera
    this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
    this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

    // render mesh
    this->shader->setUniform("ModelMatrix", modelMatrix);
    this->shader->setUniform("Texture", *texture);
    this->mesh->render();
}

