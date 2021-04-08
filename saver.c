
#include<stdio.h>
#include<GL/glut.h>

void draw(){
    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Opengl");
    glutFullScreen();
    //glutDisplayFunc(draw);
    //system("pause");
    return 0;
}