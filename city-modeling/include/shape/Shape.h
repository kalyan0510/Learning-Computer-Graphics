#ifndef city_modeling_SHAPE_H
#define city_modeling_SHAPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>


// An abstract class to represent generic shapes,
// just consider it a Java interface if you are new to C++ polymorphism.
// An "= 0;" says that the function is pure virtual and there will be no definition
// (just like APIs in Java interfaces).
class Shape
{
public:
    [[nodiscard]] virtual GLint getNumVertices() const = 0;

    [[nodiscard]] virtual const glm::vec3 * getVertexData() const = 0;

    [[nodiscard]] virtual const glm::vec3 * getNormalData(int mode) const = 0;

    [[nodiscard]] virtual const std::vector<glm::vec3> &getVertexVec() const = 0;
    [[nodiscard]] virtual const std::vector<glm::vec3> &getNormalVec(int mode) const = 0;
};



#endif  // city_modeling_SHAPE_H
