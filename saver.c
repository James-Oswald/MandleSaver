

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<math.h>

#define uint unsigned int

#include"shader.h"
#include"window.h"
#include"goodTime.h"

GLFWwindow* window;
clock_t begin;
double curx = 0, cury = 0;
double goalx = 0, goaly = 0;
double zoom = 1, goalZoom = 1; 
double time = 0;
uint VAO, VBO, shaderProgram;
uint uWinSize, uTime, uCenter;

void init(){
    if(glewInit() != GLEW_OK){
        printf("GLEW DEAD");
        exit(1);
    }
    uint vertexShader = loadShader("vert.glsl", GL_VERTEX_SHADER);
    uint fragmentShader = loadShader("frag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram = loadProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    //we need to make a quad covering the whole screen
    float vertices[] = {    
        // first triangle
        1,  1,  // top right
        1, -1,  // bottom right
        -1, 1, // top left 
        // second triangle
        1, -1, // bottom right
        -1, -1, // bottom left
        -1,  1, // top left
    };  

    glGenBuffers(1, &VBO);  
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    uWinSize = glGetUniformLocation(shaderProgram, "windowSize");
    uTime = glGetUniformLocation(shaderProgram, "time");
    uCenter = glGetUniformLocation(shaderProgram, "center");
    glUseProgram(shaderProgram);
    
    glBindVertexArray(VAO);
}

void loop(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glUniform2d(uWinSize, width, height);
    if(time > 10){
        double theta = 2*M_PI*rand()/RAND_MAX; //Pick a random theta
        double r = (1 - cos(theta))/2;
        goalx = r * cos(theta) + 0.25;
        goaly = r * sin(theta);
        beginTimer();
    }
    double time = readTimer();
    curx = (goalx - curx)/100*time + curx;
    cury = (goaly - cury)/100*time + cury;
    glUniform2d(uCenter, curx, cury);
    glUniform1d(uTime, time);
    char title[30];
    sprintf(title, "%f", time);
    glfwSetWindowTitle(window, title);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void cleanUp(){
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}


int main(int argc, char** argv){
    window = windowInit();
    init();
    windowLoop(window, loop);
    cleanUp();
    return 0;
}