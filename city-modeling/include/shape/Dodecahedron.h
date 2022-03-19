#ifndef city_modeling_Dodecahedron_H
#define city_modeling_Dodecahedron_H

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

class Dodecahedron : public Shape
{
public:
    Dodecahedron(int mode, int subdivide_d)
    {
        for(int i=0;i<facets.size();i++){
                    glm::vec3 a = glm::normalize(glm::make_vec3(vertices[facets[i][0]-1]));
                    glm::vec3 b = glm::normalize(glm::make_vec3(vertices[facets[i][1]-1]));
                    glm::vec3 c = glm::normalize(glm::make_vec3(vertices[facets[i][2]-1]));
                    subdivide(a,b,c, subdivide_d);
        }
        // std::cout<<vertex.size()/3<<" "<<normal.size()/3<<std::endl;

        for(int i=0;i<vertex.size()/3;i++){
                    glm::vec3 a = vertex[3*i];
                    glm::vec3 b = vertex[3*i+1];
                    glm::vec3 c = vertex[3*i+2];
                    // subdivide(a,b,c, subdivide_d);
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

        // std::cout<<vertex.size()/3<<" "<<normal.size()/3<<std::endl;
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

    void subdivide(const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3, GLuint depth)
    {
        if (depth == 0)
        {
            // drawTriangle(v1, v2, v3);
            vertex.push_back(v1);
            vertex.push_back(v2);
            vertex.push_back(v3);
            return;
        }

        glm::vec3 v12 = glm::normalize((v1 + v2) / 2.0f);
        glm::vec3 v23 = glm::normalize((v2 + v3) / 2.0f);
        glm::vec3 v31 = glm::normalize((v3 + v1) / 2.0f);

        subdivide(v1, v12, v31, depth - 1);
        subdivide(v2, v23, v12, depth - 1);
        subdivide(v3, v31, v23, depth - 1);
        subdivide(v12, v23, v31, depth - 1);
    }

private:
    // static constexpr int NUM_FACETS = 20;
    // static constexpr int NUM_VERTICES = NUM_FACETS * 3;

    const std::vector<glm::vec3> vertices{glm::vec3(0, 0.618, 1.618),
        glm::vec3(0, -0.618, 1.618),
        glm::vec3(0, -0.618, -1.618),
        glm::vec3(0, 0.618, -1.618),
        glm::vec3(1.618, 0, 0.618),
        glm::vec3(-1.618, 0, 0.618),
        glm::vec3(-1.618, 0, -0.618),
        glm::vec3(1.618, 0, -0.618),
        glm::vec3(0.618, 1.618, 0),
        glm::vec3(-0.618, 1.618, 0),
        glm::vec3(-0.618, -1.618, 0),
        glm::vec3(0.618, -1.618, 0),
        glm::vec3(1, 1, 1),
        glm::vec3(-1, 1, 1),
        glm::vec3(-1, -1, 1),
        glm::vec3(1, -1, 1),
        glm::vec3(1, -1, -1),
        glm::vec3(1, 1, -1),
        glm::vec3(-1, 1, -1),
        glm::vec3(-1, -1, -1)
    };
    const std::vector<std::vector<int>> facets = 
    {
        {1, 2, 16}, {16, 5, 13}, {13, 1, 16},
        {1, 13, 9}, {9, 10, 14}, {14, 1, 9},
        {1, 14, 6}, {6, 15, 2}, {2, 1, 6},
        {2, 15, 11}, {11, 12, 16}, {16, 2, 11},
        {3, 4, 18}, {18, 8, 17}, {17, 3, 18},
        {3, 17, 12}, {12, 11, 20}, {20, 3, 12},
        {3, 20, 7}, {7, 19, 4}, {4, 3, 7},
        {19, 10, 9}, {9, 18, 4}, {4, 19, 9},
        {16, 12, 17}, {17, 8, 5}, {5, 16, 17},
        {5, 8, 18}, {18, 9, 13}, {13, 5, 18},
        {14, 10, 19}, {19, 7, 6}, {6, 14, 19},
        {6, 7, 20}, {20, 11, 15}, {15, 6, 20}
    };

    std::tuple<glm::vec3, glm::vec3, glm::vec3> subdivideFacet(GLuint handle)
    {
        const glm::vec3 & A = vertex[handle * 3];
        const glm::vec3 & B = vertex[handle * 3 + 1];
        const glm::vec3 & C = vertex[handle * 3 + 2];

        glm::vec3 V_AB = glm::normalize((A + B) / 2.0f);
        glm::vec3 V_BC = glm::normalize((B + C) / 2.0f);
        glm::vec3 V_AC = glm::normalize((A + C) / 2.0f);

        return {V_AB, V_BC, V_AC};
    }

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec3> avged_normal;
};



#endif  // city_modeling_Dodecahedron_H
