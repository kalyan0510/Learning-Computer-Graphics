#include <iostream>

#include "shape/CustomObject.h"
#include "shape/Faces.h"
#include "shape/LineSegment.h"
#include "shape/Ellipse.h"
#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include "glm/gtx/string_cast.hpp"
#define PI 3.14159265


Faces::Faces(std::vector<std::vector<double>> faces, double screenWidth_, double screenHeight_) :
        CustomObject(screenWidth_, screenHeight_)
{
    std::vector<glm::vec2> tempVBA;
    for (auto& row: faces){
        makeFace(row, 1, screenWidth_, screenHeight_);
    }
    // printf("\n%d %d\n", faces.size(), vertexBufferData.size());
}

void Faces::makeFace(std::vector<double> f, int generation, double screenWidth_, double screenHeight_ ) {
    if(f[5]<generation) {
        return;
    }
    // printf("YOOYOYO");
    std::vector<glm::vec2> tempVBA;
    if(generation==1){
        tempVBA = Ellipse(f[0], f[1], f[2], f[3], 0, screenWidth_, screenHeight_).getVertexBufferData();
        appendToVBA(tempVBA);
    }
    tempVBA = Ellipse(f[0]-f[2]/2, f[1], f[2]/2, f[3]/2, 0, screenWidth_, screenHeight_).getVertexBufferData();
    appendToVBA(tempVBA);
    tempVBA = Ellipse(f[0]+f[2]/2, f[1], f[2]/2, f[3]/2, 0, screenWidth_, screenHeight_).getVertexBufferData();
    appendToVBA(tempVBA);
    
    tempVBA = LineSegment(f[0]-f[2]/6, f[1]-f[3]/2, f[0], f[1]- 3*f[3]/4, screenWidth_, screenHeight_).getVertexBufferData();
    appendToVBA(tempVBA);

    tempVBA = LineSegment(f[0]+f[2]/6, f[1]-f[3]/2, f[0], f[1]- 3*f[3]/4, screenWidth_, screenHeight_).getVertexBufferData();
    appendToVBA(tempVBA);

    tempVBA = LineSegment(f[0]-f[2]/6, f[1]-f[3]/2, f[0]+f[2]/6, f[1]-f[3]/2, screenWidth_, screenHeight_).getVertexBufferData();
    appendToVBA(tempVBA);

    std::vector<double> l_f{f[0]-f[2]/2, f[1], f[2]/2, f[3]/2, f[4], f[5]};
    makeFace(l_f, 2*generation, screenWidth_, screenHeight_);


    std::vector<double> r_f{f[0]+f[2]/2, f[1], f[2]/2, f[3]/2, f[4], f[5]};
    makeFace(r_f, 2*generation+1, screenWidth_, screenHeight_);
}