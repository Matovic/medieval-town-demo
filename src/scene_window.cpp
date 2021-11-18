// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
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
#include "generator.h"
//#include "player.h"
//#include "space.h"

#include "tower.h"

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
    camera->position.z = -15.0f;
    this->scene_.camera_ = move(camera);

    // Add space background
    //this->scene_.objects_.push_back(std::make_unique<Space>());
    auto tower = std::make_unique<Tower>();
    tower->position = {-5.0f, 0.0f, 0.0f};
    //tower->position.x -= 5.0f;
    this->scene_.objects_.push_back(move(tower));

    auto tower2 = std::make_unique<Tower>();
    tower2->position = {5.0f, 0.0f, 0.0f};
    //tower2->position.x += 5.0f;
    this->scene_.objects_.push_back(move(tower2));

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
      this->scene_.cursor_.left = action == GLFW_PRESS;

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
        }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      this->scene_.cursor_.right = action == GLFW_PRESS;
    }
}

/*!
* Window update implementation that will be called automatically from pollEvents
*/
void SceneWindow::onIdle() {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta based on bool variable animate
    float dt = this->animate_ ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    this->scene_.update(dt);
    this->scene_.render();
}
