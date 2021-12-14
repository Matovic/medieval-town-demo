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
#include "scene_night.h"

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
  SceneWindow(const unsigned int& height, const unsigned int& width);

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override;

private:
  Scene scene_;
  //SceneNight sceneNight_;
  bool animate_ = true;
  //bool animate_ = false;

  // delta time for animation
  float dt = 0.0f;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene();

  /*!
   * Reset and initialize the new scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initSceneNight();
};

#endif // _PPGSO_SCENE_WINDOW_H

