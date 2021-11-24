//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

#include "market.h"

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

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/market.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/market-stand.obj");
}

bool Market::update(Scene &scene, float dt) {

    // Rotate the object
   // if (scene.camera_->position.z > -13 && scene.camera_->position.z < -1)
     //   this->rotation += rotMomentum * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Market::render(Scene &scene) {
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

