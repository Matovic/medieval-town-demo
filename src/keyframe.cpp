#include "keyframe.h"

KeyFrame::KeyFrame(const glm::mat4& viewMatrix, const float& time)
    : viewMatrix_{std::move(viewMatrix)}, time_{time}
{

}
