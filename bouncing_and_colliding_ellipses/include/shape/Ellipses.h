//
// Created by xihan1 on 8/30/21.
//

#ifndef INC_123456789_boucing_and_colliding_ellipses_ELLIPSES_H
#define INC_123456789_boucing_and_colliding_ellipses_ELLIPSES_H

#include "shape/CustomObject.h"


class Ellipses : public CustomObject
{
public:
    Ellipses() = default;

    Ellipses(std::vector<std::vector<double>> ellipses, bool shaded, double screenWidth_, double screenHeight_);

    ~Ellipses() override = default;
};


#endif  // INC_123456789_boucing_and_colliding_ellipses_ELLIPSES_H
