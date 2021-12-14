#include "apple.h"
#include "explosion.h"
#include "cauldron.h"

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
    this->age_ += dt;
    this->rotation += this->rotMomentum * dt;
    if (this->age_ > 31)
    {
        this->position += this->speed * dt;

        // TODO: Collide with scene
        for (auto &obj : scene.objects_) {
            // Ignore self in scene
            if (obj.get() == this) continue;

            // We only need to collide with cauldron, ignore other objects
            auto cauldron = dynamic_cast<Cauldron*>(obj.get());
            if (!cauldron) continue;

            //std::cout << (distance(this->position, obj->position) > (obj->scale.y + this->scale.y) * 0.7f);

            // Compare distance to approximate size of the apple estimated from scale.
            if (distance(this->position, obj->position) < (obj->scale.y + this->scale.y) * 0.45f)
            {
                //int pieces = 3;

                // Generate explosion based on apple
                auto explosion = std::make_unique<Explosion>();
                explosion->position = this->position;
                explosion->scale = this->scale * 70.0f;
                explosion->speed_ = this->speed / 5.0f;
                scene.objects_.push_back(move(explosion));

                // activate animation with keyframes
                cauldron->keyframes_activated = true;

                // Destroy self
                return false;
            }
        }
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Apple::render(Scene &scene) {
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


