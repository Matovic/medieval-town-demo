#include "cauldron.h"
#include "explosion.h"
//#include "fountain.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_interpolation.hpp> // glm::interpolate
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Cauldron::mesh;
std::unique_ptr<ppgso::Texture> Cauldron::texture;
std::unique_ptr<ppgso::Shader> Cauldron::shader;

Cauldron::Cauldron() {
    // Set scale and rotation
    this->scale *= 1.f;
    this->rotation ={0.0f, 0.0f, 1.5f};

    // Initialize static resources if needed
    if (!this->shader) this->shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!this->texture) this->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/cauldron.bmp"));
    if (!this->mesh) this->mesh = std::make_unique<ppgso::Mesh>("objects/cauldron.obj");

    // keyframes
    // true, position, rotation, scale
    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(true, glm::vec3(0.f, 1.2f, 25.3f), glm::vec3{0, 0, 1.5f}, this->scale, 1));
    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(true, glm::vec3(0.f, 1.2f, 24.0f), glm::vec3{0, 0, 1.5f}, this->scale, 2));

    // create keyframes iterators
    this->currIterator = v_keyframe_.begin();
    this->nextIterator = v_keyframe_.begin();
    ++this->nextIterator;
}

bool Cauldron::update(Scene &scene, float dt) {
    this->age += dt;
    if (this->keyframes_activated)
    {
        this->keyframe_TTL += dt;
        //this->viewMatrix = lookAt(this->position, this->position - this->back + this->rotation, this->up);//this->position - this->back + this->rotation, this->up);
        auto currentKeyFrame = this->currIterator->get();
        auto nextKeyFrame = this->nextIterator->get();

        if (this->keyframe_TTL >= (currentKeyFrame->time_ + 0.002f) && this->executedKeyFrames < this->v_keyframe_.size())
        {
            ++this->executedKeyFrames;
            ++this->currIterator;
            if (this->executedKeyFrames < this->v_keyframe_.size() - 1)
                ++this->nextIterator;
        }

        else if (this->executedKeyFrames == this->v_keyframe_.size())
        {
            auto explosion = std::make_unique<Explosion>();
            explosion->position = this->position;
            explosion->scale = this->scale * 7.0f;
            //explosion->speed_ = speed / 5.0f;
            scene.objects_.push_back(std::move(explosion));
            return false;
        }

        else
        {
            float t = ((this->keyframe_TTL) / (currentKeyFrame->time_));
            //t /= 10;
            //std::cout << "t:" << t << std::endl;

            this->modelMatrix = currentKeyFrame->modelMatrix;
            this->modelMatrix = glm::interpolate
                    (
                        this->modelMatrix,//currentKeyFrame->viewMatrix_,       // current key frame
                        nextKeyFrame->modelMatrix,          // next key frame
                        t  // interpolate by t
                    );

            currentKeyFrame->modelMatrix = this->modelMatrix;

            //std::cout << glm::to_string(this->viewMatrix) << std::endl;
            //std::cout << "Changed, " << glm::to_string(currentKeyFrame->viewMatrix_) << "\nnext " << glm::to_string(nextKeyFrame->viewMatrix_) << " keyframes: "  << this->executedKeyFrames << " age: " << this->age << " t: " << t << '\n';
        }
    }

    // Generate modelMatrix from position, rotation and scale
    else
        generateModelMatrix();

    return true;
}

void Cauldron::render(Scene &scene) {
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
    this->shader->setUniform("ModelMatrix", this->modelMatrix);
    this->shader->setUniform("Texture", *texture);
    this->mesh->render();
}


