//
// Created by xihan1 on 8/25/21.
//

#include "global/Context.h"


Context::Context()
{
    vertexBufferData.clear();
}


Context & Context::getInstance()
{
    static Context instance;
    return instance;
}


void Context::setState(State s)
{
    state = s;
    showPreview = false;
    freeze=false;
    couple_pts.clear();

    vertexBufferData.clear();

    mouseClickCount = 0;
    vertexBufferDataPloyLine.clear();
}
