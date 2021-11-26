#include "keyframe.h"

KeyFrame::KeyFrame(const glm::vec3& camera_position) :
    camera_position{camera_position}
{
  this->camera_position = camera_position;
  this->controlPoints[0] = this->camera_position;
  this->bezierShape(50);
}

glm::vec3 KeyFrame::linearInterpolation(glm::vec3 p0, glm::vec3 p1, float t) {
    glm::vec3 p;
    p.x = p0.x + (p1.x-p0.x)*t;
    p.y = p0.y + (p1.y-p0.y)*t;
    p.z = p0.z + (p1.z-p0.z)*t;
    return p;
}

/*!
* Compute points for Bezier curve using 4 control points - De Casteljau's algorithm
*/
glm::vec3 KeyFrame::bezierPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const float& t)
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
void KeyFrame::bezierShape(const int& count)
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

