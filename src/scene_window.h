#ifndef _PPGSO_SCENE_WINDOW_H
#define _PPGSO_SCENE_WINDOW_H

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

#include "scene.h"

//const unsigned int SIZE = 1024;

/*!
 * Custom windows for our simple demo
 * Container for other objects
 */
class SceneWindow : public ppgso::Window {
public:
  /*!
   * Construct custom game window
   */
  SceneWindow(const unsigned int&);

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override;

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override;

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override;

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override;
  
private:
  Scene scene_;
  bool animate_ = true;
  //bool animate_ = false;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene();
};

#endif // _PPGSO_SCENE_WINDOW_H

