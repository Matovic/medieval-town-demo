#include "light.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


Light::Light() 
{
}

bool Light::update(float dt) {
    if (this->lightDirection_.y > 0)
        this->lightDirection_ += this->speed * dt;
    this->lightDirection_ += this->speed * dt;
    this->lightColor_ += this->colorSpeed * dt;
    this->age += dt;
    if (this->final_age > 0 && this->age > this->final_age)
        return false;
    return true;
}
