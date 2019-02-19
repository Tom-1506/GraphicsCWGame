#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <iostream>
using namespace std;

//Variable definitions
GLfloat pcX = 0; //player character X position
GLfloat pcY = 0; //player character Y position

float pcSpeed = 10.0f; //speed of pc

//Function definitions
void display();
void reshape(int w, int h);
void init();
void keyfunction(unsigned char key, int x, int y);
void special(int key, int x, int y);

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glPushMatrix();
        glTranslatef(pcX, pcY, 0);
        glColor3f(1, 0, 0);
        glBegin(GL_POLYGON);
            glVertex2f(384, 384);
            glVertex2f(384, 640);
            glVertex2f(640, 640);
            glVertex2f(640, 384);
        glEnd();
    glPopMatrix();

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void){
    glClearColor(0, 0, 0, 0);
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

void special(int key, int w, int h){
    switch(key){
        case GLUT_KEY_LEFT:
            pcX -= pcSpeed;
            break;
        case GLUT_KEY_RIGHT:
            pcX += pcSpeed;
            break;
        case GLUT_KEY_UP:
            pcY += pcSpeed;
            break;
        case GLUT_KEY_DOWN:
            pcY -= pcSpeed;
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(2200,400);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("CW_Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutKeyboardFunc(keyfunction);
    glutSpecialFunc(special);

    glutMainLoop();
}