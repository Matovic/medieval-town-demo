#include "keyframe.h"

KeyFrame::KeyFrame(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, const float& time)//const glm::mat4& viewMatrix, const float& time)
    : eye_{std::move(eye)}, center_{std::move(center)}, up_{std::move(up)}, time_{time} //viewMatrix_{std::move(viewMatrix)}, time_{time}
{
    this->quatMatrix_ = glm::lookAt(this->eye_, this->center_, this->up_);
    this->viewMatrix_ = glm::lookAt(this->eye_, this->center_, this->up_);
}
