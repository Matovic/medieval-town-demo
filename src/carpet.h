#ifndef MEDIEVAL_TOWN_CARPET_H
#define MEDIEVAL_TOWN_CARPET_H
#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
//#include "cauldron_night.h"
#include <ppgso/ppgso.h>

/*!
 * Simple carpet object
 * This sphere object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 * It will move down along the Y axis and self delete when reaching below -10
 */
class Carpet : public Object {
private:
    // Static resources (Shared between instances)
    // every object needs mesh, shader to render and texture
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    //std::unique_ptr<CauldronNight> cauldron;

public:
    /*!
     * Create new asteroid
     */
    Carpet();

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
    float age = 0.0f;

protected:
    virtual void generateModelMatrix_child() {};
};


#endif //MEDIEVAL_TOWN_CARPET_H
