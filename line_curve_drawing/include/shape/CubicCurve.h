//
// Created by xihan1 on 8/30/21.
//

#ifndef INC_123456789_line_curve_draw_CUBICCURVE_H
#define INC_123456789_line_curve_draw_CUBICCURVE_H

#include "shape/Shape.h"


class CubicCurve : public Shape
{
public:
    CubicCurve() = default;

    /// Cubic curve y = ax^3 + bx^2 + cx + d
    /// \param a
    /// \param b
    /// \param c
    /// \param d
    CubicCurve(double a_, double b_, double c_, double d_, double screenWidth_, double screenHeight_);

    virtual ~CubicCurve() = default;

protected:
    void midpointAlgorithm(double a, double b, double c, double d);
    void drawFlatCurves(double a, double b, double c, double d, int start, int end);
    void drawSteepCurves(double a, double b, double c, double d, int start, int end);
};


#endif //INC_123456789_line_curve_draw_CUBICCURVE_H
