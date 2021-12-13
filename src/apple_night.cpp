#include "apple_night.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>



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

    // Rotate the object
    // if (scene.camera_->position.z > -13 && scene.camera_->position.z < -1)
    //   this->rotation += rotMomentum * dt;

    // TODO: Collide with scene
    /*for (auto &obj : scene.objects_) {
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
    }*/

    // update parent
    //glm::vec3 speed{1.f, 1.f, -2.f};
    //this->position *= speed;
    //this->cauldron->position *= speed;
    //this->cauldron->carpet->position *= speed;
    this->cauldron->update(scene, dt);

    // Generate modelMatrix from position, rotation and scale
    //generateModelMatrix();
    //this->modelMatrix *= this->cauldron->modelMatrix;
    this->modelMatrix =
            glm::translate(glm::mat4(1.0f), this->position * this->cauldron->position * this->cauldron->carpet->position)
            * glm::orientate4(this->rotation)
            * glm::scale(glm::mat4(1.0f), this->scale);

    return true;
}

void AppleNight::render(Scene &scene) {
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

