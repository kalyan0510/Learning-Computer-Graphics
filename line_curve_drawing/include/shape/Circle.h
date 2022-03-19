//
// Created by xihan1 on 8/30/21.
//

#ifndef INC_123456789_line_curve_draw_CIRCLE_H
#define INC_123456789_line_curve_draw_CIRCLE_H

#include "shape/Shape.h"


class Circle : public Shape
{
public:
    Circle() = default;

    Circle(double x0_, double y0_, double x1_, double y1_, double screenWidth_, double screenHeight_);

    ~Circle() override = default;

protected:
    /// Midpoint circle-drawing algorithm for circle centered at (x0, y0)
    /// and its radius is norm of vector (x1 - x0, y1 - y0)
    /// \param x0        screen space coordinate
    /// \param y0        screen space coordinate
    /// \param x1        screen space coordinate
    /// \param y1        screen space coordinate
    void midpointAlgorithm(int x0, int y0, int x1, int y1);

    /// Plot the point pt in each circle quadrant
    /// \param xc x coordinate of center of circle
    /// \param yc y coordinate of center of circle
    /// \param pt
    void circlePlotPoints(int xc, int yc, const glm::ivec2 & pt);
};



#endif //INC_123456789_line_curve_draw_CIRCLE_H
