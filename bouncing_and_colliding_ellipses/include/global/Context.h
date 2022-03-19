//
// Created by ax on 3/5/21.
//

#ifndef INC_123456789_boucing_and_colliding_ellipses_CONTEXT_H
#define INC_123456789_boucing_and_colliding_ellipses_CONTEXT_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


/// A singleton Context class managing all global stuff
class Context
{
public:
     int SCR_WIDTH {1600};
     int SCR_HEIGHT {900};

    enum State
    {
        BOUNCING_BALLS = 1,
        FOUR_BOUNCING_BALLS = 2,
        BOUNCING_FACES = 3,
        FOUR_BOUNCING_FACES = 4,
        BOUNCING_ELLIPSES = 5,
        OTHER = 6
    }
    state = BOUNCING_BALLS;

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

    // used for storing ellipse(s) state
    std::vector<std::vector<double>> ellipsesState{};

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


#endif  // INC_123456789_boucing_and_colliding_ellipses_CONTEXT_H

