#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

//#define FULLSCREEN

void onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* windowInit(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef FULLSCREEN
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "MandelBrot", monitor, NULL);
#else
    GLFWwindow* window = glfwCreateWindow(500, 500, "MandelBrot", NULL, NULL);
#endif
    if (window == NULL){
        printf("glfw Init failed");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetFramebufferSizeCallback(window, onResize);
    return window;
}

void windowLoop(GLFWwindow* window, void (*loopFunc)(void)){
    while(!glfwWindowShouldClose(window)){
        loopFunc();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}