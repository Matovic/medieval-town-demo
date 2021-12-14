#include "keyframe.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

KeyFrame::KeyFrame(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, const float& time)//const glm::mat4& viewMatrix, const float& time)
    : eye_{std::move(eye)}, center_{std::move(center)}, up_{std::move(up)}, time_{time} //viewMatrix_{std::move(viewMatrix)}, time_{time}
{
    //this->quatMatrix_ = glm::lookAt(this->eye_, this->center_, this->up_);
    this->viewMatrix_ = glm::lookAt(this->eye_, this->center_, this->up_);
    this->quatMatrix_ = glm::quat_cast(this->viewMatrix_);
}

KeyFrame::KeyFrame(const bool& isCauldron, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const float& time)
    : position{std::move(position)}, rotation{std::move(rotation)}, scale{std::move(scale)}, time_{time}
{
    this->modelMatrix =
            glm::translate(glm::mat4(1.0f), this->position)
            * glm::orientate4(this->rotation)
            * glm::scale(glm::mat4(1.0f), this->scale);
}
