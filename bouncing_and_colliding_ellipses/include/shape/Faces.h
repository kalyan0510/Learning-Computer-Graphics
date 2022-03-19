#ifndef INC_123456789_boucing_and_colliding_ellipses_FACES_H
#define INC_123456789_boucing_and_colliding_ellipses_FACES_H

#include "shape/CustomObject.h"


class Faces : public CustomObject
{
public:
    Faces() = default;

    Faces(std::vector<std::vector<double>> faces, double screenWidth_, double screenHeight_);

    ~Faces() override = default;

    void makeFace(std::vector<double> f, int generation, double screenWidth_, double screenHeight_ );
    
};


#endif  // INC_123456789_boucing_and_colliding_ellipses_FACES_H
