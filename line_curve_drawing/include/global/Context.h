//
// Created by ax on 3/5/21.
//

#ifndef INC_123456789_line_curve_draw_CONTEXT_H
#define INC_123456789_line_curve_draw_CONTEXT_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


/// A singleton Context class managing all global stuff
class Context
{
public:
    const int SCR_WIDTH {1600};
    const int SCR_HEIGHT {900};

    enum State
    {
        LINE_SEGMENT = 1,
        POLYLINE = 2,
        POLYGON = 3,
        CIRCLE = 4,
        ELLIPSE = 5,
        CUBIC_CURVE = 6,
    }
    state = LINE_SEGMENT;

public:
    static Context & getInstance();

public:
    Context(const Context &) = delete;
    Context(Context &&) = delete;
    Context & operator=(const Context &) = delete;
    Context & operator=(Context &&) = delete;

    /// Set state and clean all buffer data.
    /// \param s new state to switch to
    void setState(State s);

public:
    double lastMouseLeftPressXPos {0};
    double lastMouseLeftPressYPos {0};

    double cursorXPos {0};
    double cursorYPos {0};

    bool showPreview {false};

    // used for drawing commands
    std::vector<glm::vec2> vertexBufferData;

    // used for polyline
    std::size_t mouseClickCount {0};
    std::vector<glm::vec2> vertexBufferDataPloyLine;

    // used for polygon
    glm::ivec2 polyLineStartPoint {0, 0};

    // used to switch polynomials in cubic curve
    std::size_t polyNo {0};

private:
    Context();

    ~Context() = default;
};


#endif  // INC_123456789_line_curve_draw_CONTEXT_H

