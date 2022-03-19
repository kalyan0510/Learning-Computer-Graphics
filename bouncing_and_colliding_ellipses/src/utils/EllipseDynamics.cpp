#include <iostream>
using namespace std;
#include "CollisionDetection.cpp"
#include <math.h>
#include <functional>
#include <glm/glm.hpp>
#include "shape/LineSegment.h"
#define PI 3.14159265
#define TO_PI PI/180.0

class EllipseDynamics{
private:
    
    // static 
    // static double ellpiseX ( double cx, double cy, double a, double b, double alpha, double t) {
    //       double value;
    //       value = cx + a*cos(alpha)*cos(t) - b*sin(alpha)*sin(t);
    //       return value;
    // }
    static std::vector<double> bounding_box(std::vector<double> e) {
        double ux = e[2] * cos(e[4]*TO_PI), uy = e[2] * sin(e[4]*TO_PI);
        double vx = e[3] * cos(e[4]*TO_PI+PI/2), vy = e[3] * sin(e[4]*TO_PI+PI/2);
        return {sqrt(ux*ux + vx*vx), sqrt(uy*uy + vy*vy)}; 
    }

    static double toCompleteAngle(double radians) {
        // printf("\n\n%f\n\n",radians);
        return (radians > 0 ? radians : (2*PI + radians)) * 180.0/PI;
    }

    static std::vector<std::vector<double>> detect_e2e_collision(std::vector<std::vector<double>> es, Context & context, bool debugOn) {

        for(int i=0; i<es.size(); i++) {
            for(int j=i+1; j< es.size(); j++) {
                // if(i==j) continue;
                std::vector<glm::vec2> coll_detection = CollisionDetection::detect_ellipse_collision({ {es[i][0], es[i][1], es[i][2], es[i][3], es[i][4]},{es[j][0], es[j][1], es[j][2], es[j][3], es[j][4]}});
                if(coll_detection.size()>1) {
                    // printf("COLLISION DETECTED");
                    double v1x = es[i][5], v1y = es[i][6],v2x = es[j][5], v2y = es[j][6];
                    double nx = coll_detection[1][0],ny=coll_detection[1][1];
                    double val = (nx*(v1x-v2x) + ny*(v1y-v2y))/(nx*nx+ny*ny);
                    double dx = val*nx, dy = val*ny;
                    double newv1x = v1x - dx , newv1y= v1y - dy;
                    es[i][5] = newv1x;
                    es[i][6] = newv1y;//toCompleteAngle(atan2(newv1y, newv1x));
                    // printf(" %f %f %f %f %f %f \n",es[i][0], es[i][1], v1x, v1y, newv1x, newv1y );
                    newv1x = v2x + dx , newv1y= v2y + dy;
                    es[j][5] = newv1x;//sqrt(newv1x*newv1x + newv1y*newv1y);
                    es[j][6] = newv1y;//toCompleteAngle(atan2(newv1y, newv1x));
                    // printf(" %f %f %f %f %f %f \n",es[j][0], es[j][1], v2x, v2y, newv1x, newv1y);
                    // printf("DEALT\n");
                    es[i][0] = es[i][0] - v1x;
                    es[i][1] = es[i][1] - v1y;
                    es[j][0] = es[j][0] - v2x;
                    es[j][1] = es[j][1] - v2y;
                    es[i][7]++;
                    es[j][7]++;
 
                }
                if(debugOn){
                    auto tmpvba = LineSegment(es[i][0], es[i][1],coll_detection[0][0],coll_detection[0][1], context.SCR_WIDTH, context.SCR_HEIGHT).getVertexBufferData();
                    context.vertexBufferData.insert(context.vertexBufferData.end(), tmpvba.begin(), tmpvba.end());
                }
                    // printf("%f %f %f %f %f %f %f %f %f\n", es[i][0], es[i][1], es[i][2], es[i][3], es[i][4], es[i][5], es[i][6], es[i][7], es[i][8]);
                    // printf("%f %f %f %f %f %f %f %f %f\n", es[j][0], es[j][1], es[j][2], es[j][3], es[j][4], es[j][5], es[j][6], es[j][7], es[j][8]);
                //////

                //////
            }
        }
        // printf("LOOPED\n");
        // for(auto e: es) {
        //     printf("%f %f %f %f %f %f %f\n", e[0], e[1], e[2], e[3], e[4], e[5], e[6]);
        // }
        return es;
    }

    static std::vector<std::vector<double>> detect_e2b_collision(std::vector<std::vector<double>> es, double screenWidth, double screenHeight) {
        for(int i=0;i<es.size(); i++) {
            std::vector<double> bbox = bounding_box(es[i]);
            if((es[i][0]+bbox[0])>screenWidth) {
                // right
                es[i][5]=-es[i][5];
                es[i][0]=screenWidth-bbox[0];
            }
            if((es[i][0]-bbox[0])<0) {
                // left
                es[i][5]=-es[i][5];
                es[i][0]=bbox[0];
            }
            if((es[i][1]+bbox[1])>screenHeight) {
                // top
                es[i][6]=-es[i][6];
                es[i][1]=screenHeight-bbox[1];

            }
            if((es[i][1]-bbox[1])<0) {
                // left
                es[i][6]=-es[i][6];   
                es[i][1]=bbox[1];             
            }
        }
        return es;
    }

public:
    //static function I
    static std::vector<std::vector<double>> nextStep(std::vector<std::vector<double>>  ellipsesState, double screenWidth, double screenHeight, Context & context, bool debugOn){
        std::vector<std::vector<double>>  newEllipsesState;
        for(std::vector<double> e: ellipsesState) {
            // double vx = e[5]*cos(e[6]*TO_PI), vy = e[5]*sin(e[6]*TO_PI);
            // printf("%f %f\n ", vx, vy);
            // std::vector<double> bb = bounding_box(e);
            // printf("%f %f ", bb[0],bb[1]);
            // printf("%f \n", e[4]);
            newEllipsesState.push_back({e[0]+e[5], e[1]+e[6], e[2], e[3], e[4], e[5], e[6], e[7], e[8]});
        }
        newEllipsesState = detect_e2e_collision(newEllipsesState, context, debugOn);
        newEllipsesState = detect_e2b_collision(newEllipsesState, screenWidth, screenHeight);
        return newEllipsesState;
    }
    
};