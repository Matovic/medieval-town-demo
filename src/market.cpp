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
        explosion->scale = this->scale * 5.0f;
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

    // Set up light
    this->shader->setUniform("LightDirection", scene.lightDirection_);
    this->shader->setUniform("lightColor", scene.lightColor_);
    this->shader->setUniform("viewPos", scene.camera_->position);

    // use camera
    this->shader->setUniform("ProjectionMatrix", scene.camera_->projectionMatrix);
    this->shader->setUniform("ViewMatrix", scene.camera_->viewMatrix);

    // render mesh
    this->shader->setUniform("ModelMatrix", modelMatrix);
    this->shader->setUniform("Texture", *texture);
    this->mesh->render();
}

