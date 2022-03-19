//
// Created by ax on 3/6/21.
//

#include <algorithm>
#include <cmath>
#include <iostream>

#include "shape/LineSegment.h"

int sign(int x) {
    return (x > 0) - (x < 0);
}

LineSegment::LineSegment(double x0_, double y0_, double x1_, double y1_, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    int x0 = static_cast<int>(x0_);
    int y0 = static_cast<int>(y0_);
    int x1 = static_cast<int>(x1_);
    int y1 = static_cast<int>(y1_);
    // checkout if the line is going to be steep
    bool is_steep = abs(y1-y0) > abs(x1-x0);
    if (!is_steep) {
        // line is not steep, then dont transpose
        if (x0>x1) {
            // swap order of points if x0 > x1
            bresenhamAlgorithm(x1, y1, x0, y0, false);    
        } else {
            bresenhamAlgorithm(x0, y0, x1, y1, false);
        }
    } else {
        // line is steep, then use transpose
        if (y0>y1) {
            // swap order of points if y0 > y1
            bresenhamAlgorithm(x1, y1, x0, y0, true);    
        } else {
            bresenhamAlgorithm(x0, y0, x1, y1, true);
        }
    }
}


void LineSegment::bresenhamAlgorithm(int x0, int y0, int x1, int y1, bool b)
{   
    // if the line is steep, we transpose x and y's. This lets us deal with it as it's |m|<1
    if (b) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // create x & y variables that are used as temporary holders for current pixel
    // although we can use x0 and y0 for same purpose, this makes code readable
    int x=x0, y=y0;
    // calculate delta x and delta y
    int del_x=x1-x0, del_y=y1-y0;
    // decide whether y increases or decreases as x increases
    int y_increment = sign(del_y);
    // correct for del_y as negative is now considered in increments
    del_y = y_increment*del_y;
    // precalculate and store these line constants. So, they can be simple used later 
    //(this might not be really a percievable optimization as lines on hardware displays
    // do not span really long)
    int twice_del_y=2*del_y, twice_del_yx_diff=2*(del_y-del_x);
    // initial calculation for p (the decision parameter)
    int p = 2*del_y-del_x;

    //Helper code comment to visualize coordinates while debuging
    //std::cout << x0 <<" "<<y0 <<" " <<x1  <<" "<<y1 <<" " << y_increment<<std::endl;

    do {
        // draw pixel
        if (b) {appendToPath(y, x);}
        else {appendToPath(x,y);} 
        // increment x
        x++;
        // decide if y should be incremented, and also update decision param 
        if (p<0) {
            p+=twice_del_y;
        } else {
            p+=twice_del_yx_diff;
            y+=y_increment;
        }
        // repeat until x1
    } while(x<x1);
}
