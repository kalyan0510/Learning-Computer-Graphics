//
// Created by xihan1 on 8/26/21.
//

#include <algorithm>

#include "shape/CustomObject.h"


CustomObject::CustomObject(double screenWidth_, double screenHeight_) :
        screenWidth(screenWidth_), screenHeight(screenHeight_)
{

}


CustomObject::~CustomObject() = default;


std::vector<glm::vec2> CustomObject::getVertexBufferData()
{
    // if (!vertexBufferDataReady)
    // {
    //     vertexBufferData.clear();
    //     vertexBufferData.reserve(path.size());

    //     std::transform(path.cbegin(),
    //                    path.cend(),
    //                    std::back_inserter(vertexBufferData),
    //                    [screenWidth = this->screenWidth, screenHeight = this->screenHeight](const glm::vec2 & pt)
    //                    {
    //                        // Map absolute coordinate into normalized device coordinate (NDC), i.e.:
    //                        // x: [0, screenWidth - 1] -> [-1, 1]
    //                        // y: [0, screenHeight - 1] -> [-1, 1]
    //                        return glm::vec2 {2.0 * pt.x / screenWidth - 1.0, 2.0 * pt.y / screenHeight - 1.0};
    //                    });

    //     vertexBufferDataReady = true;
    // }

    return vertexBufferData;
}


void CustomObject::appendToVBA(std::vector<glm::vec2> vbaData)
{
    vertexBufferData.insert(vertexBufferData.end(), vbaData.begin(), vbaData.end());
}
