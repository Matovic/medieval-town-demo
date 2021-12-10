//#include <glm/gtc/random.hpp>
//#include "asteroid.h"
//#include "projectile.h"
//#include "explosion.h"

#include "apple.h"
#include "explosion.h"
#include "fountain.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Apple::mesh;
std::unique_ptr<ppgso::Texture> Apple::texture;
std::unique_ptr<ppgso::Shader> Apple::shader;

Apple::Apple() {
    // Set scale and rotation
    this->scale *= 0.002f;
    this->rotation ={0.0f, 0.0f, 1.5f};

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/apple.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/Apple.obj");
}

bool Apple::update(Scene &scene, float dt) {

    // Rotate the object
    // if (scene.camera_->position.z > -13 && scene.camera_->position.z < -1)
    //   this->rotation += rotMomentum * dt;

    // TODO: Collide with scene
    for (auto &obj : scene.objects_) {
        // Ignore self in scene
        if (obj.get() == this) continue;

        // We only need to collide with asteroids and projectiles, ignore other objects
        //auto asteroid = dynamic_cast<Asteroid*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
        //auto projectile = dynamic_cast<Projectile*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
        //if (!asteroid && !projectile) continue;

        auto fountain = dynamic_cast<Fountain*>(obj.get());
        if (!fountain) continue;

        //std::cout << (distance(this->position, obj->position) > (obj->scale.y + this->scale.y) * 0.7f);

        // Compare distance to approximate size of the asteroid estimated from scale.
        if (distance(this->position, obj->position) < (obj->scale.y + this->scale.y) * 0.7f) {
            //int pieces = 3;

            // Generate explosion
            auto explosion = std::make_unique<Explosion>();
            explosion->position = (obj->position + this->position);
            explosion->scale = (obj->scale + this->scale) * 10.0f;
            //explosion->speed_ = speed / 2.0f;
            scene.objects_.push_back(move(explosion));

            // Destroy self
            return false;
        }
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Apple::render(Scene &scene) {
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


