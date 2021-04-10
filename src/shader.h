#include<stdlib.h>
#include<stdio.h>
#include<GL/glew.h>

#define uint unsigned int

uint loadShader(char* filename, uint shaderType){
    FILE* file;
    file = fopen(filename, "r");
    if(!file){
        printf("Could not open %s\n", filename);
        perror("Error:");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* glShaderCont = (char*)malloc(fsize + 1);
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