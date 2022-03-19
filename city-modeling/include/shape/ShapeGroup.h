#ifndef city_modeling_SHAPEGRP_H
#define city_modeling_SHAPEGRP_H

#include "shape/Shape.h"

class ShapeGroup : public Shape
{
public:
    ShapeGroup(std::vector<Shape*> shapes)
    {
        total_vert =0;

        for(int i=0; i<shapes.size();i++) {
            std::cout<<"Inside"<<std::endl;
            std::cout<<shapes[i]->getNumVertices()<<std::endl;
            std::cout<<shapes[i]->getVertexVec().size()<<std::endl;

            vertex.insert(vertex.end(), shapes[i]->getVertexVec().begin(), shapes[i]->getVertexVec().end());
            normal.insert(normal.end(), shapes[i]->getNormalVec(0).begin(), shapes[i]->getNormalVec(0).end());
            avged_normal.insert(avged_normal.end(), shapes[i]->getNormalVec(3).begin(), shapes[i]->getNormalVec(3).end());
            std::cout<<"Inside 1 Done"<<std::endl;

        }
        total_vert = vertex.size();
    }

    [[nodiscard]] GLint getNumVertices() const override
    {
        return total_vert;
    }

    [[nodiscard]] const glm::vec3 * getVertexData() const override
    {
        return vertex.data();
    }

    [[nodiscard]] const glm::vec3 * getNormalData(int mode) const override
    {
        return mode==3?avged_normal.data():normal.data();
    }

    [[nodiscard]] const std::vector<glm::vec3> &getVertexVec() const override
    {
        return vertex;
    }

    [[nodiscard]] const std::vector<glm::vec3> &getNormalVec(int mode) const override
    {
        // return normal;
        return mode==3?avged_normal:normal;

    }

private:
    int total_vert=0;
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec3> avged_normal;
};


#endif // city_modeling_SHAPEGRP_H
