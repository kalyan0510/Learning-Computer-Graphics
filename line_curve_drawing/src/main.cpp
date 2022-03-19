#include <cstdlib>
#include <iostream>
#include <fstream>

#include "global/Context.h"
#include "shader/shader.h"
#include "shape/Circle.h"
#include "shape/Ellipse.h"
#include "shape/LineSegment.h"
#include "shape/CubicCurve.h"


void cursorPosCallback(GLFWwindow * window, double, double);

void keyboardInputCallback(GLFWwindow *);

void mouseButtonCallback(GLFWwindow *, int, int, int);

///VARIABLE TO STORE CUBIC CURVE POLYNOMIALS//////////////
std::vector<glm::vec4> polynomials;

///END OF |VARIABLE TO STORE CUBIC CURVE POLYNOMIALS|//////


// All global stuff are stored inside this context object
Context & context = Context::getInstance();


int main()
{
    // Loading cubic curve params from curveparams.txt
    std::fstream myfile("curveparams.txt", std::ios_base::in);
    double a_p, b_p, c_p, d_p;
    while(myfile >> a_p >> b_p >> c_p >> d_p) {
        polynomials.emplace_back(a_p, b_p, c_p, d_p);
        // std::cout << a_p << " " << b_p << " " << c_p << " " << d_p << "\n";
    }

    // 1. Initialize OpenGL content by GLFW

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window = glfwCreateWindow(context.SCR_WIDTH,
                                           context.SCR_HEIGHT,
                                           "line_curve_draw - Line Segment Mode",
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

///START OF CUSTOM CODE SEGMENT////////////////////////////////////////////////////////

// method to pull vertex buffer from cubic curve and update it to context
void initCubicCurve(){
    glm::vec4 polynomial = polynomials[context.polyNo%polynomials.size()];
    context.vertexBufferData = CubicCurve(polynomial[0],polynomial[1],polynomial[2],polynomial[3],
                                                context.SCR_WIDTH,
                                               context.SCR_HEIGHT).getVertexBufferData();
}


// method to draw linesegment between each vertices of the polyline /polygon
void makePolyLines()
{
    context.vertexBufferData.clear();
    LineSegment lineSegment;
    std::vector<glm::vec2> tempVBA;
    // iterate through each consequitive vertex pair 
    for(int iter=1; iter<context.vertexBufferDataPloyLine.size(); iter++) {
        lineSegment = LineSegment(context.vertexBufferDataPloyLine[iter-1][0],
                                 context.vertexBufferDataPloyLine[iter-1][1],
                                 context.vertexBufferDataPloyLine[iter][0],
                                 context.vertexBufferDataPloyLine[iter][1],
                                 context.SCR_WIDTH,
                                 context.SCR_HEIGHT);
        tempVBA = lineSegment.getVertexBufferData();
        context.vertexBufferData.insert(context.vertexBufferData.end(), tempVBA.begin(), tempVBA.end());
    }
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
        glfwSetWindowTitle(window, "line_curve_draw - Line Segment Mode");
        context.setState(Context::LINE_SEGMENT);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "line_curve_draw - Polyline Mode");
        context.setState(Context::POLYLINE);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "line_curve_draw - Polygon Mode");
        context.setState(Context::POLYGON);
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "line_curve_draw - Circle Mode");
        context.setState(Context::CIRCLE);
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "line_curve_draw - Ellipse Mode");
        context.setState(Context::ELLIPSE);
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "line_curve_draw - Cubic Curve Mode");
        context.setState(Context::CUBIC_CURVE);
        // added this call, so that curve is displayed immediately on loading this screen
        initCubicCurve();
    }
}


void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    switch (context.state)
    {
    case Context::LINE_SEGMENT:
    case Context::CIRCLE:
    case Context::ELLIPSE:
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            context.lastMouseLeftPressXPos = context.cursorXPos;
            context.lastMouseLeftPressYPos = context.cursorYPos;
            context.showPreview = true;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            context.showPreview = false;
        }

        break;
    }
    case Context::POLYLINE:
    case Context::POLYGON:
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            // set starting point of polygon
            if(context.mouseClickCount==0) {
                context.polyLineStartPoint = {context.cursorXPos, context.cursorYPos};
            }
            // note down the last mouse press locations
            context.lastMouseLeftPressXPos = context.cursorXPos;
            context.lastMouseLeftPressYPos = context.cursorYPos;
            // mouse click counted. Used for noting down the first vertex
            context.mouseClickCount++;
            // add current location into the polygon vertex buffer
            context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            // set to preview to update the trailing line segment dynamically
            context.showPreview = true;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            // add the final line segment
            context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            // if its a polygon, add one more line segment between starting vertex and ending vertex to close the loop
            if(context.state == Context::POLYGON) {
                context.vertexBufferDataPloyLine.emplace_back(context.polyLineStartPoint);
                makePolyLines();
            }
            context.showPreview = false;
        }

        break;
    }
    case Context::CUBIC_CURVE:
    {
        context.vertexBufferData.clear();
        // Register mouse presses to toggle between different polynomials
        if(action == GLFW_PRESS){
           context.polyNo++;
        }
        // draw the curve to vertex buffer
        initCubicCurve();
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

    static LineSegment currentLineSegment;

    if (context.state == Context::LINE_SEGMENT ||
        context.state == Context::POLYLINE ||
        context.state == Context::POLYGON)
    {
        currentLineSegment = LineSegment(context.lastMouseLeftPressXPos,
                                         context.lastMouseLeftPressYPos,
                                         context.cursorXPos,
                                         context.cursorYPos,
                                         context.SCR_WIDTH,
                                         context.SCR_HEIGHT);
    }

    if (context.showPreview)
    {
        switch (context.state)
        {
        case Context::LINE_SEGMENT:
        {
            context.vertexBufferData = currentLineSegment.getVertexBufferData();
            break;
        }
        case Context::POLYLINE:
        case Context::POLYGON:
        {
            std::vector<glm::vec2> tempVBA;
            // draw all line segments from the vertices stored
            makePolyLines();
            // display the last line segment dynaically
            tempVBA = currentLineSegment.getVertexBufferData();
            context.vertexBufferData.insert(context.vertexBufferData.end(), tempVBA.begin(), tempVBA.end());
            break;
        }
        case Context::CIRCLE:
        {
            context.vertexBufferData = Circle(context.lastMouseLeftPressXPos,
                                              context.lastMouseLeftPressYPos,
                                              context.cursorXPos,
                                              context.cursorYPos,
                                              context.SCR_WIDTH,
                                              context.SCR_HEIGHT).getVertexBufferData();
            break;
        }
        case Context::ELLIPSE:
        {
            context.vertexBufferData = Ellipse(context.lastMouseLeftPressXPos,
                                               context.lastMouseLeftPressYPos,
                                               context.cursorXPos,
                                               context.cursorYPos,
                                               context.SCR_WIDTH,
                                               context.SCR_HEIGHT).getVertexBufferData();
            break;
        }
        case Context::CUBIC_CURVE:
        {
            // Do nothing as the cursor event will not trigger any change in cubic curve rasterization
            break;
        }
        default:
        {
            std::cout << std::unitbuf
                      << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                      << "\n[ERROR] " << "Unknown context state \"" << context.state << '\"'
                      << std::nounitbuf << std::endl;
            glfwTerminate();
            std::abort();
        }
        }
    }
}
