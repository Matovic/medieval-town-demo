// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Sky objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "scene_window.h"
#include "camera.h"
#include "keyframe.h"
#include "scene.h"
#include "scene_night.h"
#include "sky.h"
#include "fountain.h"
#include "tower.h"
#include "floor.h"
#include "market.h"
#include "house.h"
#include "apple.h"
#include "cauldron.h"
#include "apple_night.h"
#include "light.h"
#include "sun.h"

/*!
* Construct custom game window
*/
SceneWindow::SceneWindow(const unsigned int& height, const unsigned int& width)
    : Window{"Medieval Town", static_cast<int>(height), static_cast<int>(width)} {
    //hideCursor();

    // bind keys to window
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    this->initScene();
    //this->initSceneNight();
}

/*!
* Reset and initialize the game scene
* Creating unique smart pointers to objects that are stored in the scene object list
*/
void SceneWindow::initScene() {
    // clear scene if it already was run
    this->scene_.objects_.clear();

    // clear lights
    this->scene_.lights_.clear();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    this->scene_.camera_ = std::move(camera);

    // Add space background
    this->scene_.objects_.push_back(std::make_unique<Sky>());

    // Add sun
    auto sun_light = std::make_unique<Light>();
    sun_light->lightDirection_= glm::vec3{0.0f, 20.0f, 5.0f};
    sun_light->lightColor_ = glm::vec3{1.0f, 1.0f, 1.0f};
    sun_light->speed = glm::vec3{.0f, .0f, 1.0f};
    sun_light->colorSpeed = glm::vec3{0.5f, 0.5f, 0.5f};
    sun_light->ambient = glm::vec3{0.08f, 0.08f, 0.08f};
    sun_light->diffuse = glm::vec3{0.2f, 0.2f, 0.1f};
    sun_light->specular = glm::vec3{0.01f, 0.01f, 0.01f};
    this->scene_.lights_.push_back(std::move(sun_light));

    auto sun_ = std::make_unique<Sun>();
    sun_->position = glm::vec3{0.0f, 20.0f, 5.0f};//30.0f, 45.0f, -50.0f};
    sun_->speed = glm::vec3{.0f, .0f, 1.0f};
    sun_->scale *= 2.0f;
    this->scene_.objects_.push_back(std::move(sun_));

    // Add floor
    auto floor = std::make_unique<Floor>();
    floor->position = {0.0f, -0.01f, 12.0f};
    this->scene_.objects_.push_back(move(floor));

    float x_offset = 0.0f;
    // add towers
    for (unsigned int i = 0; i < 3; ++i, x_offset += 5.0f)
    {
        // Add tower from right side
        auto tower = std::make_unique<Tower>();
        tower->position = {-5.0f - x_offset, 0.0f, 0.0f};
        tower->rotMomentum = {0.0f, 0.0f, -0.25f};
        this->scene_.objects_.push_back(move(tower));

        // Add tower from left side
        tower = std::make_unique<Tower>();
        tower->position = {5.0f + x_offset, 0.0f, 0.0f};
        tower->rotMomentum = {0.0f, 0.0f, 0.25f};
        this->scene_.objects_.push_back(move(tower));
    }

    // Add fountain
    auto fountain = std::make_unique<Fountain>();
    fountain->position = {0.0f, 0.0f, 15.0f};
    this->scene_.objects_.push_back(move(fountain));

    // Add markets
    auto market1 = std::make_unique<Market>();
    market1->position = {1.f, 0.0f, 25.0f};
    market1->rotation = {0.0f, 0.0f, ppgso::PI * 1.f};
    market1->final_age_ = 40.0f;//-1.0f;
    this->scene_.objects_.push_back(move(market1));

    int market_count = 5;
    float z_offset = 0.0f;
    float market_age = 12.0f;
    while (market_count != 0)
    {
        auto market = std::make_unique<Market>();
        auto market2 = std::make_unique<Market>();
        market->position = {glm::linearRand(-10.0f, -5.0f), 0.0f, 8.f + z_offset};
        market->rotation = {0.0f, 0.0f, ppgso::PI * glm::linearRand(0.f, 3.14f)};
        market2->position = {glm::linearRand(5.0f, 10.0f), 0.0f, 8.f + z_offset};
        market2->rotation = {0.0f, 0.0f, ppgso::PI * glm::linearRand(0.f, 3.14f)};
        market_count--;
        z_offset += 5.0f;
        market->final_age_ = market_age;
        market2->final_age_ = market_age;
        market_age += 1.0f;
        this->scene_.objects_.push_back(move(market));
        this->scene_.objects_.push_back(move(market2));
    }

    // Add apple
    auto apple = std::make_unique<Apple>();
    apple->position = {0.f, 1.8f, 25.3f};
    apple->rotation = {0.0f, 0.0f, ppgso::PI * 1.f};
    this->scene_.objects_.push_back(move(apple));

    // Add cauldron
    auto cauldron = std::make_unique<Cauldron>();
    cauldron->position = {0.f, 1.2f, 25.3f};
    this->scene_.objects_.push_back(move(cauldron));

    // add houses
    z_offset = 0.0f;
    for (unsigned int i = 0; i < 6; ++i, z_offset += 5.0f)
    {
        // add house on 1 side
        auto house = std::make_unique<House>();
        house->position = {-15.0f, 4.5f, 5.0f + z_offset};
        this->scene_.objects_.push_back(move(house));

        // add house on the second side
        house = std::make_unique<House>();
        house->position = {15.0f, 4.5f, 5.0f + z_offset};
        house->rotation = {0.0f, 0.0f, ppgso::PI * 1.5f};
        this->scene_.objects_.push_back(move(house));

        // add 5 houses on the back side
        if ( i > 4)
            continue;
        house = std::make_unique<House>();
        house->position = {15.0f - (5.0f + z_offset), 4.5f, 35.0f};
        house->rotation = {0.0f, 0.0f, ppgso::PI};
        this->scene_.objects_.push_back(move(house));
    }
}

/*!
* Reset and initialize the game scene
* Creating unique smart pointers to objects that are stored in the scene object list
*/
void SceneWindow::initSceneNight() {
    // we are in the night scene
    this->scene_.secondScene_ = true;

    // clear scene if it already was run
    this->scene_.objects_.clear();

    // clear lights
    this->scene_.lights_.clear();

    // Delete camera
    //this->scene_.camera_.release();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f, false);
    //camera->position.y = 1.5f;
    //camera->position.z = -15.0f;
    this->scene_.camera_ = move(camera);

    // Add background
    this->scene_.objects_.push_back(std::make_unique<Sky>(false));

    // Add floor
    auto floor = std::make_unique<Floor>();
    floor->position = {0.0f, -0.01f, 12.0f};
    this->scene_.objects_.push_back(move(floor));

    // Add moon
    auto moon_light = std::make_unique<Light>();
    moon_light->lightDirection_= glm::vec3{0.0f, 20.0f, 65.0f};
    moon_light->lightColor_ = glm::vec3{1.0f, 1.0f, 1.0f};
    moon_light->speed = glm::vec3{0.0f, 0.0f, -1.0f};
    moon_light->colorSpeed = glm::vec3{0.5f, 0.5f, 0.5f};
    moon_light->ambient = glm::vec3{0.05f, 0.05f, 0.05f};
    moon_light->diffuse = glm::vec3{0.02f, 0.02f, 0.01f};
    moon_light->specular = glm::vec3{0.01f, 0.01f, 0.01f};
    this->scene_.lights_.push_back(std::move(moon_light));

    auto moon = std::make_unique<Sun>();
    moon->position = glm::vec3{0.0f, 20.0f, 65.0f};
    moon->color = {1.0f, 1.0f, 1.0f};
    moon->speed = glm::vec3{0.0f, 0.0f, -1.0f};
    moon->scale *= 2.0f;
    this->scene_.objects_.push_back(std::move(moon));

    // Add cauldron
    auto cauldron = std::make_unique<Cauldron>(false);
    cauldron->keyframes_activated = true;
    this->scene_.objects_.push_back(move(cauldron));

    float x_offset = 0.0f;
    // add towers
    for (unsigned int i = 0; i < 3; ++i, x_offset += 5.0f)
    {
        // Add tower from right side
        auto tower = std::make_unique<Tower>();
        tower->position = {-5.0f - x_offset, 0.0f, 0.0f};
        tower->rotMomentum = {0.0f, 0.0f, -0.25f};
        this->scene_.objects_.push_back(move(tower));

        // Add tower from left side
        tower = std::make_unique<Tower>();
        tower->position = {5.0f + x_offset, 0.0f, 0.0f};
        tower->rotMomentum = {0.0f, 0.0f, 0.25f};
        this->scene_.objects_.push_back(move(tower));
    }

    // Add fountain
    auto fountain = std::make_unique<Fountain>();
    fountain->position = {0.0f, 0.0f, 15.0f};
    this->scene_.objects_.push_back(move(fountain));

    // Add lights everywhere
    for (int i = 0; i < 99; ++i)
    {
        glm::vec3 lightColor{0.05 + 0.10 * (sin(dt * 1.20)), 0.95 + 0.25 * (sin(dt * 1.20)), 0.05 + 0.25 * (sin(dt * 1.20))};
        glm::vec3 colorSpeed{glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f)};
        auto light = std::make_unique<Light>();
        float x_rand = glm::linearRand(-10.0f, 10.0f);
        float z_rand = glm::linearRand(0.0f, 35.0f);
        float y_speed_rand = glm::linearRand(-1.0f, -0.1f);
        light->lightDirection_ = {x_rand, 25.0f, z_rand};
        light->speed = {0.1f, y_speed_rand, 0.0f};//-0.981f, 0.0f};
        light->lightColor_ = lightColor;
        light->colorSpeed = colorSpeed;
        light->ambient = {0.002f, 0.002f, 0.002f};
        light->diffuse = {0.0005f, 0.0005f, 0.0005f};
        light->specular = {0.001f, 0.001f, 0.001f};
        this->scene_.lights_.push_back(std::move(light));

        auto lamp = std::make_unique<Sun>();
        lamp->position = glm::vec3{x_rand, 25.0f, z_rand};
        lamp->speed = {0.1f, y_speed_rand, 0.0f};
        lamp->color = lightColor;
        lamp->scale *= 0.2f;
        lamp->colorSpeed = colorSpeed;
        this->scene_.objects_.push_back(std::move(lamp));
    }

    // Add apple, cauldron & carpet with hierarchical transformations
    this->scene_.objects_.push_back(std::make_unique<AppleNight>());

    // add houses
    float z_offset = 0.0f;
    for (unsigned int i = 0; i < 6; ++i, z_offset += 5.0f)
    {
        // add house on 1 side
        auto house = std::make_unique<House>();
        house->position = {-15.0f, 4.5f, 5.0f + z_offset};
        this->scene_.objects_.push_back(move(house));

        // add house on the second side
        house = std::make_unique<House>();
        house->position = {15.0f, 4.5f, 5.0f + z_offset};
        house->rotation = {0.0f, 0.0f, ppgso::PI * 1.5f};
        this->scene_.objects_.push_back(move(house));

        // add 5 houses on the back side
        if ( i > 4)
            continue;
        house = std::make_unique<House>();
        house->position = {15.0f - (5.0f + z_offset), 4.5f, 35.0f};
        house->rotation = {0.0f, 0.0f, ppgso::PI};
        this->scene_.objects_.push_back(move(house));
    }
}

/*!
* Window update implementation that will be called automatically from pollEvents
*/
void SceneWindow::onIdle() {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta based on bool variable animate
    //this->dt = this->animate_ ? (float) glfwGetTime() - time : 0;
    this->dt = (float) glfwGetTime() - time;

    time = (float) glfwGetTime();

    /*if (!this->scene_.secondScene_ && this->scene_.camera_->position.z > 5)
        this->scene_.firstScene_ = false;*/

    // if we have to traisnition to second scene
    if (!this->scene_.firstScene_ && !this->scene_.secondScene_)
        this->initSceneNight();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    this->scene_.update(dt);
    this->scene_.render();
}
