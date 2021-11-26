#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>

#include <glm/glm.hpp>
//#include <ppgso/ppgso.h>

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class KeyFrame {
public:
  /*!
   * Create new KeyFrame
   * @param camera_position is initial camera position
   */
  KeyFrame(const glm::vec3& camera_position);

  /*!
  * Compute points for a sequence of Bezier curves defined by a vector of control points
  * Each bezier curve will reuse the end point of the previous curve
  * @param count - Number of points to generate on each curve
  */
  void bezierShape(const int& count);

  /*!
  * Compute points for Bezier curve using 4 control points - De Casteljau's algorithm
  */
  glm::vec3 bezierPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const float& t);

  glm::vec3 linearInterpolation(glm::vec3 p0, glm::vec3 p1, float t);

  // Control points for the bezier curves
  // First curve is 4 control points
  // Rest of the curves are 3 control points, each reusing the last curve end
  // Defines the letter "R"
  std::vector<glm::vec3> controlPoints = {
      {0.0, 5.0, 0.0},
      {5.0, 0.0, 20.0},
      {10.0, 5.0, 40.0},
      {15.0, 0.0, 60.0},/*
      {-15.0, 5.0, 80.0},
      {-25.0, 5.0, 100.0},
      {-50.0, 5.0, 120.0},
      {0.5,1},
      {0.5,0.2},
      {0.5,0.1},
      {0.1,0.1},
      {0.1,0.1},
      {0.2,0.1},
      {0.2,0.5},
      {0,0.5},*/
      // R
      //{ 0,  -1},    // 1. curve, P0
      //{ 0, -.3},    // 1. curve, P0
      //{ 0,  .3},    // 1. curve, P0
      //{ 0,   1},    // 1. curve, P0
      //{.3,   1},    // 2. curve, P0
      //{.5,   1},    // 2. curve, P0
      //{.5,  .5},    // 2. curve
      //{.5,   0},    // 3. curve
      //{.3,   0},    // 3. curve
      //{ 0,   0},    // 3. curve
      //{.3, -.3},    // 4. curve
      //{.5, -.5},    // 4. curve
      //{.5,  -1},    // 4. curve
  };
  // This will hold the bezier curve geometry once we generate it
  std::vector<glm::vec3> points;

  std::vector<glm::vec3> rotation_points;
  std::vector<float> time_points;

  glm::vec3 camera_position;

  unsigned int index = 0;
};

#endif // KEYFRAME_H
