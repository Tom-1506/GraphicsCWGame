#include "utility.h"
#include "player.h"
#include "scene.h"

//Function definitions
void update();
void display();
void reshape(int w, int h);
void init();
void keyfunction(unsigned char key, int x, int y);

void update(){
    sceneUpdate();
    playerUpdate();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    update();

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void){
    glClearColor(0, 0, 0, 0);

    sceneInit();
    playerInit();
}

void reshape(int w, int h){
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyfunction(unsigned char key, int x, int y){
    switch(key){
        case 27:
            exit(0);
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(1800,400);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("CW_Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutKeyboardFunc(keyfunction);

    glutMainLoop();
}