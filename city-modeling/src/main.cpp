#include <bits/stdc++.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader/Camera.h"
#include "shader/Shader.h"
#include "shape/Cube.h"
#include "shape/Tetrahedron.h"
#include "shape/Icosahedron.h"
#include "shape/ShapeGroup.h"
#include "shape/Octahedron.h"
#include "shape/Dodecahedron.h"
#include <math.h> 


#define PI 3.14159f

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void cursorPosCallback(GLFWwindow * window, double xpos, double ypos);

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


void perFrameTimeLogic();

void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);

void render1();
void render2();
void render3();
void render4();
void render5();
void render6();
void render7();
void render8();
void render9();
void render10();

// screen size
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;


struct RendModel {
    std::vector<Shader> shaders;
    Shape* shape;
    glm::mat4 modelTransform;
    glm::vec3 objColor;
    glm::vec4 innerTlevels;
    glm::vec4 outerTlevels;
    std::vector<float> data;
};

void renderAnObject(Shader shader, RendModel myModel);


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));


// global contexts
int mode = 0;
int problem = 1;
std::vector<RendModel> mybunch;
bool additive = false;


Shader phongShader;//("src/shader/phong.vert.glsl", "src/shader/phong.frag.glsl");
Shader flatShader;//("src/shader/phong.vert.glsl", "src/shader/flat.frag.glsl");
Shader normalShader;//("src/shader/phong.vert.glsl", "src/shader/normal.frag.glsl");

Shader tappflatShader;//("src/shader/phong.vert.glsl", "src/shader/normal.frag.glsl");

std::vector<Shader> simpleShaders;//{flatShader, flatShader, normalShader, phongShader};
std::vector<Shader> prblm2Shaders;//{tappflatShader, tappflatShader, normalShader, phongShader};
std::vector<Shader> sphereShaders;
std::vector<Shader> cylinderShaders;
std::vector<Shader> coneShaders;
std::vector<Shader> circleShaders;
std::vector<Shader> torusShaders;
std::vector<Shader> rectShaders;

Shape* base = new Tetrahedron(3);
// mouse
bool mousePressed = false;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool shiftPressed = false;

int subdivide_d = 0;
int subhelper = 0;
glm::vec3 temppos(0.0f, 0.0f, 0.0f);


// timing
float deltaTime = 0.0f;     // time between current frame and last frame
float lastFrame = 0.0f;
float theta = 0;
float speed = 2.0*PI/15.0;

bool hstart=false;
bool vstart=false;

// lighting
glm::vec3 lightPos(200.0f, 200.0f, 200.0f);
GLuint phongVertexBuffer;
GLuint phongNormalBuffer;
GLuint phongVertexArray;

std::vector<int> patch_size_by_prblm{0,3,3,4,4,4};

int main()
{


    // {flatShader, flatShader, normalShader, phongShader};

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Special care will be need to taken for normal vectors if you support viewpoprt scaling;
    // just forbid it for convenience.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "city_modeling", nullptr, nullptr);

    if (!window)
    {
        std::cout << std::unitbuf
                  << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                  << "\n[ERROR] " << "Failed to create GLFW window!"
                  << std::nounitbuf << std::endl;
        glfwTerminate();
        std::abort();
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << std::unitbuf
                  << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                  << "\n[ERROR] " << "Failed to initialize GLAD!"
                  << std::nounitbuf << std::endl;

        std::abort();
    }


    phongShader = (*(new Shader("src/shader/phong.vert.glsl", "src/shader/phong.frag.glsl")));
    flatShader = (*(new Shader("src/shader/phong.vert.glsl", "src/shader/flat.frag.glsl")));
    normalShader = (*(new Shader("src/shader/phong.vert.glsl", "src/shader/normal.frag.glsl")));
    
    
    simpleShaders.push_back(flatShader);
    simpleShaders.push_back(flatShader);
    simpleShaders.push_back(normalShader);
    simpleShaders.push_back(phongShader);
    
    // tappflatShader = (*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/p2.phong.frag.glsl")));
    // tappflatShader = (*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/p2.phong.frag.glsl")));
    // tappflatShader = (*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/p2.phong.frag.glsl")));
    // tappflatShader = (*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/p2.phong.frag.glsl")));
    // tappflatShader = (*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/p2.phong.frag.glsl")));
    
    // prblm2Shaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/q.tese","src/shader/q.frag"))));
    prblm2Shaders.push_back((*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/flat.frag.glsl"))));
    prblm2Shaders.push_back((*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/flat.frag.glsl"))));
    prblm2Shaders.push_back((*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/normal.frag.glsl"))));
    prblm2Shaders.push_back((*(new Shader("src/shader/p2.phong.vert.glsl", "src/shader/p2.tesc","src/shader/p2.eval","src/shader/phong.frag.glsl"))));
    
    sphereShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/sphere.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    sphereShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/sphere.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    sphereShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/sphere.tese","src/shader/norm.gs","src/shader/normal.frag.glsl"))));
    sphereShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/sphere.tese","src/shader/norm.gs","src/shader/phong.frag.glsl"))));

    torusShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/torus.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    torusShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/torus.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    torusShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/torus.tese","src/shader/norm.gs","src/shader/normal.frag.glsl"))));
    torusShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/torus.tese","src/shader/norm.gs","src/shader/phong.frag.glsl"))));

    coneShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/conetop.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    coneShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/conetop.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    coneShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/conetop.tese","src/shader/norm.gs","src/shader/normal.frag.glsl"))));
    coneShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/conetop.tese","src/shader/norm.gs","src/shader/phong.frag.glsl"))));

    circleShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/circle.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    circleShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/circle.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    circleShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/circle.tese","src/shader/norm.gs","src/shader/normal.frag.glsl"))));
    circleShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/circle.tese","src/shader/norm.gs","src/shader/phong.frag.glsl"))));

    cylinderShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/cylinder.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    cylinderShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/cylinder.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    cylinderShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/cylinder.tese","src/shader/norm.gs","src/shader/normal.frag.glsl"))));
    cylinderShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/cylinder.tese","src/shader/norm.gs","src/shader/phong.frag.glsl"))));

    rectShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/rect.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    rectShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/rect.tese","src/shader/norm.gs","src/shader/flat.frag.glsl"))));
    rectShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/rect.tese","src/shader/norm.gs","src/shader/normal.frag.glsl"))));
    rectShaders.push_back((*(new Shader("src/shader/q.vert", "src/shader/q.tesc","src/shader/rect.tese","src/shader/norm.gs","src/shader/phong.frag.glsl"))));

    // std::cout << "HERE"<<std::endl; 
    // mybunch.push_back({normalShader, someShape, glm::mat4(1.0), glm::vec3(1.0f,0.0f,0.5f)});
    render1();
    // std::cout << "HERE"<<std::endl; 
    glGenVertexArrays(1, &phongVertexArray);
    glBindVertexArray(phongVertexArray);

 
    glGenBuffers(1, &phongVertexBuffer);
    glGenBuffers(1, &phongNormalBuffer);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void *>(0));

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    int frame_num = 0;

    while (!glfwWindowShouldClose(window))
    {   
        frame_num++;
        if(mode==0 ) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        // shape = (frame_num%2==0)?std::unique_ptr<Shape>(new Cube(2)):std::unique_ptr<Shape>(new Cube(3));
        // std::cout<<typeid(shape).name()<<std::endl;
        perFrameTimeLogic();
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // std::cout<<"P "<<camera.Position[0]<<" "<<camera.Position[1]<<" "<<camera.Position[2]<<std::endl;
    // std::cout<<"W "<<camera.Front[0]<<" "<<camera.Front[1]<<" "<<camera.Front[2]<<std::endl;
    // std::cout<<"W "<<camera.Right[0]<<" "<<camera.Right[1]<<" "<<camera.Right[2]<<std::endl;
    // std::cout<<"W "<<camera.Up[0]<<" "<<camera.Up[1]<<" "<<camera.Up[2]<<std::endl;
    // std::cout<<"Y"<<camera.Yaw<<" "<<camera.Pitch<<" "<<std::endl;

        for(int shpid=0;shpid<mybunch.size();shpid++) {
            glm::mat4 model(1.0f);
            renderAnObject(mybunch[shpid].shaders[mode], mybunch[shpid]);
        }
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &phongVertexArray);
    glDeleteBuffers(1, &phongVertexBuffer);
    glDeleteBuffers(1, &phongNormalBuffer);
    for(auto shader:simpleShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:prblm2Shaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:sphereShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:torusShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:coneShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:circleShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:cylinderShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    for(auto shader:rectShaders){
        glDeleteProgram(shader.getShaderProgramHandle());
    }
    
    // glDeleteProgram(simpleShaders[mode].getShaderProgramHandle());
    // glDeleteProgram(prblm2Shaders[mode].getShaderProgramHandle());
    // glDeleteProgram(flatShader.getShaderProgramHandle());
    // glDeleteProgram(normalShader.getShaderProgramHandle());
    glfwTerminate();

    return 0;
}


void render() {
    switch(problem) {
        case 1:
        render1();
        break;
        case 2:
        render2();
        break;        
        case 3:
        render3();
        break;
        case 4:
        render4();
        break;
        case 5:
        render5();
        break;
        case 6:
        render6();
        break;
        case 7:
        render7();
        break;
        case 8:
        render8();
        break;
        case 9:
        render9();
        break;
        case 10:
        render10();
        break;
    }
}

void render1()  {
    if(!additive) mybunch.clear();
    // Shape* icosa = new Icosahedron(3);
    Shape* octa = new Octahedron(3);
    Shape* cube = new Cube(3);
    Shape* tetra = new Tetrahedron(3);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)+temppos);
    model = glm::rotate( model, 25.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({simpleShaders, octa, model, glm::vec3(1.0f, 0.5f, 0.31f)});
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f)+temppos);
    model = glm::rotate( model, 0.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({simpleShaders, cube, model, glm::vec3(1.0f,0.0f,0.5f)});
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)+temppos);
    model = glm::rotate( model, 45.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({simpleShaders, tetra, model, glm::vec3(1.0f,0.0f,0.5f)});
}


void render10()  {
    if(!additive) mybunch.clear();
    // Shape* icosa = new Icosahedron(3);
    Shape* octa = new Octahedron(3);
    Shape* cube = new Cube(3);
    Shape* tetra = new Tetrahedron(3);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate( model, 25.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({prblm2Shaders, octa, model, glm::vec3(1.0f, 0.5f, 0.31f)});
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::rotate( model, 45.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({prblm2Shaders, cube, model,  glm::vec3(0.4f,0.4f,0.4f)});
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
    model = glm::rotate( model, 45.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({prblm2Shaders, tetra, model, glm::vec3(0.1f,1.f,0.7f)});
}

void render2() {
    if(!additive) mybunch.clear();
    Shape* icosa = new Icosahedron(3, subdivide_d);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)+temppos);
    model = glm::rotate( model, 25.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({simpleShaders, icosa, model, glm::vec3(1.0f, 0.5f, 0.31f)});
}


void renderCylinder(float ar, float br, float h, glm::mat4 model, glm::vec3 color) {
    float levels = 9+6*subdivide_d; 
    mybunch.push_back({cylinderShaders, base, glm::translate(glm::mat4(1.0), temppos)*model, color,
     glm::vec4(levels), glm::vec4(levels), 
     //a,b,r,ths,the,rs,re
     {ar,br,0,-PI, PI, .0,h}
    });
    mybunch.push_back({circleShaders, base,  glm::translate(glm::mat4(1.0), temppos)*model, color,
     glm::vec4(levels), glm::vec4(levels), 
     //a,b,r,ths,the,rs,re
     {ar,br,0,-PI, PI, .0,0}
    });
    mybunch.push_back({circleShaders, base, glm::translate(glm::mat4(1.0), temppos)*model, color,
     glm::vec4(levels), glm::vec4(levels), 
     //a,b,r,ths,the,rs,re
     {ar,br,h,-PI, PI, .0,0}
    });
}

void renderCone(float ar, float br, float h, glm::mat4 model, glm::vec3 color) {
    float levels = 9+6*subdivide_d; 
    mybunch.push_back({coneShaders, base, glm::translate(glm::mat4(1.0), temppos)*model, color,
     glm::vec4(levels), glm::vec4(levels), 
     //a,b,r,ths,the,rs,re
     {ar,br,.7,-PI, PI, .0,h}
    });
    mybunch.push_back({circleShaders, base, glm::translate(glm::mat4(1.0), temppos)*model, color,
     glm::vec4(levels), glm::vec4(levels), 
     //a,b,r,ths,the,rs,re
     {h*ar,h*br,h,-PI, PI, .0,0}
    });
}

void renderSphere(float ra, float rb, float rc, glm::mat4 model, glm::vec3 color, float e1, float e2) {
    float levels = 9+6*subdivide_d; 
    mybunch.push_back({sphereShaders, base, glm::translate(glm::mat4(1.0), temppos)*model, color,
     glm::vec4(levels), glm::vec4(levels), 
     //rx, ry, rz, ths, the, phis, phie
     {ra,rb,rc,-PI, 2*PI, -PI/2.0f,0,e1,e2}
    });
}

void render3() {
    if(!additive) mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    glm::mat4 model;
    // subdivide_d = 2;
    float levels = 9+6*subdivide_d; 

    // CONE
    model = glm::rotate( glm::mat4(1.0f), PI, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(3.50f, -2.0f, 0.0f));
    renderCone(.5,.5,4,model, glm::vec3(1.0f, 0.5f, 0.31f));


    // CYLINDER
    model = glm::rotate( glm::mat4(1.0f), PI, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-4.0f, -1.0f, 0.0f));
    renderCylinder(2.,2.,3.,model, glm::vec3(1.0f, 0.5f, 0.31f));

    // SPHERE
    model = glm::rotate( glm::mat4(1.0f), 25.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f))*model;
    renderSphere(1.5,1.5,1.5,model, glm::vec3(1.0f, 0.5f, 0.31f), 1.0, 1.0);
}


void render4() {
    glm::mat4 model;
    // subdivide_d = 2;
    std::vector<std::vector<glm::vec4>> levels = {
             {glm::vec4(4.0,4.0,0.0,0.0),glm::vec4(2.0)},
     {glm::vec4(7.0),glm::vec4(7.0,4.0,7.0,4.0)},
     {glm::vec4(13.0),glm::vec4(13.0)}, 
    };
    if(!additive) mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)+temppos);
    model = glm::rotate( model, PI/4, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({sphereShaders, tetra, model, glm::vec3(1.0f, 0.5f, 0.31f),
        levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {2.0,3.0,1.0,-PI, PI, -PI/2.0f,PI/2.0f,1.0f,1.0f}
    });
}

void render5() {
    glm::mat4 model;
    // subdivide_d = 2;
    std::vector<std::vector<glm::vec4>> levels = {
     {glm::vec4(13.0),glm::vec4(13.0)}, 
     {glm::vec4(20.0),glm::vec4(20.0)}, 
     {glm::vec4(32.0),glm::vec4(32.0)}
    };
    if(!additive) mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    model = glm::rotate( glm::mat4(1.0f), -PI/6, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)+temppos);
    mybunch.push_back({torusShaders, tetra, model, glm::vec3(1.0f, 0.5f, 0.31f),
        levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {3.0,2.0,1.0,-PI, PI, -PI,PI,1.0,1.0}
    });
}

void render6() {
    glm::mat4 model(1.0f);
    // subdivide_d = 2;
    std::vector<std::vector<glm::vec4>> levels = {
     {glm::vec4(13.0),glm::vec4(13.0)}, 
     {glm::vec4(20.0),glm::vec4(20.0)}, 
     {glm::vec4(32.0),glm::vec4(32.0)}
    };
    if(!additive) mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    model = glm::rotate( model, PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    mybunch.push_back({sphereShaders, tetra, glm::translate(glm::mat4(1.0f), temppos+ glm::vec3(-4.0f, 0.0f, 0.0f))*model, glm::vec3(1.0f, 0.5f, 0.31f),
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {2.0,1.0,1.0,-PI, PI, -PI/2.0f,PI/2.0f,2.0f,1.0f}
    });

    mybunch.push_back({sphereShaders, tetra, glm::translate(glm::mat4(1.0f), temppos+ glm::vec3(4.0f, 0.0f, 0.0f))*model, glm::vec3(1.0f, 0.5f, 0.31f),
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {1.0,2.0,2.0,-PI, 1.2*PI, -PI/2.0f,PI/2.0f,1.0f,.10f}
    });

    mybunch.push_back({sphereShaders, tetra, glm::translate(glm::mat4(1.0f), temppos+ glm::vec3(0.0f, 0.0f, 0.0f))*model, glm::vec3(1.0f, 0.5f, 0.31f),
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {1.0,2.0,1.0,-PI, 1.2*PI, -PI/2.0f,PI/2.0f,.10f,.10f}
    });    
}
void render7() {
    if(!additive) mybunch.clear();
    Shape* dodeca = new Dodecahedron(3, subdivide_d);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate( model, 25.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    mybunch.push_back({simpleShaders, dodeca, model, glm::vec3(1.0f, 0.5f, 0.31f)});
}

void renderRect(float a, float b, glm::mat4 model, glm::vec3 color) {
    std::vector<std::vector<glm::vec4>> levels = {
     {glm::vec4(2.0),glm::vec4(2.0)}, 
     {glm::vec4(6.0),glm::vec4(6.0)}, 
     {glm::vec4(8.0),glm::vec4(8.0)}
    };
    mybunch.push_back({rectShaders, base, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))*model, color,
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {a,b,1.0,-PI, 1.2*PI, -PI/2.0f,PI/2.0f,.10f,.10f}
    });   
}

void renderRoadSystem() {
    renderRect(1.5f, 30.0f, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.02f, 0.0f)), glm::vec3(0.05f, 0.36f, 0.61f));
    renderRect(1.5f, 30.0f, glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.04f, 0.0f)), {0.1,0.1,0.1});
    renderRect(1.5f, 30.0f, glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 0.04f, 0.0f)), {0.1,0.1,0.1});
    renderRect(1.5f, 30.0f, glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.04f, 0.0f)), {0.1,0.1,0.1});
    
    renderRect(30.f, 1.50f, glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.04f, 30.0f)), {0.1,0.1,0.1});
    renderRect(30.f, 1.50f, glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.04f, 20.0f)), {0.1,0.1,0.1});
    renderRect(30.f, 1.50f, glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.04f, 10.0f)), {0.1,0.1,0.1});
    renderRect(30.f, 1.50f, glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.04f, 00.0f)), {0.1,0.1,0.1});
}

void renderStadium(float e) {
    glm::mat4 model;
    // subdivide_d = 2;
    std::vector<std::vector<glm::vec4>> levels = {
     {glm::vec4(13.0),glm::vec4(13.0)}, 
     {glm::vec4(20.0),glm::vec4(20.0)}, 
     {glm::vec4(32.0),glm::vec4(32.0)}
    };
    if(!additive) mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    model = glm::rotate( glm::mat4(1.0f), 0.f, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)+temppos);
    mybunch.push_back({torusShaders, tetra, model, glm::vec3(.6f, .6f, .6f),
        levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {2.0,1.0,1.0,-PI, 1.3*PI, -PI,PI/2,1.0,e}
    });
    mybunch.push_back({circleShaders, base, glm::translate(glm::mat4(1.0), temppos), glm::vec3(0.1,0.7,0.1),
     glm::vec4(20), glm::vec4(20), 
     //a,b,r,ths,the,rs,re
     {2,2,0.1,-PI, PI, .0,0}
    });
    renderRect(1.f, 1.50f, glm::translate(glm::mat4(1.0f), temppos+glm::vec3(0.f, 0.15f, 00.0f)), {1.,0.34,0.2});
}

void renderbow() {
    glm::mat4 model;
    // subdivide_d = 2;
    std::vector<std::vector<glm::vec4>> levels = {
     {glm::vec4(13.0),glm::vec4(13.0)}, 
     {glm::vec4(20.0),glm::vec4(20.0)}, 
     {glm::vec4(32.0),glm::vec4(32.0)}
    };
    if(!additive) mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    model = glm::rotate( glm::mat4(1.0f), PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)+temppos)*model;
    mybunch.push_back({torusShaders, tetra, model, glm::vec3(.6f, .6f, .6f),
        levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {4.0,.2,.2,-PI, 2*PI, -PI,PI/2,1.0,1.0}
    });
    // mybunch.push_back({circleShaders, base, glm::translate(glm::mat4(1.0), temppos), glm::vec3(0.1,0.7,0.1),
    //  glm::vec4(20), glm::vec4(20), 
    //  //a,b,r,ths,the,rs,re
    //  {2.8,2.8,0.1,-PI, PI, .0,0}
    // });
    // renderRect(1.f, 1.50f, glm::translate(glm::mat4(1.0f), temppos+glm::vec3(0.f, 0.15f, 00.0f)), {1.,0.34,0.2});
}


void renderSuperBuilding(float h, float e1, float e2) {
 
    glm::mat4 model = glm::rotate( glm::mat4(1.0f), PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f))*model;
    renderSphere(1.5,1.5,6.5,model, glm::vec3(1.0f, 0.5f, 0.31f), e1,e2);

    model = glm::rotate( glm::mat4(1.0f), PI, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.0f, -.50f, 0.0f));
    renderCylinder(2.,2.,.5,model, glm::vec3(1.f, .4f, 0.2f));
}


void render8() {
    mybunch.clear();
    additive = true;
    glm::vec3 oneoffset(-10,0,-15);
    renderRect(60.0f,60.0f, glm::mat4(1.0f), {0.4,0.4,0.4});
    temppos = glm::vec3(-6,0.5,6)+oneoffset;
    render1();
    temppos = glm::vec3(6,0.5,6)+oneoffset;
    render1();
    temppos = glm::vec3(-6,0.5,3)+oneoffset;
    render2();
    temppos = glm::vec3(-8,0.5,1)+oneoffset;
    render3();
    temppos = glm::vec3(5,0.5,-5)+oneoffset;
    render4();
    // temppos = glm::vec3(10, 5, 0)+oneoffset;
    // render5();
    temppos = glm::vec3(-6, 1, -6)+oneoffset;
    render6();
    renderRoadSystem(); 
    temppos = glm::vec3(15, 0, 15);
    renderStadium(.1);
    temppos = glm::vec3(5, 0, 5);
    renderStadium(2);
    temppos = glm::vec3(5, 0, -5);
    renderStadium(1);
    temppos = glm::vec3(15, 0, 5);
    renderSuperBuilding(5, 0.1, 1);
    temppos = glm::vec3(25, 0, 5);
    renderSuperBuilding(7, 1,2);
    temppos = glm::vec3(15, 0, 25);
    renderSuperBuilding(6, 1,.1);
    temppos = glm::vec3(25, 0, 15);
    renderSuperBuilding(3, 2, .1);
    temppos = glm::vec3(25, 0, 25);
    renderSuperBuilding(4,2,1);

    temppos = glm::vec3(-15, 0, 5);
    renderSuperBuilding(5, 0.1, 1);
    temppos = glm::vec3(-25, 0, 5);
    renderSuperBuilding(7, 1,2);
    temppos = glm::vec3(-15, 0, 25);
    renderSuperBuilding(6, 1,.1);
    temppos = glm::vec3(-25, 0, 15);
    renderSuperBuilding(3, 2, .1);
    temppos = glm::vec3(-25, 0, 25);
    renderSuperBuilding(4,2,1);

    temppos = glm::vec3(25, 0, -5);
    renderSuperBuilding(4,1,.1);
    temppos = glm::vec3(25, 0, -15);
    renderSuperBuilding(4,1,2);
    temppos = glm::vec3(25, 0, -25);
    renderSuperBuilding(4,2,.1);
    temppos = glm::vec3(15, 0, -5);
    renderSuperBuilding(4,.1,.1);

    temppos = glm::vec3(15, 0, -15);
    renderSuperBuilding(4,1,.1);
    temppos = glm::vec3(15, 0, -25);
    renderSuperBuilding(4,1,2);
    temppos = glm::vec3(5, 0, -5);
    renderSuperBuilding(4,2,.1);
    temppos = glm::vec3(5, 0, -15);
    renderSuperBuilding(4,.1,.1);

    temppos = glm::vec3(-5, 0, 25);
    renderbow();

    temppos = glm::vec3(5, 0, -25);
    renderbow();
    temppos = glm::vec3(15, 0, -25);
    renderbow();

    additive = false;
}
void render9() {render8();}


void renderSuper() {
    glm::mat4 model(1.0f);
    // subdivide_d = 2;
    std::vector<std::vector<glm::vec4>> levels = {
     {glm::vec4(13.0),glm::vec4(13.0)}, 
     {glm::vec4(20.0),glm::vec4(20.0)}, 
     {glm::vec4(32.0),glm::vec4(32.0)}
    };
    mybunch.clear();
    Shape* tetra = new Tetrahedron(3);
    model = glm::rotate( model, PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    mybunch.push_back({sphereShaders, tetra, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f))*model, glm::vec3(1.0f, 0.5f, 0.31f),
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {1.0,1.0,1.0,-PI, PI, -PI/2.0f,0.0f,2.0f,1.0f}
    });

    mybunch.push_back({sphereShaders, tetra, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f))*model, glm::vec3(1.0f, 0.5f, 0.31f),
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {1.0,1.0,10.0,-PI, 1.2*PI, -PI/2.0f,0.f,1.0f,.10f}
    });

    mybunch.push_back({sphereShaders, tetra, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))*model, glm::vec3(1.0f, 0.5f, 0.31f),
     levels[subdivide_d][0],levels[subdivide_d][1], 
     //rx, ry, rz, ths, the, phis, phie
     {1.0,1.0,1.0,-PI, 1.2*PI, -PI/2.0f,0.f,.10f,.10f}
    });    
}

void renderAnObject(Shader shader, RendModel myModel) {
    // std::cout<<data.size()<<std::endl;
    Shape* shape = myModel.shape;
    shader.use();
    shader.setVec3("objectColor", myModel.objColor);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.Position);
    // glm::mat4 model(1.0f);
    shader.setMat4("model", myModel.modelTransform);
    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float) SCR_WIDTH / (float) SCR_HEIGHT,
                                            0.01f,
                                            100.0f);
    shader.setMat4("projection", projection);
    shader.setInt("mode", mode);
    shader.setVec4("innerTlevels", myModel.innerTlevels);
    shader.setVec4("outerTlevels", myModel.outerTlevels);
    for(int p=0;p<myModel.data.size();p++) {
        shader.setFloat("i"+std::to_string(p), myModel.data[p]);
        // std::cout<<("i"+std::to_string(p))<<std::endl;
    }



    auto sz = patch_size_by_prblm[problem]==4?4:shape->getNumVertices();
    glBindBuffer(GL_ARRAY_BUFFER, phongVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sz * sizeof(glm::vec3)),
                 shape->getVertexData(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void *>(0));

    glBindBuffer(GL_ARRAY_BUFFER, phongNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sz * sizeof(glm::vec3)),
                 shape->getNormalData(mode),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void *>(0));
    glBindVertexArray(phongVertexArray);
    glPatchParameteri(GL_PATCH_VERTICES, patch_size_by_prblm[problem]);
    // glDrawElements(GL_PATCHES, shape->getNumVertices(), GL_UNSIGNED_INT, 0);
    auto drawType = (shader.num==2)?GL_TRIANGLES:GL_PATCHES;
    glDrawArrays(drawType, 0, shape->getNumVertices());
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (mods==GLFW_MOD_SHIFT && shiftPressed==true){
        // std::cout<<(key==GLFW_KEY_EQUAL)<<"Dadada"<<std::endl;
        shiftPressed = false;
        }
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        hstart = true;
        // camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        vstart = true;
        // camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        mode = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        mode = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        mode = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        mode = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        shiftPressed = true;
        render();
        // std::cout<<"shiftPressed"<<std::endl;
    }
    // if (shiftPressed && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE))
    // {
    //     std::cout<<"shiftRELEASED"<<std::endl;
    //     shiftPressed = false;

    // }       
     if (shiftPressed & glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {   
        if((subhelper+5)%4==0)
            subdivide_d = (subdivide_d + 1)%3;
        subhelper++;
        // shiftPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        if((subhelper+5)%4==0)
            subdivide_d = (subdivide_d + 2)%3;
        subhelper--;
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        problem = 1;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        problem = 2;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
    {
        problem = 3;
        subdivide_d=2;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
    {
        problem = 4;

        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
        problem = 5;
        subdivide_d=2;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
    {
        subdivide_d=2;
        problem = 6;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS)
    {
        problem = 7;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS)
    {
        camera.Position = glm::vec3(30.3355, 24.3995 ,41.3455);
        camera.Front = glm::vec3(-0.468172, -0.553392, -0.688892);
        camera.Right = glm::vec3(0.827079 ,0, -0.562085);
        camera.Up = glm::vec3(-0.311054 ,0.832921, -0.457699);
        camera.Yaw = 235.3;
        camera.Pitch = -33.0;

        problem = 8;
        // render();
    }
    if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)
    {
        problem = 9;
        // render();
    }
    

    if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS)
    {
        problem = 10;
        // render();
    }  
    render();

}


void perFrameTimeLogic()
{
    // cout<<
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if(theta<2*PI && (hstart || vstart)){
        theta = deltaTime*speed + theta;
        // std::cout<<theta<<std::endl;

        if (hstart){
            camera.Position = glm::vec3(5*sin(theta)+30, 24.0f,5*cos(theta)+41);
                    // camera.Position = glm::vec3(30.3355, 24.3995 ,41.3455);
            camera.Front = glm::vec3(-0.468172, -0.553392, -0.688892);
            camera.Right = glm::vec3(0.827079 ,0, -0.562085);
            camera.Up = glm::vec3(-0.311054 ,0.832921, -0.457699);
            camera.Yaw = 235.3;
            camera.Pitch = -33.0;
        }
        if (vstart){

            camera.Position = glm::vec3( 30.0f,24 + 5*cos(theta),41+15*sin(theta) );
            // camera.Position = glm::vec3(5*sin(theta)+30, 24.0f,5*cos(theta)+41);
                    // camera.Position = glm::vec3(30.3355, 24.3995 ,41.3455);
            camera.Front = glm::vec3(-0.468172, -0.553392, -0.688892);
            camera.Right = glm::vec3(0.827079 ,0, -0.562085);
            camera.Up = glm::vec3(-0.311054 ,0.832921, -0.457699);
            camera.Yaw = 235.3;
            camera.Pitch = -33.0;
        }

    }else {
        theta = 0;
        hstart = false;
        vstart = false;
    }
}


void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mousePressed = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mousePressed = false;
    }
}


void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    auto xoffset = static_cast<float>(xpos - lastX);
    auto yoffset = static_cast<float>(lastY - ypos);  // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    if (mousePressed)
    {
        camera.processMouseMovement(xoffset, yoffset);
    }
}


void scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}


