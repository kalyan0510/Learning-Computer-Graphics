//
// Created by ax on 3/6/21.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "shape/CCurve.h"


CCurve::CCurve(std::vector<glm::vec2> points, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    //curve( x0,  y0,  x1,  y1,  x2,  y2,  x3,  y3);
    curve(points);
}


//void CCurve::curve(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
void CCurve::curve(std::vector<glm::vec2> points)
{   
    if(points.size()<2){
        throw std::invalid_argument( "received less than 2 points" );
    }
    std::vector<glm::vec2> appoints(points.begin(), points.end());
    appoints.insert(appoints.begin(), 2.0f*points[0] - points[1]);
    appoints.push_back(2.0f*points[points.size()-1] - points[points.size()-2]);

    std::vector<glm::vec2> newp;
    for(int i=1;i<points.size();i++){
        newp.push_back(appoints[i-1]);
        newp.push_back(appoints[i]);
        newp.push_back(appoints[i+1]);
        newp.push_back(appoints[i+2]);
    }

    // for(int i=4; i<points.size();i++){
    //     int sz = newp.size();
    //     glm::vec2 c1=newp[sz-3], c2=newp[sz-2], c3=newp[sz-1];
    //     glm::vec2 d0 = c3, d1=c3-((sz*1.0f)/4.0f)*(c2-c3), d2;
    //     d2 = 2.0f*d1 - c3 + ((sz*1.0f*(sz-1))/12.0f)*( c1 - 2.0f*c2 + 2.0f*d1 );
    //     newp.push_back(d0);
    //     newp.push_back(d1);
    //     newp.push_back(d2);
    //     newp.push_back(points[i]);   
    // }


    //  for(int i=4; i<points.size();i++){
    //     int sz = newp.size();
    //     glm::vec2 c1=newp[sz-3], c2=newp[sz-2], c3=newp[sz-1];
    //     glm::vec2 d0 = c3, d1=2.0f*c3-c2, d2 = (4.0f*c3-4.0f*c2+c1)*.25f;
    //     newp.push_back(d0);
    //     newp.push_back(d1);
    //     newp.push_back(d2);
    //     newp.push_back(points[i]);
    // }

    // put them in the buffer
    for(int i=0;i<newp.size();i++){
        appendToPath(newp[i][0], newp[i][1]);   
    }
    // appendToPath(x0, y0);
    // appendToPath(x1, y1);
    // appendToPath(x2, y2);
    // appendToPath(x3, y3);
    // appendToPath(x3, y3+300);
    // appendToPath(x2, y2+300);
    // appendToPath(x1, y1+300);
    // appendToPath(x0, y0+300);
    // appendToPath(x3, y3-300);
    // appendToPath(x2, y2-300);
    // appendToPath(x1, y1-300);
    // appendToPath(x0, y0-300);
}


    // for(int i=4; i<points.size();i++){
    //     int sz = newp.size();
    //     glm::vec2 c1=newp[sz-3], c2=newp[sz-2], c3=newp[sz-1];
    //     glm::vec2 d0 = c3, d1=2.0f*c3-c2, d2 = 4.0f*c3-4.0f*c2+c1;
    //     newp.push_back(d0);
    //     newp.push_back(d1);
    //     newp.push_back(d2);
    //     newp.push_back(points[i]);   
    // }