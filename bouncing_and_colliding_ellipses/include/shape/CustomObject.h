//
// Created by xihan1 on 8/26/21.
//

#ifndef INC_123456789_boucing_and_colliding_ellipses_CUSTOM_OBJECT_H
#define INC_123456789_boucing_and_colliding_ellipses_CUSTOM_OBJECT_H

#include <vector>

#include <glm/glm.hpp>


class CustomObject
{
public:
    CustomObject() = default;

    CustomObject(double screenWidth_, double screenHeight_);

    virtual ~CustomObject() = 0;

    /// \return a vector that can be used as data of OpenGL vertex buffer
    virtual std::vector<glm::vec2> getVertexBufferData();

protected:
    /// add a point to the line
    /// \param x screen space coordinate
    /// \param y screen space coordinate
    void appendToVBA(std::vector<glm::vec2> vba);

protected:
    double screenWidth {1};
    double screenHeight {1};

    /// data buffer for all points this line segment passes through
    /// in absolute coordinates
    std::vector<glm::vec2> path;

    /// data buffer for VBO, contains 3D points in normalized coordinate
    /// will be transformed from `path`, i.e. from absolute coordinate to NDC
    // bool vertexBufferDataReady {false};
    std::vector<glm::vec2> vertexBufferData;
};


#endif //INC_123456789_boucing_and_colliding_ellipses_CUSTOM_OBJECT_H
