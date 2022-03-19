#ifndef city_modeling_TETRA_H
#define city_modeling_TETRA_H

#include "shape/Shape.h"
// struct KeyFuncs
// {
//     size_t operator()(const glm::vec3& k)const
//     {
//         return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
//     }

//     bool operator()(const glm::vec3& a, const glm::vec3& b)const
//     {
//             return a.x == b.x && a.y == b.y && a.z == b.z;
//     }
// };

class Tetrahedron : public Shape
{
public:
    Tetrahedron(int mode)
    {
        for(int i=0;i<NUM_VERTICES;i++){
            vertex.push_back(glm::make_vec3(vertexData[i]));
            // normal.push_back(glm::make_vec3(vertexData[i]));
        }
        for(int i=0;i<NUM_VERTICES/3;i++){
                    glm::vec3 a = glm::make_vec3(vertexData[3*i]);
                    glm::vec3 b = glm::make_vec3(vertexData[3*i+1]);
                    glm::vec3 c = glm::make_vec3(vertexData[3*i+2]);
                    glm::vec3 mid = (a+b+c)/3.0f;
                    glm::vec3 anormal = glm::cross(b-a,c-b);
                    float x = glm::dot(mid, anormal)<0 ?-1.0f:1.0f;
                    // std::cout<<x<<std::endl;
                    normal.push_back(x*glm::cross(b-a,c-b));
                    normal.push_back(x*glm::cross(c-b,a-c));
                    normal.push_back(x*glm::cross(a-c,b-a));
        }
        // for(int i=0;i<normal.size();i++){
        //     std::cout<<"      "<<normal[i][0] <<" "<< normal[i][1] <<" " <<normal[i][2]<<std::endl;            
        // }
        avged_normal = normal;
        switch(mode) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
                // std::cout<<"DOing Stuff"<<std::endl;
                // std::cout<<vertex.size()<<" "<<normal.size()<<std::endl;

                std::unordered_map<glm::vec3, std::vector<glm::vec3>, KeyFuncs, KeyFuncs> normalmap;
                for(int i=0;i<normal.size();i++){
                    normalmap[vertex[i]].push_back(normal[i]);
                }
                for(int i=0;i<normal.size();i++){
                    glm::vec3 temp{0,0,0};
                    for(int j=0;j<normalmap[vertex[i]].size();j++){
                        temp+=normalmap[vertex[i]][j];
                    }
                    avged_normal[i] = temp;//*float(1.0/(normalmap[vertex[i]].size()*1.0));
                    // std::cout<< normalmap[vertex[i]].size() <<"      "<<normal[i][0] <<" "<< normal[i][1] <<" " <<normal[i][2]<<std::endl;
                }  
            break;
        }

    }

    [[nodiscard]] GLint getNumVertices() const override
    {
        return vertex.size();
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
        return mode==3?avged_normal:normal;
    }

private:
    static constexpr int NUM_FACETS = 6 * 2;
    static constexpr int NUM_VERTICES = NUM_FACETS * 3;

    const GLfloat vertexData[NUM_VERTICES][3] =
            // { .0f, 1.f, 0.f,    // Left
            //   -1., 0.f, 1.f,
            //   -1., 0.f, -1.f,

            //   0.f, 1.f, 0.f,    // Right
            //   1.f, 0.f, 1.f,
            //   1.f, 0.f, 1.f,

            //   0.f, 1.f, 0.f,    // Near 
            //  -1.f, 0.f, 1.f,
            //   1.f, 0.f, 1.f,

            //   0.f, 1.f, 0.f,    // Far 
            //  -1.f, 0.f, -1.f,
            //   1.f, 0.f, -1.f };
    {
        .57f,-.57f,-.57f,
        -.57f,.57f,-.57f,
        -.57f,-.57f,.57f,

        .57f,.57f,.57f,
        -.57f,.57f,-.57f,
        -.57f,-.57f,.57f,

        .57f,.57f,.57f,
        .57f,-.57f,-.57f,
        -.57f,-.57f,.57f,

        .57f,.57f,.57f,
        .57f,-.57f,-.57f,
        -.57f,.57f,-.57f

    };

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec3> avged_normal;
    
};


#endif // city_modeling_TETRA_H
