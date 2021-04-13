

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<math.h>

#define uint unsigned int

#include"shader.h"      //shader program helper functions
#include"window.h"      //glfw helper functions
#include"goodTime.h"    //custom timer

const double jumpTime = 2;
const double stayTime = 3;
const double zoomLevel = 30;

GLFWwindow* window;
clock_t begin;
double curx = 0, cury = 0;
double goalx = 0, goaly = 0;
double zoom = 1, goalZoom = 1; 
double time = 0, lastTime = 0;
uint VAO, VBO, shaderProgram;
uint uWinSize, uTime, uCenter, uZoom;

int nanErrorCounter = 0;

void saverInit(){
    //Init glew so we don't need to load GL funcs by ourselves
    if(glewInit() != GLEW_OK){
        printf("GLEW DEAD");
        exit(1);
    }

    //Shader program setup
    uint vertexShader = loadShader("./vert.glsl", GL_VERTEX_SHADER);
    uint fragmentShader = loadShader("./frag.glsl", GL_FRAGMENT_SHADER);
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

    //We need to set up that quad in a VBO and VAO
    glGenBuffers(1, &VBO);  
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    //Get uniform locations to write to
    uWinSize = glGetUniformLocation(shaderProgram, "windowSize");
    uTime = glGetUniformLocation(shaderProgram, "time");
    uCenter = glGetUniformLocation(shaderProgram, "center");
    uZoom = glGetUniformLocation(shaderProgram, "zoom");

    //Bind the quad for the main loop
    glUseProgram(shaderProgram);    
    glBindVertexArray(VAO);
}

void saverLoop(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //load the window size uniform
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glUniform2d(uWinSize, width, height);

    //load the center uniform
    double theta;
    if(time > jumpTime + stayTime){ //find a new center to zoom in on every few seconds
        int selection = (int)(3*((double)rand()/RAND_MAX)); //select 1 of 3 interesting areas to zoom on
        switch(selection){
            case 0: //Zoom on the stem
                goalx = -0.5*(double)rand()/RAND_MAX - 1.5;
                goaly = 0;
                break;
            case 1: //zoom on the bulb
                theta = 2*M_PI*((double)rand()/RAND_MAX); //Pick a random angle to zoom on
                goalx = 0.25 * cos(theta) - 1;  
                goaly = 0.25 * sin(theta);
                break;
            default: //zoom on the cartoid
                theta = 2*M_PI*((double)rand()/RAND_MAX); //Pick a random angle to zoom on
                double r = (1 - cos(theta))/2;
                goalx = r * cos(theta) + 0.25;  
                goaly = r * sin(theta);
        }
        time = 0;
        beginTimer();
    }
    lastTime = time;
    time = readTimer();
    double deltat = time - lastTime;
    curx += (goalx-curx)/fabs(jumpTime - time)*deltat;
    cury += (goaly-cury)/fabs(jumpTime - time)*deltat; 
    
    if(isnan(curx) || isnan(cury)){ //if coincidently jumpTime == time and we're too close to our goal
        time = jumpTime + stayTime;
        cury = 0;
        curx = 0;
        nanErrorCounter++;
        return;
    }
        
    glUniform2d(uCenter, curx, cury);
    //load the time uniform

    //This forumla is motivated graphically, see https://www.desmos.com/calculator/vqac7m1j8k
    double timeZoom = zoomLevel*sigmoid(-fabs((zoomLevel/(0.41*stayTime)) * (time - (jumpTime + stayTime/2.5))) + zoomLevel)+1;
    //double timeZoom = zoomLevel*sigmoid((zoomLevel/jumpTime) * (time - jumpTime)) + 1;
    glUniform1d(uZoom, timeZoom);
    glUniform1d(uTime, time);

    //debug via the window tyle
    char title[100];
    sprintf(title, "time: %f, cur:(%f, %f), nanERC: %d", time, curx, cury, nanErrorCounter);
    glfwSetWindowTitle(window, title);

    //draw the window
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void saverFree(){
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}


int main(int argc, char** argv){
    window = windowInit();
    saverInit();
    windowLoop(window, saverLoop);
    saverFree();
    return 0;
}