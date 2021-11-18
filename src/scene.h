#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
  public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    std::unique_ptr<Camera> camera_;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects_;

    // TODO: Create more containers for objects that will not be interacting with each other
    // and we do not have to check it everytime in update method, e.g. snowball, water
    // std::list< std::unique_ptr<Object> > water_;

    // Keyboard state for interacting with the scene or with objects in the scene
    // know which keys were pressed
    std::map< int, int > keyboard_;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection_{-1.0f, -1.0f, -1.0f};

    // TODO: Define other lights here

    // Store cursor state
    struct {
      double x, y;
      bool left, right;
    } cursor_;
};

#endif // _PPGSO_SCENE_H
