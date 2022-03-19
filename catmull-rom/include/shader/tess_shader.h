#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


class Shader
{
public:
    Shader(std::string shader_paths[], GLenum types[], int length)
    {
        // 1. retrieve the vertexShader/fragmentShader source code from filePath
        // std::cout<<length<<"KAlYAN";
        std::string shader_codes[length]; 

        // std::string vertShaderCode;
        // std::string fragShaderCode;
        for(int i=0; i<length; i++) {
            if (std::ifstream fin {shader_paths[i], std::ifstream::in})
            {
                std::ostringstream sout;
                sout << fin.rdbuf();
                shader_codes[i] = sout.str();
            }
            else
            {
                std::cout << std::unitbuf
                          << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                          << "\n[ERROR] " << "shader file not successfully read!"
                          << std::nounitbuf << std::endl;

                std::abort();
            }
        }

        const char * shader_ptrs[length];
        // const char * fragShaderPtr = fragShaderCode.data();
        for(int i=0;i<length; i++) {
            shader_ptrs[i] = shader_codes[i].data();
            // std::cout<<shader_ptrs[i]<<std::endl;
        }

        // 2. compile shaders

        // vertexShader shader

        unsigned int shaders[length];
        
        for(int i=0;i<length; i++) {
            shaders[i]= glCreateShader(types[i]);
            glShaderSource(shaders[i], 1, &shader_ptrs[i], nullptr);
            glCompileShader(shaders[i]);
            checkCompileErrors(shaders[i], "VERTEX");            
        }


        // // fragmentShader shader
        // unsigned int fragmentShader;
        // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // glShaderSource(fragmentShader, 1, &fragShaderPtr, nullptr);
        // glCompileShader(fragmentShader);
        // checkCompileErrors(fragmentShader, "FRAGMENT");

        // shader program
        shaderProgram = glCreateProgram();
        for(int i=0;i<length; i++)
            glAttachShader(shaderProgram, shaders[i]);
        glLinkProgram(shaderProgram);
        checkCompileErrors(shaderProgram, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        for(int i=0;i<length; i++)
            glDeleteShader(shaders[i]);
    }

    void use()
    {
        glUseProgram(shaderProgram);
    }

    void setBool(const std::string & name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int) value);
    }

    void setInt(const std::string & name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    void setFloat(const std::string & name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    unsigned int getShaderProgramHandle() const
    {
        return shaderProgram;
    }

private:
    // utility function for checking shader compilation/linking errors.
    static void checkCompileErrors(unsigned int shader, const std::string & type)
    {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

                std::cout << std::unitbuf
                          << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                          << "\n[ERROR] " << type << " shader compilation failed"
                          << "\n[ERROR] " << infoLog
                          << std::nounitbuf << std::endl;

                std::abort();
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);

                std::cout << std::unitbuf
                          << "[ERROR] " << __FILE__ << ':' << __LINE__ << ' ' << __PRETTY_FUNCTION__
                          << "\n[ERROR] " << R"(Shader program linking failed)"
                          << "\n[ERROR] " << infoLog
                          << std::nounitbuf << std::endl;

                std::abort();
            }
        }
    }

private:
    unsigned int shaderProgram;
};

#endif // LEARNOPENGL_SHADER_H
