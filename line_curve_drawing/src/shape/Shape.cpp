//
// Created by xihan1 on 8/26/21.
//

#include <algorithm>

#include "shape/Shape.h"


Shape::Shape(double screenWidth_, double screenHeight_) :
        screenWidth(screenWidth_), screenHeight(screenHeight_)
{

}


Shape::~Shape() = default;


std::vector<glm::vec2> Shape::getVertexBufferData()
{
    if (!vertexBufferDataReady)
    {
        vertexBufferData.clear();
        vertexBufferData.reserve(path.size());

        std::transform(path.cbegin(),
                       path.cend(),
                       std::back_inserter(vertexBufferData),
                       [screenWidth = this->screenWidth, screenHeight = this->screenHeight](const glm::vec2 & pt)
                       {
                           // Map absolute coordinate into normalized device coordinate (NDC), i.e.:
                           // x: [0, screenWidth - 1] -> [-1, 1]
                           // y: [0, screenHeight - 1] -> [-1, 1]
                           return glm::vec2 {2.0 * pt.x / screenWidth - 1.0, 2.0 * pt.y / screenHeight - 1.0};
                       });

        vertexBufferDataReady = true;
    }

    return vertexBufferData;
}


void Shape::appendToPath(int x, int y)
{
    path.emplace_back(x, y);
}
