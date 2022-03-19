//
// Created by xihan1 on 8/30/21.
//

#include <iostream>

#include "shape/CustomObject.h"
#include "shape/Ellipses.h"
#include "shape/Ellipse.h"
#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include "glm/gtx/string_cast.hpp"
#define PI 3.14159265


Ellipses::Ellipses(std::vector<std::vector<double>> ellipses, bool shaded, double screenWidth_, double screenHeight_) :
        CustomObject(screenWidth_, screenHeight_)
{
    std::vector<glm::vec2> tempVBA;
    for (auto& row: ellipses){
        tempVBA = Ellipse(row[0],row[1],row[2],row[3],row[4], shaded,
                               screenWidth_,
                               screenHeight_).getVertexBufferData();
        appendToVBA(tempVBA);
    }
}
