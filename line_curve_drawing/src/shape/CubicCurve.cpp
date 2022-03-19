//
// Created by xihan1 on 8/30/21.
//

#include "shape/CubicCurve.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

CubicCurve::CubicCurve(double a_, double b_, double c_, double d_, double screenWidth_, double screenHeight_) :
        Shape(screenWidth_, screenHeight_)
{
    printf("Displaying curve for equation (%f)*x^3 + (%f)*x^2 + (%f)*x + %f\n", a_,b_,c_,d_);
    midpointAlgorithm(a_, b_, c_, d_);
}

// method to evaluate a point's position relative to polynomial 
double f(double a,double b,double c,double d,double x, double y) {
    return a*x*x*x + b*x*x + c*x + d - y;
}

// derivative of cubic curve
double f_(double a,double b,double c,double d,double x) {
    return 3*a*x*x + 2*b*x + c;
}

// +1 of x>0 else -1
int sign(double x) {
    return (x > 0) - (x < 0);
}

// clips the value of x so that it lies between a and b
int clip(int a, int b, int x) {
    if (a<=x && x<=b) {
        return x;
    } else if(x<a) {
        return a;
    } else {
        return b;
    }
}

// for every third degree polynomial, there can exist two intervals where the derivative of 
//the polynomial has magnitude less than 1. This method returns bounding limits of two intervals
// {l1,l2}, {l3,l4}
std::vector<int> getIntervalPoints(double a, double b, double c, double d) {
    std::vector<int> ints;
    double sqrt_part_with_na=sqrt(max(b*b-3*a*c-3*a,0.0));
    double sqrt_part_with_pa=sqrt(max(b*b-3*a*c+3*a,0.0));
    //std::cout<<b*b-3*a*c-3*a<<" "<<b*b-3*a*c+3*a<<"\n";
    int l1 = round((-b-sqrt_part_with_pa)/(3*a));
    int l2 = round((-b-sqrt_part_with_na)/(3*a));
    int l3 = round((-b+sqrt_part_with_na)/(3*a));
    int l4 = round((-b+sqrt_part_with_pa)/(3*a));
    if(a>=0) {
        ints = {l1, l2, l3, l4};
    } else {
        ints = {l3, l4, l1, l2};
    }
    return ints;
}

// this method draws curves when their derivative is less than 1 and greated than -1
void CubicCurve::drawFlatCurves(double a, double b, double c, double d, int start, int end) {
    int w1 = int(screenWidth/2.0), h1=int(screenHeight/2.0);
    int x = start, y = round(f(a,b,c,d,start, 0));
    double p1=f(a,b,c,d,x, y+0.5), p2=f(a,b,c,d,x,y-0.5);
    appendToPath(x,y);
    while(x<=end) {
        x++;
        if(p1>0 && p2>0) {
            y++;
            //std::cout<<"0\n";
        } else if (p1<0 && p2<0){
            y--;
            //std::cout<<"1\n";
        } else {
            //std::cout<<"-\n";
        }
        //std::cout<<x<<"<>"<<f(a,b,c,d,x,0)<<"<>"<<y<<"\n";
        appendToPath(x+w1,y+h1);
        p1=f(a,b,c,d,x, y+0.5), p2=f(a,b,c,d,x,y-0.5);
    }
}


// method to draw steep monotonic curves
void CubicCurve::drawSteepCurves(double a, double b, double c, double d, int start, int end) {
    int w1 = int(screenWidth/2.0), h1=int(screenHeight/2.0);
    int y_increment = sign(f_(a,b,c,d,start));
    int x = start, y = round(f(a,b,c,d,start, 0));
    double p1=f(a,b,c,d,x+0.5, y+y_increment);
    // std::cout<<"y_increment : "<<y_increment<<"  p1 "<<p1<<"\n";
    appendToPath(x+w1,y+h1);
    int progress_show = 0;
    while(x<=end) {
        progress_show++;
        y+=y_increment;
        if(y_increment>0 && p1<0) {
            x++;
            // std::cout<<"1\n";
        }
        if(y_increment<0 && p1>0){
            x++;
        }
        // std::cout<<x<<"<>"<<f(a,b,c,d,x,0)<<"<>"<<y<<"\n";
        if(-w1<x && x<w1 && -h1<y && y<h1){
           appendToPath(x+w1,y+h1);
        } else {
            if(progress_show%1000==0){
                std::cout<<"Computing graph. Please Wait.....\n";
            }
        }
        p1=f(a,b,c,d,x+0.5, y);
    }
}

// main algorithm
void CubicCurve::midpointAlgorithm(double a, double b, double c, double d)
{
    // TODO: implement cubic curve
    std::vector<int> intervalPts = getIntervalPoints(a,b,c,d);

    //// fetch the intervals where derivative's magnitude is lessthan 1 
    int l1=intervalPts[0], l2=intervalPts[1], l3=intervalPts[2], l4=intervalPts[3];
    //std::cout<<"L1234: "<<intervalPts[0]<<"  "<<intervalPts[1] << "  " <<intervalPts[2] << "  " <<intervalPts[3]<< "  \n";
    int w1 = -int(screenWidth/2.0), w2 = int(screenWidth/2.0);
    int h1 = -int(screenHeight/2.0), h2 = int(screenHeight/2.0);

    // clip the intervals, so we dont care about them if they lie beyond the screen coverage
    l1 = clip(w1,w2, l1);
    l2 = clip(w1,w2, l2);
    l3 = clip(w1,w2, l3);
    l4 = clip(w1,w2, l4);
    // std::cout<<"L1234: "<<l1<<"  "<<l2<< "  " <<l3 << "  " <<l4<< "  \n";

    // intervals where derivative is huge 
    std::vector<glm::ivec2> steep_intervals = {{w1,max(w1,l1)}, {min(l2,w2), max(l3,w1)}, {min(l4,w2), w2}};
    
    // intervals where derivative is small (|m|<1)
    std::vector<glm::ivec2> flat_intervals = {{max(w1,l1),min(l2,w2)}, {max(l3,w1), min(l4,w2)}};
    
    // draw the curve between these intervals
    for(glm::ivec2 ve:flat_intervals) {
        // std::cout<<"flat region: "<<ve[0]<<" "<<ve[1]<<" & \n";
        drawFlatCurves(a,b,c,d,ve[0],ve[1]);
    }
    // std::cout<<"\n";

    // draw the curve between these intervals
    for(glm::ivec2 ve:steep_intervals) {
        // std::cout<<"steep region: "<<ve[0]<<" "<<ve[1]<<" & \n";
        if(ve[0]<ve[1]){
            drawSteepCurves(a,b,c,d,ve[0], ve[1]);
        }
    }
    // std::cout<<"\n";
}

