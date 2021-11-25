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
Camera::Camera(float fow, float ratio, float near, float far) {
  this->bezierShape(15);
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
    if (this->index < this->points.size())
        this->position += this->points[this->index++] * dt;
    /*else{
        this->position -= this->speed;
        std::cout <<"LA\n";
    }
    std::cout << position.z << '\n';*/

    // matrix transform for build a camera view
      //      eye (camera position) = viewRotation, center (camera target, where we are looking) = vektor(0,0,0),
      //      up() = (0,0,1)(towards z)
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

glm::vec3 Camera::linearInterpolation(glm::vec3 p0, glm::vec3 p1, float t) {
    glm::vec3 p;
    p.x = p0.x + (p1.x-p0.x)*t;
    p.y = p0.y + (p1.y-p0.y)*t;
    p.z = p0.z + (p1.z-p0.z)*t;
    return p;
}

/*!
* Compute points for Bezier curve using 4 control points - De Casteljau's algorithm
*/
glm::vec3 Camera::bezierPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const float& t)
{

    auto a = linearInterpolation(p0, p1, t);
    auto b = linearInterpolation(p1, p2, t);
    auto c = linearInterpolation(p2, p3, t);
    auto ab = linearInterpolation(a, b, t);
    auto bc = linearInterpolation(b, c, t);
    return linearInterpolation(ab, bc, t);

    //return linearInterpolation(p0, p1, t);
}

/*!
* Compute points for a sequence of Bezier curves defined by a vector of control points
* Each bezier curve will reuse the end point of the previous curve
* @param count - Number of points to generate on each curve
*/
void Camera::bezierShape(const int& count)
{
    for(int i = 1; i < (int) controlPoints.size(); i+=3) {
      for (int j = 0; j <= count; j++) {
        // TODO: Generate points for each Bezier curve and insert them
        // move t on every point based on j * (1/count), needs to be casted to float
        float t = static_cast<float>(j) / static_cast<float>(count);

        // first curve has 4 points, other curves have 3 points, but every previous point is used as beginning
        glm::vec3 point = bezierPoint(controlPoints[i - 1], controlPoints[i],
                controlPoints[i + 1], controlPoints[i + 2], t);
        //glm::vec3 point = bezierPoint(controlPoints[i - 1], controlPoints[i], t);
        points.emplace_back(point);
      }
    }
}
