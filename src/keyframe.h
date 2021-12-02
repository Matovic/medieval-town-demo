#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>

#include <glm/glm.hpp>
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
  KeyFrame(const glm::mat4&, const float&);

  // viewMatrix holds where to lookAt return type
  glm::mat4 viewMatrix_;

  float time_{0.0f};
};

#endif // KEYFRAME_H
