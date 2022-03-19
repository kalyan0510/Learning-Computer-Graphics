//
// Created by xihan1 on 8/30/21.
//

#include "shape/Circle.h"


Circle::Circle(double x0_, double y0_, double x1_, double y1_, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    int x0 = static_cast<int>(x0_);
    int y0 = static_cast<int>(y0_);
    int x1 = static_cast<int>(x1_);
    int y1 = static_cast<int>(y1_);

    midpointAlgorithm(x0, y0, x1, y1);
}


void Circle::midpointAlgorithm(int x0, int y0, int x1, int y1)
{
    // TODO
    // calculating euclidean distance b/w given points and saving r to an integer variable
    int r=round(sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0)));
    // setting starting point to (0,r)
    int x=0,y=r;
    // initializing decision variable
    // the 5/4 (in formula 5/4-r) can be safely rounded to 1 because the possible radius values which are around 1.25
    // are 1.0 and 1.414 = (sqrt(2) when a diagonal pixel is selected) and so, rounding 1.25 will 
    // not have an affect 
    int p = 1 -r;
    //plotting the initial point
    circlePlotPoints(x0, y0, {x, y});
    while(x<y) {
        x++;
        if (p<=0) {
            p+=2*x+1;
        } else {
            y--;
            p+=2*(x-y)+1;
        }
        circlePlotPoints(x0, y0, {x, y});
    }
}


void Circle::circlePlotPoints(int xc, int yc, const glm::ivec2 & pt)
{
    // TODO
    // initialize 4 quadradant signs
    std::vector<glm::ivec2> quadrants = {{1,1},{1,-1},{-1,1},{-1,-1}};
    for(glm::ivec2 quad: quadrants) {
        // flip the pt to corresponding quadradant
        glm::ivec2 point=(pt*quad);
        // draw pixel
        appendToPath(xc+point[0], yc+point[1]);
        // draw pixel on other side of circle
        appendToPath(xc+point[1], yc+point[0]);
    }

}
