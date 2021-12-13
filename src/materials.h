#ifndef materials_H
#define materials_H

//#include <list>
//#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Materials {
public:

    Materials(const glm::vec3&, const glm::vec3&, const glm::vec3&, const std::string&);

    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };

    public:
    static Material getMaterialValues(std::string type);
};

#endif
