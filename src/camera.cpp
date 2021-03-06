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
        // init
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, -15), glm::vec3{0, 2, -13}, this->up, 0));
        //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 0), glm::vec3{0, 2, 1}, this->up, 10));

        // fountain
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 2), glm::vec3{0, 2, 3}, this->up, 10));
        // right
        //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(-2, 2, 5), glm::vec3{-3, 2, 6}, this->up, 3));
        // left
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(4, 2, 5), glm::vec3{5, 2, 6}, this->up, 12));

        // forward
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(2, 2, 15), glm::vec3{3, 2, 16}, this->up, 15));

        // market center
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 20), glm::vec3{0, 2, 21}, this->up, 18));

        // apple
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 24), glm::vec3{0, 2, 25}, this->up, 33));

        // back up
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 20), glm::vec3{0, 2, 21}, this->up, 40));

        // look up
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 6, 20), glm::vec3{0, 7, 21}, this->up, 60));
    }

    // set keyframes for Night scene
    else
    {
        // look up
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 6, 20), glm::vec3{0, 7, 21}, this->up, 0));

        // back up
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 20), glm::vec3{0, 2, 21}, this->up, 20));

        // apple
        //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 24), glm::vec3{0, 2, 25}, this->up, 27));

        // market center
        //this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 20), glm::vec3{0, 2, 21}, this->up, 42));

        // forward
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(2, 2, 15), glm::vec3{3, 2, 16}, this->up, 40));

        // left
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(4, 2, 5), glm::vec3{5, 2, 6}, this->up, 48));

        // fountain
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, 2), glm::vec3{0, 2, 3}, this->up, 50));

        // init
        this->v_keyframe_.push_back(std::make_unique<KeyFrame>(glm::vec3(0, 2, -15), glm::vec3{0, 2, -13}, this->up, 60));
    }

  this->currIterator = v_keyframe_.begin();
  this->nextIterator = v_keyframe_.begin();

  ++this->nextIterator;

  float fowInRad = (ppgso::PI/180.0f) * fow;

  // glm::perspective creates a 4x4 perspective projection matrix that is used in a shader
  this->projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

/*!
 * Update Camera viewMatrix based on up, position and back vectors
 */
bool Camera::update(float dt) {
    this->age += dt;

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
    {
        return true;
    }
    else
    {
        float t = ((this->age - currentKeyFrame->time_) / (nextKeyFrame->time_ - currentKeyFrame->time_));

        this->viewMatrix = currentKeyFrame->viewMatrix_;
        this->viewMatrix = glm::interpolate
                (
                    this->viewMatrix,//currentKeyFrame->viewMatrix_,       // current key frame
                    nextKeyFrame->viewMatrix_,          // next key frame
                    t  // interpolate by t
                );

        //currentKeyFrame->viewMatrix_ = this->viewMatrix;

        //std::cout << glm::to_string(this->viewMatrix) << std::endl;
        //std::cout << "Changed, " << glm::to_string(currentKeyFrame->viewMatrix_) << "\nnext " << glm::to_string(nextKeyFrame->viewMatrix_) << " keyframes: "  << this->executedKeyFrames << " age: " << this->age << " t: " << t << '\n';
    }
    return false;
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
