#include <glm/glm.hpp>

#include "camera.h"

/*!
 * Create new Camera that will generate viewMatrix and projectionMatrix based on its
 * position, up and back vectors
 * @param fow - Field of view in degrees
 * @param ratio - Viewport screen ratio (usually width/height of the render window)
 * @param near - Distance to the near frustum plane
 * @param far - Distance to the far frustum plane
 */
Camera::Camera(float fow, float ratio, float near, float far) :
    keyframe_{std::make_unique<KeyFrame>(this->position)}
{
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

    // matrix transform for build a camera view
      //      eye (camera position), center (camera target, where we are looking)), up() = (0,0,1)(towards z)
    this->viewMatrix = lookAt(this->position, this->position - this->back + this->rotation, this->up);//this->position - this->back + this->rotation, this->up);
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
