#include <cstdlib>
#include <iostream>
#include <fstream>

#include "global/Context.h"
#include "shader/tess_shader.h"
#include "shape/BCurve.h"
#include "shape/CCurve.h"
#include <string>
// #include <algorithm>

void cursorPosCallback(GLFWwindow * window, double, double);

void keyboardInputCallback(GLFWwindow *);

void mouseButtonCallback(GLFWwindow *, int, int, int);

///VARIABLE TO STORE CUBIC CURVE POLYNOMIALS//////////////
std::vector<glm::vec4> polynomials;
bool isLeftPressed = false;
int selindex=-1;
glm::vec2 loc_mem;



///END OF |VARIABLE TO STORE CUBIC CURVE POLYNOMIALS|//////


// All global stuff are stored inside this context object
Context & context = Context::getInstance();

void load_points(){
    context.vertexBufferDataPloyLine.clear();
    // Loading cubic curve params from curveparams.txt
    std::fstream myfile("points.txt", std::ios_base::in);
    double a_p, b_p;
    while(myfile >> a_p >> b_p) {
        // polynomials.emplace_back(a_p, b_p, c_p, d_p);
        context.vertexBufferDataPloyLine.emplace_back(a_p, b_p);
        // std::cout << a_p << " " << b_p << " " << c_p << " " << d_p << "\n";
    }
    std::cout<<"loaded\n";
}
void save_points(){
    // context.vertexBufferDataPloyLine.clear();
    // Loading cubic curve params from curveparams.txt
    // std::fstream myfile("points.txt", std::ios_base::in);
    std::ofstream myfile;
    myfile.open ("points.txt");
    // double a_p, b_p;
    for(int i=0;i<context.vertexBufferDataPloyLine.size();i++) {
        myfile << context.vertexBufferDataPloyLine[i][0]<<" "<<context.vertexBufferDataPloyLine[i][1]<<"\n";
        // polynomials.emplace_back(a_p, b_p, c_p, d_p);
        // context.vertexBufferDataPloyLine.emplace_back(a_p, b_p);
        // std::cout << a_p << " " << b_p << " " << c_p << " " << d_p << "\n";
    }
    myfile.close();
    std::cout<<"saved\n";
}

int main()
{


    // 1. Initialize OpenGL content by GLFW

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window = glfwCreateWindow(context.SCR_WIDTH,
                                           context.SCR_HEIGHT,
                                           "catmull_rom - 4 point Bezier curve",
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
    std::string shader_paths[2] = {"src/shader/vert.glsl", "src/shader/bezier_curve.frag"};
    GLenum shader_types[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};       
    Shader shader(shader_paths, shader_types, 2);
    // shader.use();


    //------------------------
    // std::string tess_shader_paths[4] = {"src/shader/bezier_curve.vert", "src/shader/bezier_curve.frag"};
    // GLenum tess_shader_types[4] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};       
    // Shader tess_shader(tess_shader_paths, tess_shader_types, 2);

    //------------------------
    // https://github.com/sayakbiswas/OpenGL-BezierCurves/blob/master/BezierCurves.cpp
    
    // https://github.com/fcaruso/GLSLParametricCurve
    // test with this shader instead

    // std::string tess_shader_paths[4] = {"src/shader/bezier_curve.vert", "src/shader/bezier_curve.tess",
    // "src/shader/bezier_curve.eval", "src/shader/bezier_curve.frag"};
    // GLenum tess_shader_types[4] = {GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER};       
    // Shader tess_shader(tess_shader_paths, tess_shader_types, 4);
    


    std::string tess_shader_paths[4] = {"src/shader/bezier_curve.vert", "src/shader/tess_control.tesc",
    "src/shader/tess_evaluation.tese", "src/shader/bezier_curve.frag"};
    // std::string tess_shader_paths[4] = {"src/shader/BezierCurve.vertexshader", "src/shader/BezierCurve.tessshader",
    // "src/shader/BezierCurve.evalshader", "src/shader/bezier_curve.frag"};
    GLenum tess_shader_types[4] = {GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER};       
    Shader btess_shader(tess_shader_paths, tess_shader_types, 4);
        
    btess_shader.use();

    std::string ctess_shader_paths[4] = {"src/shader/bezier_curve.vert", "src/shader/tess_control.tesc",
    "src/shader/cr_tess_eval.tese", "src/shader/bezier_curve.frag"};
    // std::string tess_shader_paths[4] = {"src/shader/BezierCurve.vertexshader", "src/shader/BezierCurve.tessshader",
    // "src/shader/BezierCurve.evalshader", "src/shader/bezier_curve.frag"};
    GLenum ctess_shader_types[4] = {GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER};       
    Shader ctess_shader(ctess_shader_paths, ctess_shader_types, 4);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if(context.state==Context::CR_CURVE) {
            ctess_shader.use();
        } else{
            btess_shader.use();
        }
        glBindVertexArray(VAO);              // bind VAO because drawing commands fetch data from current VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);  // also bind VBO because we are modifying its data! (NOT needed if no changes to its data)


        // for (int p=0;p<context.vertexBufferData.size();p++){
        //     std::cout<<context.vertexBufferData.data()[p][0]<<" "<<context.vertexBufferData.data()[p][1]<<" | ";
        // }
        // std::cout<<std::endl;
        // Update VBO
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(context.vertexBufferData.size() * sizeof(glm::vec2)),
                     context.vertexBufferData.data(),
                     GL_DYNAMIC_DRAW );

        glEnableVertexAttribArray(0);
        // Draw
        glPatchParameteri( GL_PATCH_VERTICES, 4 );
        glDrawArrays( GL_PATCHES, 0, context.vertexBufferData.size());
        // This function will take configuration of generic vertex attribute arrays from currently-bound VAO
        // and render shape w.r.t. using generic vertex attribute arrays
        // glDrawArrays(GL_POINTS,
        //              0,                                                       // start from index 0 in current VBO
        //              static_cast<GLsizei>(context.vertexBufferData.size()));  // draw these number of GL_POINTS elements
        shader.use();
        // glDrawArrays( GL_LINE_STRIP, 0, 12 );
        // glPatchParameteri( GL_PATCH_VERTICES, 1);
        glBindVertexArray(0);              // unbind whatever objects
        glBindBuffer(GL_ARRAY_BUFFER, 0);  // not currently in use
        glBindVertexArray(VAO);              // bind VAO because drawing commands fetch data from current VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);  // also bind VBO because we are modifying its data! (NOT needed if no changes to its data)

        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(context.vertexBufferDataPloyLine.size() * sizeof(glm::vec2)),
                     context.vertexBufferDataPloyLine.data(),
                     GL_DYNAMIC_DRAW );
        glPointSize(15.0);
        glDrawArrays( GL_POINTS, 0, context.vertexBufferDataPloyLine.size());

        glBindVertexArray(0);              // unbind whatever objects
        glBindBuffer(GL_ARRAY_BUFFER, 0);  // not currently in use
        glDisableVertexAttribArray( 0 );


        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.getShaderProgramHandle());
    glDeleteProgram(btess_shader.getShaderProgramHandle());
    glDeleteProgram(ctess_shader.getShaderProgramHandle());
    glfwTerminate();

    return EXIT_SUCCESS;
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
        glfwSetWindowTitle(window, "catmull_rom - 4 point Bezier curve");
        context.setState(Context::B_CURVE);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "catmull_rom - Piecewise Bezier curve");
        context.setState(Context::PB_CURVE);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        glfwSetWindowTitle(window, "catmull_rom - Catmull Rom Spline");
        context.setState(Context::CR_CURVE);
    }

    // if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    // {
    //     glfwSetWindowTitle(window, "catmull_rom - Other");
    //     context.setState(Context::OTHER);
    // }
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
    {
        if(context.couple_pts.size()>0){
            context.vertexBufferDataPloyLine.erase(context.vertexBufferDataPloyLine.begin()+context.couple_pts[context.couple_pts.size()-1]);
        }
        context.couple_pts.clear();
        cursorPosCallback(nullptr, 0,0);

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        load_points();
        context.freeze=true;
        context.couple_pts.clear();
        context.showPreview=false;
        cursorPosCallback(nullptr, 0,0);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        save_points();
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        context.freeze=true;
        context.couple_pts.clear();
        context.showPreview=false;
        cursorPosCallback(nullptr, 0,0);

    }
}

int findSelected(double xpos, double ypos) {
    int idx = -1;
    for(int i=0;i<context.vertexBufferDataPloyLine.size(); i++) {
        double dist = (context.vertexBufferDataPloyLine[i][0]-xpos)*(context.vertexBufferDataPloyLine[i][0]-xpos);
        dist += (context.vertexBufferDataPloyLine[i][1]-ypos)*(context.vertexBufferDataPloyLine[i][1]-ypos);
        if(dist<256.0){
            idx = i;
        }
    }
    return idx;
}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    // std::cout<<((action==GLFW_PRESS)?"PRESSED":"RELEASED")<<std::endl;
    switch (context.state)
    {
    case Context::B_CURVE:
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            context.lastMouseLeftPressXPos = context.cursorXPos;
            context.lastMouseLeftPressYPos = context.cursorYPos;
            context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            context.showPreview = true;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            context.showPreview = false;
        }
        if(context.vertexBufferDataPloyLine.size() > (context.showPreview?3:4)){
            context.vertexBufferDataPloyLine.erase(context.vertexBufferDataPloyLine.begin());
        }
        cursorPosCallback(nullptr, 0,0);
        break;
    }
    case Context::PB_CURVE:
    case Context::CR_CURVE:
    {   
        if(context.freeze) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            {
                isLeftPressed = true;
                selindex = findSelected(context.cursorXPos, context.cursorYPos);
                loc_mem[0]=context.cursorXPos;
                loc_mem[1]=context.cursorYPos;
            }
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            {
                isLeftPressed = false;
                selindex=-1;
                if(loc_mem[0]==context.cursorXPos && loc_mem[1]==context.cursorYPos) {
                    context.couple_pts.push_back(findSelected(context.cursorXPos, context.cursorYPos));
                    if(context.couple_pts.size()>2){
                        context.couple_pts.erase(context.couple_pts.begin());
                    }
                }

            }
            if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                if(context.couple_pts.size()==1) {
                    if(context.couple_pts[0]==0)
                        context.vertexBufferDataPloyLine.insert(context.vertexBufferDataPloyLine.begin(), glm::vec2(context.cursorXPos, context.cursorYPos));
                    if(context.couple_pts[0]==context.vertexBufferDataPloyLine.size()-1)
                        context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
                } else if (context.couple_pts.size()==2) {
                    if(abs(context.couple_pts[0]-context.couple_pts[1])==1){
                        int small = context.couple_pts[0] < context.couple_pts[1] ? context.couple_pts[0]:context.couple_pts[1];
                        context.vertexBufferDataPloyLine.insert(context.vertexBufferDataPloyLine.begin()+small+1, glm::vec2(context.cursorXPos, context.cursorYPos));
                    } else{
                        std::cout<<"can only select consequtive points"<<std::endl;
                    }
                }
                context.couple_pts.clear();
                cursorPosCallback(nullptr, 0,0);
            }
        }

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
            if(!context.freeze)
            context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            // set to preview to update the trailing line segment dynamically
            if(!context.freeze)
            context.showPreview = true;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            // add the final line segment
            // context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            // // if its a polygon, add one more line segment between starting vertex and ending vertex to close the loop
            // if(context.state == Context::POLYGON) {
            //     context.vertexBufferDataPloyLine.emplace_back(context.polyLineStartPoint);
            //     makePolyLines();
            // }
            if(!context.freeze)
            context.vertexBufferDataPloyLine.emplace_back(context.cursorXPos, context.cursorYPos);
            context.showPreview = false;
            context.freeze=true;
        }

        break;
    }
    // case Context::CUBIC_CURVE:
    // {
    //     // TODO
    //     context.vertexBufferData.clear();
    //     // Register mouse presses to toggle between different polynomials
    //     if(action == GLFW_PRESS){
    //        context.polyNo++;
    //     }
    //     // draw the curve to vertex buffer
    //     initCubicCurve();
    //     break;
    // }
    default:
    {
        std::abort();
    }
    }
}

void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    if(window!=nullptr){
        context.cursorXPos = xpos;
        context.cursorYPos = context.SCR_HEIGHT - ypos;  // reverse y coordinate to facilitate debugging
    }
    // static LineSegment currentLineSegment;

    // if (context.state == Context::LINE_SEGMENT ||
    //     context.state == Context::POLYLINE ||
    //     context.state == Context::POLYGON)
    // {
    //     currentLineSegment = LineSegment(context.lastMouseLeftPressXPos,
    //                                      context.lastMouseLeftPressYPos,
    //                                      context.cursorXPos,
    //                                      context.cursorYPos,
    //                                      context.SCR_WIDTH,
    //                                      context.SCR_HEIGHT);
    // }

    // if (context.showPreview)
    // {
    switch (context.state)
    {
        case Context::B_CURVE:
        {
            //context.vertexBufferData = currentLineSegment.getVertexBufferData();
            // context.vertexBufferData = {glm::vec2(context.cursorXPos,
                                         // context.cursorYPos), glm::vec2(20,10), glm::vec2(30,20), glm::vec2(40, 30)};
            int sz = context.vertexBufferDataPloyLine.size();
            if(sz<3 && context.showPreview || sz<4 && !context.showPreview ) break;
            int p = context.showPreview?1:0;
            // BCurve bcurve = BCurve(
            //     context.vertexBufferDataPloyLine[sz-4+p][0], context.vertexBufferDataPloyLine[sz-4+p][1],
            //     context.vertexBufferDataPloyLine[sz-3+p][0], context.vertexBufferDataPloyLine[sz-3+p][1],
            //     context.vertexBufferDataPloyLine[sz-2+p][0], context.vertexBufferDataPloyLine[sz-2+p][1],
            //     context.showPreview?context.cursorXPos:context.vertexBufferDataPloyLine[sz-1][0], context.showPreview?context.cursorYPos:context.vertexBufferDataPloyLine[sz-1][1], 
            //     context.SCR_WIDTH, context.SCR_HEIGHT);
            std::vector<glm::vec2> last_4(context.vertexBufferDataPloyLine.end()-(context.showPreview?3:4), context.vertexBufferDataPloyLine.end());
            if(context.showPreview){
                last_4.emplace_back(context.cursorXPos, context.cursorYPos);
            }
            BCurve bcurve = BCurve(last_4, context.SCR_WIDTH, context.SCR_HEIGHT);
            context.vertexBufferData = bcurve.getVertexBufferData();
            // std::cout<<context.cursorXPos <<"  "<<context.cursorYPos<<std::endl;
            break;
        }
        case Context::PB_CURVE:
        {   
            // std::cout<<selindex<<std::endl;
            if(context.freeze){
                if(isLeftPressed && selindex>-1){
                    context.vertexBufferDataPloyLine[selindex][0] = context.cursorXPos;
                    context.vertexBufferDataPloyLine[selindex][1] = context.cursorYPos;       
                }
            }
            int sz = context.vertexBufferDataPloyLine.size();
            if(sz<3 && context.showPreview || sz<4 && !context.showPreview ) break;
            int p = context.showPreview?1:0;
            // BCurve bcurve = BCurve(
            //     context.vertexBufferDataPloyLine[sz-4+p][0], context.vertexBufferDataPloyLine[sz-4+p][1],
            //     context.vertexBufferDataPloyLine[sz-3+p][0], context.vertexBufferDataPloyLine[sz-3+p][1],
            //     context.vertexBufferDataPloyLine[sz-2+p][0], context.vertexBufferDataPloyLine[sz-2+p][1],
            //     context.showPreview?context.cursorXPos:context.vertexBufferDataPloyLine[sz-1][0], context.showPreview?context.cursorYPos:context.vertexBufferDataPloyLine[sz-1][1], 
            //     context.SCR_WIDTH, context.SCR_HEIGHT);
            std::vector<glm::vec2> allpts(context.vertexBufferDataPloyLine.begin(), context.vertexBufferDataPloyLine.end());
            if(context.showPreview){
                allpts.emplace_back(context.cursorXPos, context.cursorYPos);
            }
            BCurve bcurve = BCurve(allpts, context.SCR_WIDTH, context.SCR_HEIGHT);
            context.vertexBufferData = bcurve.getVertexBufferData();
            // std::cout<<context.cursorXPos <<"  "<<context.cursorYPos<<std::endl;
            break;
            // TODO
            // std::vector<glm::vec2> tempVBA;
            // // draw all line segments from the vertices stored
            // makePolyLines();
            // // display the last line segment dynaically
            // tempVBA = currentLineSegment.getVertexBufferData();
            // context.vertexBufferData.insert(context.vertexBufferData.end(), tempVBA.begin(), tempVBA.end());
            // break;
        }
        case Context::CR_CURVE:
        {
            // std::cout<<selindex<<std::endl;
            if(context.freeze){
                if(isLeftPressed && selindex>-1){
                    context.vertexBufferDataPloyLine[selindex][0] = context.cursorXPos;
                    context.vertexBufferDataPloyLine[selindex][1] = context.cursorYPos;       
                }
            }
            int sz = context.vertexBufferDataPloyLine.size();
            if(sz<1 && context.showPreview || sz<2 && !context.showPreview ) break;
            int p = context.showPreview?1:0;
            // BCurve bcurve = BCurve(
            //     context.vertexBufferDataPloyLine[sz-4+p][0], context.vertexBufferDataPloyLine[sz-4+p][1],
            //     context.vertexBufferDataPloyLine[sz-3+p][0], context.vertexBufferDataPloyLine[sz-3+p][1],
            //     context.vertexBufferDataPloyLine[sz-2+p][0], context.vertexBufferDataPloyLine[sz-2+p][1],
            //     context.showPreview?context.cursorXPos:context.vertexBufferDataPloyLine[sz-1][0], context.showPreview?context.cursorYPos:context.vertexBufferDataPloyLine[sz-1][1], 
            //     context.SCR_WIDTH, context.SCR_HEIGHT);
            std::vector<glm::vec2> allpts(context.vertexBufferDataPloyLine.begin(), context.vertexBufferDataPloyLine.end());
            if(context.showPreview){
                allpts.emplace_back(context.cursorXPos, context.cursorYPos);
            }
            CCurve ccurve = CCurve(allpts, context.SCR_WIDTH, context.SCR_HEIGHT);
            context.vertexBufferData = ccurve.getVertexBufferData();
            // std::cout<<context.cursorXPos <<"  "<<context.cursorYPos<<std::endl;
            break;
            // TODO
            // std::vector<glm::vec2> tempVBA;
            // // draw all line segments from the vertices stored
            // makePolyLines();
            // // display the last line segment dynaically
            // tempVBA = currentLineSegment.getVertexBufferData();
            // context.vertexBufferData.insert(context.vertexBufferData.end(), tempVBA.begin(), tempVBA.end());
            // break;
        }
        // case Context::CIRCLE:
        // {
        //     context.vertexBufferData = Circle(context.lastMouseLeftPressXPos,
        //                                       context.lastMouseLeftPressYPos,
        //                                       context.cursorXPos,
        //                                       context.cursorYPos,
        //                                       context.SCR_WIDTH,
        //                                       context.SCR_HEIGHT).getVertexBufferData();
        //     break;
        // }
        // case Context::ELLIPSE:
        // {
        //     context.vertexBufferData = Ellipse(context.lastMouseLeftPressXPos,
        //                                        context.lastMouseLeftPressYPos,
        //                                        context.cursorXPos,
        //                                        context.cursorYPos,
        //                                        context.SCR_WIDTH,
        //                                        context.SCR_HEIGHT).getVertexBufferData();
        //     break;
        // }
        // case Context::CUBIC_CURVE:
        // {
        //     // TODO
        //     // Do nothing as the cursor event will not trigger any change in cubic curve rasterization
        //     break;
        // }
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
    // }
}
