#pragma once

#define ASSERT(expression) if(!(expression)) __debugbreak();

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLCall(x)   GLClearError();\
                    x;\
                    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define GLFWCall(x) GLFWClearError();\
                    x;\
                    ASSERT(GLFWLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while(glGetError() != GL_NO_ERROR) {}
}

static bool GLLogCall(const char* function, const char* file, int line) 
{
    while(GLenum error = glGetError())
    {
        // DEBUG_ERROR("[OPENGL ERROR] (", error, "):", function, file, ":", line);
        return false;
    }

    return true;
}

static void GLFWClearError()
{
    while(glfwGetError(NULL) != GLFW_NO_ERROR) {}
}

static bool GLFWLogCall(const char* function, const char* file, int line)
{
    const char* desc;
    bool err = glfwGetError(&desc) != GLFW_NO_ERROR;
    if (err)
    {
        // DEBUG_ERROR("[GLFW ERROR] (", desc, "):", function, file, ":", line);
        return false;
    }

    return true;
}