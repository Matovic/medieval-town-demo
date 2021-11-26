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
#include "scene.h"
#include "scene_night.h"
#include "generator.h"
//#include "player.h"
#include "sky.h"
#include "fountain.h"
#include "tower.h"
#include "floor.h"
#include "market.h"
#include "house.h"

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

    initScene();
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
    market1->position = {2.0f, 0.0f, 10.0f};
    this->scene_.objects_.push_back(move(market1));

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

    // Add player to the scene
    //auto player = std::make_unique<Player>();
    //player->position.y = -6;
    //this->scene_.objects_.push_back(move(player));
}

/*!
* Reset and initialize the game scene
* Creating unique smart pointers to objects that are stored in the scene object list
*/
void SceneWindow::initSceneNight() {
    // we no longer
    this->scene_.secondScene_ = true;

    // clear scene if it already was run
    this->scene_.objects_.clear();

    // Delete camera
    //this->scene_.camera_.release();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    //camera->position.y = 1.5f;
    //camera->position.z = -15.0f;
    this->scene_.camera_ = move(camera);

    // Add background
    this->scene_.objects_.push_back(std::make_unique<Sky>(false));

    // Add floor
    auto floor = std::make_unique<Floor>();
    floor->position = {0.0f, -0.01f, 2.0f};
    this->scene_.objects_.push_back(move(floor));

    // Add tower
    auto tower = std::make_unique<Tower>();
    tower->position = {-5.0f, 0.0f, 0.0f};
    tower->rotMomentum = {0.0f, 0.0f, -0.25f};
    this->scene_.objects_.push_back(move(tower));

    // Add second tower
    tower = std::make_unique<Tower>();
    tower->position = {5.0f, 0.0f, 0.0f};
    tower->rotMomentum = {0.0f, 0.0f, 0.25f};
    this->scene_.objects_.push_back(move(tower));

    // Add fountain
    auto fountain = std::make_unique<Fountain>();
    fountain->position = {0.0f, 0.0f, 15.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(fountain));

    // Add house
    auto house1 = std::make_unique<House>();
    house1->position = {-10.0f, 4.5f, 10.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(house1));

    // Add house
    auto house2 = std::make_unique<House>();
    house2->position = {-10.0f, 4.5f, 15.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(house2));

    // Add house
    auto house3 = std::make_unique<House>();
    house3->position = {-10.0f, 4.5f, 20.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(house3));

    // Add house
    auto house4 = std::make_unique<House>();
    house4->position = {10.0f, 4.5f, 10.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(house4));

    // Add house
    auto house5 = std::make_unique<House>();
    house5->position = {10.0f, 4.5f, 15.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(house5));

    // Add house
    auto house6 = std::make_unique<House>();
    house6->position = {10.0f, 4.5f, 20.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(house6));

    // Add generator to scene
    //auto generator = std::make_unique<Generator>();
    //generator->position.y = 10.0f;
    //this->scene_.objects_.push_back(move(generator));

    // Add player to the scene
    //auto player = std::make_unique<Player>();
    //player->position.y = -6;
    //this->scene_.objects_.push_back(move(player));
}

/*!
* Handles pressed key when the window is focused
* @param key Key code of the key being pressed/released
* @param scanCode Scan code of the key being pressed/released
* @param action Action indicating the key state change
* @param mods Additional modifiers to consider
*/
void SceneWindow::onKey(int key, int scanCode, int action, int mods) {
    this->scene_.keyboard_[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      this->animate_ = !this->animate_;
    }

    // move forward
    if (key == GLFW_KEY_W) {
      this->scene_.camera_->position.z += 5.0f * this->dt;
    }

    // move backwards
    if (key == GLFW_KEY_S) {
      this->scene_.camera_->position.z -= 5.0f * this->dt;
    }

    // move right
    if (key == GLFW_KEY_D) {
      this->scene_.camera_->position.x -= 5.0f * this->dt;
    }

    // move left
    if (key == GLFW_KEY_A) {
      this->scene_.camera_->position.x += 5.0f * this->dt;;
    }

    // move up
    if (key == GLFW_KEY_SPACE) {
      this->scene_.camera_->position.y += 5.0f * this->dt;
    }

    // move down
    if (key == GLFW_KEY_X) {
      this->scene_.camera_->position.y -= 5.0f * this->dt;;
    }

    // rotate left
    if (key == GLFW_KEY_Q) {
      this->scene_.camera_->rotation.x += std::sin(glm::radians(80.0f)) * this->dt;

        /*
direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
direction.y = sin(glm::radians(pitch));
direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
*/
    }

    // rotate right
    if (key == GLFW_KEY_E) {
      this->scene_.camera_->rotation.x -= std::sin(glm::radians(80.0f)) * this->dt;
    }
}

/*!
* Handle cursor position changes
* @param cursorX Mouse horizontal position in window coordinates
* @param cursorY Mouse vertical position in window coordinates
*/
void SceneWindow::onCursorPos(double cursorX, double cursorY) {
    this->scene_.cursor_.x = cursorX;
    this->scene_.cursor_.y = cursorY;
}

/*!
* Handle cursor buttons
* @param button Mouse button being manipulated
* @param action Mouse bu
* @param mods
*/
void SceneWindow::onMouseButton(int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {

        this->scene_.camera_->rotation.y -= std::sin(glm::radians(80.0f)) * this->dt;
      /*this->scene_.cursor_.left = action == GLFW_PRESS;

        if (this->scene_.cursor_.left) {
            // Convert pixel coordinates to Screen coordinates
            double u = (this->scene_.cursor_.x / width - 0.5f) * 2.0f;
            double v = - (this->scene_.cursor_.y / height - 0.5f) * 2.0f;

            // Get mouse pick vector in world coordinates
            auto direction = this->scene_.camera_->cast(u, v);
            auto position = this->scene_.camera_->position;

            // Get all objects in scene intersected by ray
            auto picked = this->scene_.intersect(position, direction);

            // Go through all objects that have been picked
            for (auto &obj: picked) {
              // Pass on the click event
              obj->onClick(this->scene_);
            }
        }*/
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      //this->scene_.cursor_.right = action == GLFW_PRESS;
        this->scene_.camera_->rotation.y += std::sin(glm::radians(80.0f)) * this->dt;
    }
}

/*!
* Window update implementation that will be called automatically from pollEvents
*/
void SceneWindow::onIdle() {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta based on bool variable animate
    this->dt = this->animate_ ? (float) glfwGetTime() - time : 0;

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
