#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
  glm::vec3 up{0,1,0};
  glm::vec3 position{0,2,0};
  glm::vec3 back{0,0,-1};
  glm::vec3 speed{0,0,2.0f};

  //
  glm::vec3 rotation{0,0,0};

  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  /*!
   * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
   * @param fow - Field of view in degrees
   * @param ratio - Viewport screen ratio (usually width/height of the render window)
   * @param near - Distance to the near frustum plane
   * @param far - Distance to the far frustum plane
   */
  Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

  /*!
   * Update Camera viewMatrix based on up, position and back vectors
   */
  void update(float dt);

  /*!
   * Get direction vector in world coordinates through camera projection plane
   * @param u - camera projection plane horizontal coordinate [-1,1]
   * @param v - camera projection plane vertical coordinate [-1,1]
   * @return Normalized vector from camera position to position on the camera projection plane
   */
  glm::vec3 cast(double u, double v);

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
      //mushroom
      {0.0, 5.0, 0.0},
      {5.0, 0.0, 20.0},
      {0.0, 5.0, 40.0},
      {-5.0, 0.0, 60.0},/*
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

  unsigned int index = 0;
};

