#include <cstdlib>
#include <iostream>
#include <fstream>

#include "global/Context.h"
#include "shader/shader.h"
#include "shape/Circle.h"
#include "shape/Ellipse.h"
#include "shape/Ellipses.h"
#include "shape/Faces.h"
#include "shape/LineSegment.h"
#include "shape/CubicCurve.h"
// #include "utils/CollisionDetection.cpp"
#include "utils/EllipseDynamics.cpp"

#define PI 3.14159265
#define TO_PI PI/180.0

void cursorPosCallback(GLFWwindow * window, double, double);

void keyboardInputCallback(GLFWwindow *);

void mouseButtonCallback(GLFWwindow *, int, int, int);
void runFrame();



// All global stuff are stored inside this context object
Context & context = Context::getInstance();

std::vector<double> bodyConfig{0,0,0};
bool debugOn=false;


void loadConfiguration() {
    std::fstream myfile("body_params.txt", std::ios_base::in);
    double a_p, b_p, c_p;
    while(myfile >> a_p >> b_p >> c_p) {
        bodyConfig[0]=a_p;
        bodyConfig[1]=b_p;
        bodyConfig[2]=std::fmod(c_p,360);
    }
}

void loadEllipseConfiguration() {
    std::fstream myfile("ellipse_params.txt", std::ios_base::in);
    double a,b,c,d,e,f,g,h,i;
    while(myfile >> a>>b>>c>>d>>e>>f>>g>>h>>i) {
        context.ellipsesState.push_back({a,b,c,d,e,f,g,h,i});
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    context.SCR_HEIGHT = height;
    context.SCR_WIDTH = width;
    glViewport(0, 0, width, height);

    // Re-render the scene because the current frame was drawn for the old resolution
    // draw();
}


int main()
{
    // Loading cubic curve params from curveparams.txt
    loadConfiguration();

    // 1. Initialize OpenGL content by GLFW

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // glfwGetWindowSize(context.SCR_WIDTH, context.SCR_HEIGHT);


    GLFWwindow * window = glfwCreateWindow(context.SCR_WIDTH,
                                           context.SCR_HEIGHT,
                                           "boucing_and_colliding_ellipses - Bouncing Balls",
                                           nullptr,
                                           nullptr);

    if (!window)
    {
        std::cout << std::unitbuf
                  << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                  << "\n[ERROR] " << "Failed to create GLFW window!"
                  << std::nounitbuf << std::endl;
        glfwTerminate();
        std::abort();
    }

    // Resigter all GUI callbacks here
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // 2. Load OpenGL functions pointers by GLAD

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << std::unitbuf
                  << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                  << "\n[ERROR] " << "Failed to initialize GLAD!"
                  << std::nounitbuf << std::endl;
        glfwTerminate();
        std::abort();
    }

    // 3. Build and compile our shader program
    Shader shader("src/shader/vert.glsl", "src/shader/frag.glsl");
    shader.use();

    // 4. Set up buffer objects

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  // generate 1 vertex array object, bind to handle VAO
    glBindVertexArray(VAO);      // bind VAO to OpenGL context

    unsigned int VBO;
    glGenBuffers(1, &VBO);               // generate 1 buffer object, bind to handle VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind VBO to OpenGL context "GL_ARRAY_BUFFER"

    // Configure generic vertex attribute arrays and store configuration in currently-bound VAO. 
    // When using currently-bound VAO, 
    // the generic vertex attribute array "layout (position = 0) in vec2" will fetch data from currently-bound VBO. 
    // The interpretation of VBO data specified as follows. 
    glVertexAttribPointer(0,                             // index: corresponds to "0" in "layout (position = 0)"
                          2,                             // size: each "vec2" generic vertex attribute has 2 values
                          GL_FLOAT,                      // data type: "vec2" generic vertex attributes are GL_FLOAT
                          GL_FALSE,                      // do not normalize data
                          sizeof(glm::vec2),             // stride between attributes in VBO data
                          reinterpret_cast<void *>(0));  // offset of 1st attribute in VBO data

    // Enable generic vertex attribute array "layout (position = 0) in vec2" to be called by rendering commands (e.g. glDrawArray). 
    // Generic vertex attribute array are disabled by default and aren't accessed by rendering commands like glDrawArray!
    // This generic vertex attribute array will take data from currently-bound VAO. 
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);                // bind buffers to context only when you are using them
    glBindBuffer(GL_ARRAY_BUFFER, 0);    // unbind whatever you are not using

    // 6. Render loop
    glViewport(0, 0, context.SCR_WIDTH, context.SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        // Process input
        keyboardInputCallback(window);

        // Background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);              // bind VAO because drawing commands fetch data from current VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);  // also bind VBO because we are modifying its data! (NOT needed if no changes to its data)

        // Update VBO
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(context.vertexBufferData.size() * sizeof(glm::vec2)),
                     context.vertexBufferData.data(),
                     GL_DYNAMIC_DRAW);

        runFrame();

        // Draw
        // This function will take configuration of generic vertex attribute arrays from currently-bound VAO
        // and render shape w.r.t. using generic vertex attribute arrays
        glDrawArrays(GL_POINTS,
                     0,                                                       // start from index 0 in current VBO
                     static_cast<GLsizei>(context.vertexBufferData.size()));  // draw these number of GL_POINTS elements

        glBindVertexArray(0);              // unbind whatever objects
        glBindBuffer(GL_ARRAY_BUFFER, 0);  // not currently in use

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.getShaderProgramHandle());
    glfwTerminate();

    return EXIT_SUCCESS;
}

std::vector<std::vector<double>> extractShapeInfo(std::vector<std::vector<double>> ellipsesState) {
    std::vector<std::vector<double>> ellipsesShape;
    for (std::vector<double >e: ellipsesState) {
        ellipsesShape.push_back({e[0], e[1], e[2], e[3], e[4], e[7]});
    }
    return ellipsesShape;
}

std::vector<std::vector<double>> extractShadedEllipses(std::vector<std::vector<double>> ellipsesState, bool shaded) {
    std::vector<std::vector<double>> ellipsesShape;
    for (std::vector<double >e: ellipsesState) {
        if(e[8]==0 && (((int)e[7])%2==1) == shaded && e[2]==e[3])
        ellipsesShape.push_back(e);
    }
    return ellipsesShape;
}

std::vector<std::vector<double>> extractFaces(std::vector<std::vector<double>> ellipsesState) {
    std::vector<std::vector<double>> ellipsesShape;
    for (std::vector<double >e: ellipsesState) {
        if(e[8]==1)
        ellipsesShape.push_back(e);
    }
    return ellipsesShape;
}



void runFrame() {
    // printf("Frame started\n");
    switch(context.state){
        case Context::BOUNCING_BALLS:
        case Context::FOUR_BOUNCING_BALLS:
        {
            context.vertexBufferData = Ellipses(extractShapeInfo(context.ellipsesState), true,
                                       context.SCR_WIDTH,
                                       context.SCR_HEIGHT).getVertexBufferData();
         break;   
        }
        case Context::BOUNCING_FACES:
        case Context::FOUR_BOUNCING_FACES:
        {
            context.vertexBufferData = Faces(extractShapeInfo(context.ellipsesState),
                                       context.SCR_WIDTH,
                                       context.SCR_HEIGHT).getVertexBufferData();
            break;
        }
        case Context::BOUNCING_ELLIPSES:{
            // context.vertexBufferData.clear();
            context.vertexBufferData = Ellipses(extractShapeInfo((context.ellipsesState)), false,
                           context.SCR_WIDTH,
                           context.SCR_HEIGHT).getVertexBufferData();
            // auto temp = Ellipses(extractShapeInfo(extractShadedEllipses(context.ellipsesState, false)), false,
            //                context.SCR_WIDTH,
            //                context.SCR_HEIGHT).getVertexBufferData();
            // context.vertexBufferData.insert(context.vertexBufferData.end(), temp.begin(), temp.end());
            auto temp = Faces(extractShapeInfo(extractFaces(context.ellipsesState)),
                           context.SCR_WIDTH,
                           context.SCR_HEIGHT).getVertexBufferData();
            context.vertexBufferData.insert(context.vertexBufferData.end(), temp.begin(), temp.end());
            break;
        }
    }
    
    // printf("KALYAN BUFER %d", context.vertexBufferData.size());
    context.ellipsesState = EllipseDynamics::nextStep(context.ellipsesState, context.SCR_WIDTH, context.SCR_HEIGHT, context, debugOn);
    ////////////////////////
    // makePolyLinesFromInput(CollisionDetection::detect_ellipse_collision(ellipses));
}


////END OF CUSTOM CODE SEGMENT/////////////////////////////////////////////////////////////////

void keyboardInputCallback(GLFWwindow * window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        context.vertexBufferData.clear();
        context.ellipsesState.clear();
        glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 1. Bouncing Balls");
        context.setState(Context::BOUNCING_BALLS);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        context.vertexBufferData.clear();
        context.ellipsesState.clear();
        glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 2. More Bouncing Balls");
        context.setState(Context::FOUR_BOUNCING_BALLS);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        context.vertexBufferData.clear();
        context.ellipsesState.clear();
        glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 3. Bouncing Faces");
        context.setState(Context::BOUNCING_FACES);
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {   
        context.vertexBufferData.clear();
        context.ellipsesState.clear();
        glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 4. More Bouncing Balls");
        context.setState(Context::FOUR_BOUNCING_FACES);
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
         context.vertexBufferData.clear();
        context.ellipsesState.clear();
        loadEllipseConfiguration();

        glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 5. Bouncing Ellipses");
        context.setState(Context::BOUNCING_ELLIPSES);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
         debugOn = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
         debugOn = false;
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
         context.vertexBufferData.clear();
        context.ellipsesState.clear();
        loadEllipseConfiguration();

        glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 6. Others");
        context.setState(Context::OTHER);
        // added this call, so that curve is displayed immediately on loading this screen
        // initCubicCurve();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // glfwSetWindowTitle(window, "boucing_and_colliding_ellipses - 6. Others");
        context.vertexBufferData.clear();
        context.ellipsesState.clear();
        // added this call, so that curve is displayed immediately on loading this screen
        // initCubicCurve();
    }
}


void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    //printf("------");
    loadConfiguration();
    switch (context.state)
    {
    case Context::BOUNCING_BALLS:
    case Context::FOUR_BOUNCING_BALLS:
    case Context::BOUNCING_FACES:
    case Context::FOUR_BOUNCING_FACES:
    case Context::BOUNCING_ELLIPSES:
    case Context::OTHER:
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            double vx = bodyConfig[1]*cos(bodyConfig[2]*TO_PI), vy = bodyConfig[1]*sin(bodyConfig[2]*TO_PI);
            // printf("YOYOYO %f %f", vx, vy);
            context.ellipsesState.push_back(
                    {context.cursorXPos, context.cursorYPos, bodyConfig[0], bodyConfig[0], 0, vx, vy, 1, 0}
                );
        }
        break;
    }
    default:
    {
        std::abort();
    }
    }
}

void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    context.cursorXPos = xpos;
    context.cursorYPos = context.SCR_HEIGHT - ypos;  // reverse y coordinate to facilitate debugging
}
