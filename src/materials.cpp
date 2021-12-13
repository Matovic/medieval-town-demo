//#include <string>
#include "materials.h"

Materials::Materials(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const std::string& type)
{

}

Materials::Material Materials::getMaterialValues(std::string type) {
    Materials::Material shinnyMaterial;

    shinnyMaterial = (Materials::Material) {
            .ambient = glm::vec3(0.2f, 0.2f, 0.2f),
            .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
            .specular = glm::vec3(1.0f, 1.0f, 1.0f),
            .shininess = 0.21794872f * 128.f // Cim vacsia hodnota, tym viac je to svetlo bodove
    };

    if (type == "Shinny") {
        return shinnyMaterial;
    }
    return shinnyMaterial;
}
