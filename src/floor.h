//
// Created by Peter Oliver Kolek on 23/11/2021.
//

#ifndef MEDIEVAL_TOWN_DEMO_FLOOR_H
#define MEDIEVAL_TOWN_DEMO_FLOOR_H
#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple asteroid object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 * It will move down along the Y axis and self delete when reaching below -10
 */
class Floor final : public Object {
private:
    // Static resources (Shared between instances)
    // every object needs mesh, shader to render and texture
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Age of the object in seconds
    float age{0.0f};

    // Speed and rotational momentum
    //glm::vec3 speed;
    glm::vec3 rotMomentum;

    /*!
     * Split the asteroid into multiple pieces and spawn an explosion object.
     *
     * @param scene - Scene to place pieces and explosion into
     * @param explosionPosition - Initial position of the explosion
     * @param explosionScale - Scale of the explosion
     * @param pieces - Asteroid pieces to generate
     */
    //void explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale, int pieces);

public:
    /*!
     * Create new asteroid
     */
    Floor();

    /*!
     * Update tower
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render tower
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    /*!
     * Custom click event for asteroid
     */
    //void onClick(Scene &scene) override;

private:
};



#endif //MEDIEVAL_TOWN_DEMO_FLOOR_H
