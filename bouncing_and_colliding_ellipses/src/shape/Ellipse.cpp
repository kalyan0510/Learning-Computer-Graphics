//
// Created by xihan1 on 8/30/21.
//

#include <iostream>

#include "shape/Ellipse.h"
#include "shape/LineSegment.h"
#include <algorithm>
#include <math.h>
#define PI 3.14159265



Ellipse::Ellipse(double x0_, double y0_, double x1_, double y1_, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    int x0 = static_cast<int>(x0_);
    int y0 = static_cast<int>(y0_);
    int x1 = static_cast<int>(x1_);
    int y1 = static_cast<int>(y1_);

    int rx = std::abs(x1 - x0);
    int ry = std::abs(y1 - y0);
    midpointAlgorithm(x0, y0, rx, ry, 0);
}


Ellipse::Ellipse(double x0_, double y0_, double a_, double b_, double alpha_, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    int x0 = static_cast<int>(x0_);
    int y0 = static_cast<int>(y0_);
    int a = static_cast<int>(a_);
    int b = static_cast<int>(b_);

    // int a_ = std::abs(x1 - x0);
    // int b_ = std::abs(y1 - y0);
    // midpointAlgorithm(x0, y0, a_, b_, 0);
    midpointAlgorithm(x0, y0, a, b, alpha_);
}

double ellpiseX ( double cx, double cy, double a, double b, double alpha, double t) {
      double value;
      value = cx + a*cos(alpha)*cos(t) - b*sin(alpha)*sin(t);
      return value;
}
double ellpiseY ( double cx, double cy, double a, double b, double alpha, double t) {
      double value;
      value = cy + a*sin(alpha)*cos(t) + b*cos(alpha)*sin(t);
      return value;
}


Ellipse::Ellipse(double x0, double y0, double a, double b, double alpha_, bool shaded, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    // int x0 = static_cast<int>(x0_);
    // int y0 = static_cast<int>(y0_);
    // int a = static_cast<int>(a_);
    // int b = static_cast<int>(b_);

    // printf("x0=%f y0=%f a=%f b=%f alpha=%f\n", x0, y0, a, b, alpha_);
    // for (int t=0;t<314*2;t++){
    //     // appendToPath(ellpiseX(x0, y0, a , b, alpha_, t/100.0), ellpiseY(x0, y0, a , b, alpha_, t/100.0));
    // }
    // int a_ = std::abs(x1 - x0);
    // int b_ = std::abs(y1 - y0);
    // midpointAlgorithm(x0, y0, a_, b_, 0);
    if(!shaded) {
        midpointAlgorithm(x0, y0, a, b, alpha_);
        return;
    }
    midpointAlgorithm(x0, y0, a, b, alpha_);    
    /*
    LineSegment lineSegment;
    std::vector<glm::vec2> newPath;
    std::vector<glm::vec2> tempVBA;
    // iterate through each consequitive vertex pair 
    for(glm::vec2 point: path) {
        lineSegment = LineSegment(x0,y0,(x0*4+point[0])/5, (y0*4+point[1])/5,
                                 screenWidth_, screenHeight_);
        tempVBA = lineSegment.path;
        newPath.insert(newPath.end(), tempVBA.begin(), tempVBA.end());
    }
    path.insert(path.begin(), newPath.begin(), newPath.end());
    */
    std::vector<glm::vec2> newPath;

    std::sort(path.begin(), path.end(),  [this](glm::vec2 const &lhs, glm::vec2 const& rhs)
          {
              return lhs[0] > rhs[0];
          }
    );
    for(int i=0;i<path.size(); i++) {
        if(path[i][0]==path[i+1][0]) {
            int x = std::min(path[i][1],path[i+1][1]);
            int y = std::max(path[i+1][1],path[i][1]);
            for(int j=x; j<=y;j++) {
                newPath.push_back({path[i][0], j});
            }
        }
    }
    path=newPath;
}


inline int Ellipse::round(double a)
{
    // TODO
    // int casting scrapes of any decimal values. so adding 0.5 and casting to int
    // will result in rounding of decimals to nearest ints
    return int(a+0.5);
}


void Ellipse::midpointAlgorithm(int xc, int yc, int rx, int ry, double alpha_)
{
    // TODO
    // setting the init point
    int x=0,y=ry;
    // caching some calculations that are needed for the algorithm
    long long rx_squared = rx*rx, ry_squared=ry*ry;
    // init decision params
    long long px = 0, py=2*rx_squared*y;
    // plot first point
    ellipsePlotPoints(xc, yc, x, y, alpha_);
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
        ellipsePlotPoints(xc, yc, x, y, alpha_);
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
        ellipsePlotPoints(xc, yc, x, y, alpha_);
    }
    // std::cout<<"\n";
    
}

//x = cx1 + a1*cos(alpha1)*cos(t) + b1*sin(alpha1)*sin(t)
//y = cy1 - a1*sin(alpha1)*cos(t) + b1*cos(alpha1)*sin(t)

void Ellipse::ellipsePlotPoints(int xc, int yc, int x, int y, double alpha_)
{
    // TODO
    double cos_val, sin_val;
    glm::ivec2 pt = {x,y};
    // initialize 4 quadradant signs
    std::vector<glm::ivec2> quadrants = {{1,1},{1,-1},{-1,1},{-1,-1}};
    for(glm::ivec2 quad: quadrants) {
        // flip the pt to corresponding quadradant
        glm::ivec2 point=(pt*quad);
        // draw pixel
        cos_val = cos(alpha_*PI/180.0);
        sin_val = sin(alpha_*PI/180.0);
        appendToPath(xc+cos_val*point[0] - point[1]*sin_val, yc+point[0]*sin_val + cos_val*point[1]);
    }
}
