#ifndef MEDIEVAL_TOWN_DEMO_CAULDRON_H
#define MEDIEVAL_TOWN_DEMO_CAULDRON_H


#include <ppgso/ppgso.h>
#include <list>
#include "scene.h"
#include "object.h"
#include "keyframe.h"

/*!
 * Simple asteroid object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 * It will move down along the Y axis and self delete when reaching below -10
 */
class Cauldron final : public Object {
private:
    // Static resources (Shared between instances)
    // every object needs mesh, shader to render and texture
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create new asteroid
     */
    Cauldron();

    /*!
     * Update
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    // animate with keyfgrames
    std::list<std::unique_ptr<KeyFrame>> v_keyframe_;
    std::list<std::unique_ptr<KeyFrame>>::iterator currIterator;
    std::list<std::unique_ptr<KeyFrame>>::iterator nextIterator;
    size_t executedKeyFrames = 1;
    bool keyframes_activated = false;
    glm::vec3 scale{1.f,1.f,1.f};


    /*!
     * Rotational momentum
     */
    glm::vec3 rotMomentum;
    float age = 0.0f;
    float keyframe_TTL = 0.0f;
};





#endif //MEDIEVAL_TOWN_DEMO_CAULDRON_H
