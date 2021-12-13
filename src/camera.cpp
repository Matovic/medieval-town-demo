#include <glm/glm.hpp>
#include <glm/gtx/matrix_interpolation.hpp> // glm::interpolate
#include <iterator>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>

#include "camera.h"

/*!
 * Create new Camera that will generate viewMatrix and projectionMatrix based on its
 * position, up and back vectors
 * @param fow - Field of view in degrees
 * @param ratio - Viewport screen ratio (usually width/height of the render window)
 * @param near - Distance to the near frustum plane
 * @param far - Distance to the far frustum plane
 */
Camera::Camera(float fow, float ratio, float near, float far, const bool firstScene)
{
    // set keyframes for first scene
    if (firstScene)
    {
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, -13), glm::vec3{0, 2, -12}, this->up, 0));
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 0), glm::vec3{0, 2, 1}, this->up, 2));
        //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 35, 0), glm::vec3{0, 30, 1}, this->up, 5));
        //    market center camera
        //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2.5, 23), glm::vec3{0, 1.5, 25}, this->up, 15));
      //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 1.5, -13), glm::vec3{0, 2, -12}, this->up, 2));
    //  this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 3), glm::vec3{0, 2, 6}, this->up, 1));
    //    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 4), glm::vec3{0, 2, 6}, this->up, 2));
    //    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 5), glm::vec3{0, 2, 6}, this->up, 3));
    //    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 6), glm::vec3{0, 2, 7}, this->up, 4));
    //

    //    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(-3, 2, 23), glm::vec3{-2, 2, 25}, this->up, 3));
    //    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(3, 2, 23), glm::vec3{2, 2, 25}, this->up, 4));
    //    this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 4, 22), glm::vec3{0, -2, 25}, this->up, 5));

    //  this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 2), glm::vec3{0, 2, 6}, this->up, 2));
    //  this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 2), glm::vec3{0, 2, 6}, this->up, 3));

    //  this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(4, 4, 14), glm::vec3{-5, 2, 15}, this->up, 5));
    //  this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 5), glm::vec3{0, 2, 6}, this->up, 7));
    //  this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(4, 4, 15), glm::vec3{4, 4, 16}, this->up, 10));
    }

    // set keyframes for Night scene
    else
    {
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3{0, 2, -13}, glm::vec3{0, 2, -12}, this->up, 0));
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3{0, 2, 24}, glm::vec3{0, 2, 25}, this->up, 5));
    }

  this->currIterator = v_keyframe_.begin();
  this->nextIterator = v_keyframe_.begin();
  //std::advance(this->nextIterator, 1);

  ++this->nextIterator;
  //std::cout << this->v_keyframe_[0]->viewMatrix_[0].y << '\n';

  float fowInRad = (ppgso::PI/180.0f) * fow;

  // glm::perspective creates a 4x4 perspective projection matrix that is used in a shader
  this->projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

/*!
 * Update Camera viewMatrix based on up, position and back vectors
 */
void Camera::update(float dt) {
    //if (position.z < -10)
    //this->position += this->speed * dt;
    //if (this->keyframe_->index < this->keyframe_->points.size())
    //    this->position += this->keyframe_->points[this->keyframe_->index++] * dt;

    //dt /= 100;
    this->age += dt;
    //this->viewMatrix = lookAt(this->position, this->position - this->back + this->rotation, this->up);//this->position - this->back + this->rotation, this->up);
    auto currentKeyFrame = this->currIterator->get();
    auto nextKeyFrame = this->nextIterator->get();

    if (this->age >= (nextKeyFrame->time_ + 0.002f) && this->executedKeyFrames < this->v_keyframe_.size())
    {
        ++this->executedKeyFrames;
        ++this->currIterator;
        if (this->executedKeyFrames < this->v_keyframe_.size() - 1)
            ++this->nextIterator;
    }

    else if (this->executedKeyFrames == this->v_keyframe_.size())
        return;

    else
    {
        float t = ((this->age - currentKeyFrame->time_) / (nextKeyFrame->time_ - currentKeyFrame->time_));
        t /= 10;

        this->viewMatrix = currentKeyFrame->viewMatrix_;
        this->viewMatrix = glm::interpolate
                (
                    this->viewMatrix,//currentKeyFrame->viewMatrix_,       // current key frame
                    nextKeyFrame->viewMatrix_,          // next key frame
                    t  // interpolate by t
                );

        currentKeyFrame->viewMatrix_ = this->viewMatrix;

        //std::cout << glm::to_string(this->viewMatrix) << std::endl;
        //std::cout << "Changed, " << glm::to_string(currentKeyFrame->viewMatrix_) << "\nnext " << glm::to_string(nextKeyFrame->viewMatrix_) << " keyframes: "  << this->executedKeyFrames << " age: " << this->age << " t: " << t << '\n';
    }
    /*
    if (this->key_pressed)
    {
        this->key_pressed = false;
        this->viewMatrix = currentKeyFrame->viewMatrix_;
        std::cout << "Changed, " << currentKeyFrame->time_ << " next " << nextKeyFrame->time_ << " keyframes: "  << this->executedKeyFrames << " age: " << this->age << " time: " << ((this->age - currentKeyFrame->time_) / (nextKeyFrame->time_ - currentKeyFrame->time_)) << '\n';
        ++this->executedKeyFrames;
        ++this->currIterator;
        //if (this->executedKeyFrames < this->v_keyframe_.size() - 1)
        //   ++this->nextIterator;
    }*/
    /*std::cout << "Prva\n";
    std::cout << glm::to_string(this->viewMatrix) << '\n';
    std::cout << "druha\n";
    std::cout << glm::to_string(currentKeyFrame->viewMatrix_) << '\n';*/
}

/*!
 * Get direction vector in world coordinates through camera projection plane
 * @param u - camera projection plane horizontal coordinate [-1,1]
 * @param v - camera projection plane vertical coordinate [-1,1]
 * @return Normalized vector from camera position to position on the camera projection plane
 */
glm::vec3 Camera::cast(double u, double v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
  return glm::vec3{direction};
}
