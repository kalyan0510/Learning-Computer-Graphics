#include <iostream>
using namespace std;
#include "brent/brent.cpp"
#include <math.h>
#include <functional>
#include <glm/glm.hpp>
#define PI 3.14159265
#define TO_PI PI/180.0

class CollisionDetection {
private:
    
    // static 
    static double ellpiseX ( double cx, double cy, double a, double b, double alpha, double t) {
          double value;
          value = cx + a*cos(alpha)*cos(t) - b*sin(alpha)*sin(t);
          return value;
    }
    static double ellpiseY ( double cx, double cy, double a, double b, double alpha, double t) {
          double value;
          value = cy + a*sin(alpha)*cos(t) + b*cos(alpha)*sin(t);
          return value;
    }

    static double warpedEllpiseX ( double cx1, double cy1, double a1, double b1, double alpha1, double cx2, double cy2, double a2, double b2, double alpha2, double t) {
          double value;
          value = cos(t)*((a2*cos(alpha1)*cos(alpha2))/a1 + (a2*sin(alpha1)*sin(alpha2))/a1) - sin(t)*((b2*cos(alpha1)*sin(alpha2))/a1 - (b2*cos(alpha2)*sin(alpha1))/a1) - (cx1*cos(alpha1))/a1 + (cx2*cos(alpha1))/a1 - (cy1*sin(alpha1))/a1 + (cy2*sin(alpha1))/a1;
          return value;
    }
    static double warpedEllpiseY ( double cx1, double cy1, double a1, double b1, double alpha1, double cx2, double cy2, double a2, double b2, double alpha2, double t) {
          double value;
          value = cos(t)*((a2*cos(alpha1)*sin(alpha2))/b1 - (a2*cos(alpha2)*sin(alpha1))/b1) + sin(t)*((b2*cos(alpha1)*cos(alpha2))/b1 + (b2*sin(alpha1)*sin(alpha2))/b1) - (cy1*cos(alpha1))/b1 + (cy2*cos(alpha1))/b1 + (cx1*sin(alpha1))/b1 - (cx2*sin(alpha1))/b1;
          return value;
    }
    static double dist(double cx1, double cy1, double a1, double b1, double alpha1, double cx2, double cy2, double a2, double b2, double alpha2, double t) {
            double x = warpedEllpiseX(cx1, cy1, a1, b1, alpha1, cx2, cy2, a2, b2, alpha2, t);
            double y = warpedEllpiseY(cx1, cy1, a1, b1, alpha1, cx2, cy2, a2, b2, alpha2, t);
            return x*x+y*y;
    }

    static bool is_in_ellipse(double cx1, double cy1, double a1, double b1, double alpha1, double px, double py) {
        double x = px - cx1, y = py - cy1, sina = sin(alpha1), cosa = cos(alpha1);
        return (pow((x*cosa+y*sina)/a1, 2)+pow((y*cosa-x*sina)/b1, 2)-1) < 0;

    }

    // static auto make_dist_func(double cx1, double cy1, double a1, double b1, double alpha1, double cx2, double cy2, double a2, double b2, double alpha2)
    // {
    //     return [cx1, cy1, a1, b1, alpha1, cx2, cy2, a2, b2, alpha2] (auto x) {
    //         dist(cx1, cy1, a1, b1, alpha1, cx2, cy2, a2, b2, alpha2, x);
    //     };
    // }
    template <class Lambda>
    class FunctionWithState : public brent::func_base, public Lambda {
      public:
         FunctionWithState(const Lambda & lambda): Lambda(lambda) {}
         double operator()(double x) override 
         { return Lambda::operator()(x); }
    };

    template<class Lambda>
    static auto function_builder_base (Lambda lambda)
    {
        return FunctionWithState<decltype(lambda)>(lambda);
    }

    static auto function_builder(double cx1, double cy1, double a1, double b1, double alpha1, double cx2, double cy2, double a2, double b2, double alpha2)
    {
        return function_builder_base([cx1, cy1, a1, b1, alpha1, cx2, cy2, a2, b2, alpha2](double t) {
            return dist(cx1, cy1, a1, b1, alpha1, cx2, cy2, a2, b2, alpha2, t);
        });
    }

    static double coll_dist(double cx1, double cy1, double p1x, double p1y, double p2x, double p2y) {
        return sqrt(pow(cx1-p1x, 2)+pow(cy1-p1y, 2)) - sqrt(pow(cx1-p2x, 2)+pow(cy1-p2y, 2));
    }

public:
    //static function I
    static std::vector<glm::vec2> detect_ellipse_collision(std::vector<std::vector<double>>  ellipses){
          double t = 0.001;

          double a = 0.0;
          double b = 2*3.141;
          double z;
          auto f = function_builder(ellipses[0][0],ellipses[0][1],ellipses[0][2],ellipses[0][3],ellipses[0][4]*TO_PI,
            ellipses[1][0],ellipses[1][1],ellipses[1][2],ellipses[1][3],ellipses[1][4]*TO_PI);
           // double f ( double x ) = make_dist_func();
        // brent::func_wrapper foo(f);
        brent::local_min( a, b, t, f,  z);
        double px = ellpiseX(ellipses[1][0],ellipses[1][1],ellipses[1][2],ellipses[1][3],ellipses[1][4]*TO_PI, z);
        double py = ellpiseY(ellipses[1][0],ellipses[1][1],ellipses[1][2],ellipses[1][3],ellipses[1][4]*TO_PI, z);
        // printf("\nKALYAN - %f %f (%f, %f)\n",z, f(z), px, py);
        vector<glm::vec2> points;
        points.emplace_back(px,py);
        if(is_in_ellipse(ellipses[0][0],ellipses[0][1],ellipses[0][2],ellipses[0][3],ellipses[0][4]*TO_PI, px, py)) {
            points.emplace_back(
                ellipses[1][3]*cos(ellipses[1][4]*TO_PI)*cos(z) -  ellipses[1][2]*sin(ellipses[1][4]*TO_PI)*sin(z),
                ellipses[1][2]*cos(ellipses[1][4]*TO_PI)*sin(z) + ellipses[1][3]*sin(ellipses[1][4]*TO_PI)*cos(z)
            );
        }

        //- a2*cos(alpha2)*sin(t) - b2*sin(alpha2)*cos(t)
        // b2*cos(alpha2)*cos(t) - a2*sin(alpha2)*sin(t)
        
        // for (int i =0; i< 268 ; i ++ ){
        //     px = ellpiseX(800, 450, 100, 50, 35*3.14/180, i/100.0);
        //     py = ellpiseY(800, 450, 100, 50, 35*3.14/180, i/100.0);
        //     points.emplace_back(px,py);
        // }

        return points;
    }
    
};