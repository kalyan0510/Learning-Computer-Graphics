cmake_minimum_required(VERSION 3.16)
project(123456789_catmull_rom)
set(CMAKE_CXX_STANDARD 17)

# packages to find

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# all sources

set(GLAD
        include/glad/glad.h
        include/KHR/khrplatform.h
        src/glad/glad.c
        )

set(GLOBAL
        include/global/Context.h
        src/global/Context.cpp
        )

set(SHADER
        include/shader/shader.h
        )

set(SHAPE
        include/shape/BCurve.h
        include/shape/CCurve.h
        include/shape/Shape.h
        src/shape/BCurve.cpp
        src/shape/CCurve.cpp        
        src/shape/Shape.cpp
        )

set(ALL_INCLUDE_DIRS
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${OPENGL_EGL_INCLUDE_DIRS}
        include
        )

set(ALL_SOURCE_FILES
        ${GLAD}
        ${GLOBAL}
        ${SHADER}
        ${SHAPE}
        src/main.cpp
        )

set(ALL_LIBRARIES
        dl
        GLEW::GLEW
        glfw
        OpenGL::GL
        OpenGL::OpenGL
        OpenGL::GLU
        OpenGL::GLX
        OpenGL::EGL
        pthread
        )

# all definitions and options

set(ALL_COMPILE_DEFS
        # -DDEBUG_LINE_SEGMENT
        )

set(ALL_COMPILE_OPTS

        )

# executable target(s)

add_executable(catmull_rom ${ALL_SOURCE_FILES})
target_compile_definitions(catmull_rom PUBLIC ${ALL_COMPILE_DEFS})
target_compile_options(catmull_rom PUBLIC ${ALL_COMPILE_OPTS})
target_include_directories(catmull_rom PUBLIC ${ALL_INCLUDE_DIRS})
target_link_libraries(catmull_rom ${ALL_LIBRARIES})

