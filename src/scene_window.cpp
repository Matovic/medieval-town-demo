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
//#include "generator.h"
//#include "carpet.h"
#include "sky.h"
#include "fountain.h"
#include "tower.h"
#include "floor.h"
#include "market.h"
#include "house.h"
#include "apple.h"
#include "cauldron.h"
//#include "cauldron_night.h"
#include "apple_night.h"
#include "light.h"
#include "sun.h"

/*!
* Construct custom game window
*/
SceneWindow::SceneWindow(const unsigned int& SIZE)
    : Window{"Medieval Town", static_cast<int>(SIZE), static_cast<int>(SIZE)} {
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

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    //camera->position.y = 1.5f;
    //camera->position.z = -15.0f;
    this->scene_.camera_ = move(camera);

    // Add space background
    this->scene_.objects_.push_back(std::make_unique<Sky>());

    // Add sun
    auto sun_light = std::make_unique<Light>();
    //sun->position = glm::vec3{30.0f, 45.0f, -50.0f};
    //sun->scale *= 5.0f;
    sun_light->lightDirection_= glm::vec3{30.0f, 45.0f, -50.0f};
    sun_light->lightColor_ = glm::vec3{1.0f, 1.0f, 1.0f};
    sun_light->speed = glm::vec3{-1.0f, -1.0f, 5.0f};
    sun_light->colorSpeed = glm::vec3{0.1f, 0.1f, 0.1f};
    sun_light->ambient = glm::vec3{0.2f, 0.2f, 0.2f};
    sun_light->diffuse = glm::vec3{0.05f, 0.05f, 0.05f};
    sun_light->specular = glm::vec3{0.01f, 0.01f, 0.01f};
    //auto sun_obj = std::make_move_iterator<Light>(sun);
    //this->scene_.objects_.push_back(sun);
    this->scene_.lights_.push_back(std::move(sun_light));

    auto sun_ = std::make_unique<Sun>();
    sun_->position = glm::vec3{30.0f, 45.0f, -50.0f};
    sun_->speed = glm::vec3{-1.0f, -1.0f, 5.0f};
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
    market1->final_age_ = -1.0f;
    this->scene_.objects_.push_back(move(market1));

    int market_count = 5;
    float z_offset = 0.0f;
    float market_age = 3.0f;
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
    //cauldron->rotation = {0.0f, 0.0f, ppgso::PI * 1.f};
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

    // Add lights on the houses
    auto light = std::make_unique<Light>();
    light->lightDirection_ = {15.0f - (5.0f + 0.0f), 8.5f, 30.0f};
    //light->position.y =
    //light->scale *= 0.2f;
    light->speed = {0.0f, 0.0f, 0.0f};
    //light->color = {1.0f, 0.0f, 0.0f};
    light->lightColor_ = {1.0f, 0.0f, 0.0f};
    light->colorSpeed = {-0.1f, 0.1f, 0.1f};
    light->ambient = {0.2f, 0.2f, 0.2f};
    light->diffuse = {.5f, .5f, .5f};
    light->specular = {1.0f, 1.0f, 1.0f};
    this->scene_.lights_.push_back(std::move(light));

    auto lamp = std::make_unique<Sun>();
    lamp->position = {15.0f - (5.0f + 0.0f), 8.5f, 30.0f};
    lamp->color = {1.0f, 0.0f, 0.0f};
    lamp->scale *= 0.2f;
    lamp->colorSpeed = {-0.1f, 0.1f, 0.1f};
    this->scene_.objects_.push_back(std::move(lamp));
}

/*!
* Reset and initialize the game scene
* Creating unique smart pointers to objects that are stored in the scene object list
*/
void SceneWindow::initSceneNight() {
    // we no longer
    //this->scene_.secondScene_ = true;

    // clear scene if it already was run
    this->scene_.objects_.clear();

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

    // Add carpet
    //auto carpet = std::make_unique<Carpet>();
    //carpet->final_age_ = -1.0f;
    //this->scene_.objects_.push_back(move(carpet));

    // Add cauldron
    //auto cauldron = std::make_unique<CauldronNight>();
    //cauldron->position = {0.0f, 0.5f, 0.5f};
    //cauldron->rotation = {0.0f, 0.0f, ppgso::PI * 1.f};
    //auto carpet = cauldron->carpet;
    //this->scene_.objects_.push_back(std::move(carpet));
    //this->scene_.objects_.push_back(move(cauldron));

    // Add apple
    auto apple = std::make_unique<AppleNight>();
    //apple->position = {0.0f, 2.0f, 25.0f};
    //apple->rotation = {0.0f, 0.0f, ppgso::PI * 1.f};
    this->scene_.objects_.push_back(move(apple));

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

    // Add generator to scene
    //auto generator = std::make_unique<Generator>();
    //generator->position.y = 10.0f;
    //this->scene_.objects_.push_back(move(generator));
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

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!this->scene_.secondScene_ && this->scene_.camera_->position.z > 5)
        this->scene_.firstScene_ = false;

    // if we have to traisnition to second scene
    //if (!this->scene_.firstScene_ && !this->scene_.secondScene_)
    //    this->initSceneNight();

    // Update and render all objects
    this->scene_.update(dt);
    this->scene_.render();
}
