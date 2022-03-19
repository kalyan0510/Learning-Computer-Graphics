//
// Created by ax on 3/6/21.
//

#ifndef INC_123456789_catmull_rom_BCURVE_H
#define INC_123456789_catmull_rom_BCURVE_H

#include <vector>

#include <glm/glm.hpp>

#include "shape/Shape.h"


class BCurve : public Shape
{
public:
    BCurve() = default;

    BCurve(std::vector<glm::vec2> points, double screenWidth_, double screenHeight_);

    ~BCurve() override = default;

protected:
    /// Bresenham line-drawing algorithm for line (x0, y0) -> (x1, y1)
    /// given that its slope |m| <= 1.0 and that (x0, y0) is start position
    /// \param x0        screen space coordinate
    /// \param y0        screen space coordinate
    /// \param x1        screen space coordinate
    /// \param y1        screen space coordinate
    /// \param transpose whether to transpose setPixel coordinates
    void curve(std::vector<glm::vec2> points);
};



#endif  // INC_123456789_catmull_rom_BCURVE_H
