#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
//#include <ppgso/ppgso.h>

/*!
 * Simple KeyFrame class that keeps track of viewMatrix and time to live
 * interpolation between keyframes
 */
class KeyFrame {
public:
  /*!
   * Create new KeyFrame
   * @param glm view matrix
   * @param float time fo the keyframe
   */
  KeyFrame(const glm::vec3&, const glm::vec3&, const glm::vec3&, const float&);//const glm::mat4&, const float&);
  KeyFrame(const bool&, const glm::vec3&, const glm::vec3&, const glm::vec3&, const float&);
  // viewMatrix holds where to lookAt return type
  glm::mat4 viewMatrix_;
  glm::vec3 eye_, center_, up_;
  glm::quat quatMatrix_;

  // Object properties
  glm::vec3 position{0,0,0};
  glm::vec3 rotation{0,0,0};
  glm::vec3 scale{1,1,1};
  glm::mat4 modelMatrix{1};

  // for camera and for an object
  float time_{0.0f};

};

#endif // KEYFRAME_H
