//
// Created by xihan1 on 8/30/21.
//

#include <iostream>

#include "shape/Ellipse.h"


Ellipse::Ellipse(double x0_, double y0_, double x1_, double y1_, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    int x0 = static_cast<int>(x0_);
    int y0 = static_cast<int>(y0_);
    int x1 = static_cast<int>(x1_);
    int y1 = static_cast<int>(y1_);

    int rx = std::abs(x1 - x0);
    int ry = std::abs(y1 - y0);
    midpointAlgorithm(x0, y0, rx, ry);
}


inline int Ellipse::round(double a)
{
    // TODO
    // int casting scrapes of any decimal values. so adding 0.5 and casting to int
    // will result in rounding of decimals to nearest ints
    return int(a+0.5);
}


void Ellipse::midpointAlgorithm(int xc, int yc, int rx, int ry)
{
    // TODO
    // setting the init point
    int x=0,y=ry;
    // caching some calculations that are needed for the algorithm
    long long rx_squared = rx*rx, ry_squared=ry*ry;
    // init decision params
    long long px = 0, py=2*rx_squared*y;
    // plot first point
    ellipsePlotPoints(xc, yc, x, y);
    //=========== region 1 =================//
    // this region is not steep w.r.t y, so we increment x and caluclate for y
    long long p = ry_squared - rx_squared*ry + round(0.25*rx_squared);
    while(px<py) {
        // increment x
        x++;
        px+=2*ry_squared;
        if (p<0) {
            p+=ry_squared+px;
        } else {
            y--;
            py-=2*rx_squared;
            p+=ry_squared+px-py;
        }
        ellipsePlotPoints(xc, yc, x, y);
    }

    //=========== region 2 =================//
    // this region is steep w.r.t y, so we decrement y and caluclate for x
    p = round(ry_squared*(x+0.5)*(x+0.5)+rx_squared*(y-1)*(y-1)-rx_squared*ry_squared);
    while(y>0) {
        y--;
        py-=2*rx_squared;
        // std::cout<<p<<"";
        if (p>0) {
            p+=rx_squared-py;
            //std::cout << "1\n";
        } else {
            x++;
            px+=2*ry_squared;
            p+=rx_squared-py+px;
            //std::cout << "0\n";
        }
        ellipsePlotPoints(xc, yc, x, y);
    }
    // std::cout<<"\n";
    
}


void Ellipse::ellipsePlotPoints(int xc, int yc, int x, int y)
{
    // TODO
    glm::ivec2 pt = {x,y};
    // initialize 4 quadradant signs
    std::vector<glm::ivec2> quadrants = {{1,1},{1,-1},{-1,1},{-1,-1}};
    for(glm::ivec2 quad: quadrants) {
        // flip the pt to corresponding quadradant
        glm::ivec2 point=(pt*quad);
        // draw pixel
        appendToPath(xc+point[0], yc+point[1]);
    }
}
