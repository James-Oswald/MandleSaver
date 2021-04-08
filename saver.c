
#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<time.h>
#include<math.h>

#define uint unsigned int

uint loadShader(char* filename, uint shaderType){
    FILE* file;
    file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* glShaderCont = malloc(fsize + 1);
    fread(glShaderCont, 1, fsize, file);
    glShaderCont[fsize] = 0;
    fclose(file);

    uint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (char const* const*)&glShaderCont, NULL);
    glCompileShader(shader);
    free(glShaderCont);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("%s failed to compile with error:\n%s", filename, infoLog);
        exit(1);
    }
    return shader;
}

uint loadProgram(uint vertexShader, uint fragmentShader){
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        printf("failed to link program with error:\n%s", infoLog);
        exit(1);
    }
    return shaderProgram;
}

void onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glutFullScreen();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "MandelBrot", monitor, NULL);
    if (window == NULL){
        printf("glfw Init failed");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);

    GLenum err = glewInit();
    if (err != GLEW_OK){
        printf("GLEW DEAD");
        exit(1);
    }

    uint VAO, VBO, shaderProgram;
    uint vertexShader = loadShader("vert.glsl", GL_VERTEX_SHADER);
    uint fragmentShader = loadShader("frag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram = loadProgram(vertexShader, fragmentShader);

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

    int uWinSize = glGetUniformLocation(shaderProgram, "WindowSize");
    int uTime = glGetUniformLocation(shaderProgram, "time");
    glUseProgram(shaderProgram);
    
    glBindVertexArray(VAO);
    clock_t begin = clock();
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glUniform2f(uWinSize, width, height);
        glUniform1f(uTime, 10*(clock() - begin)/(float)CLOCKS_PER_SEC);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}