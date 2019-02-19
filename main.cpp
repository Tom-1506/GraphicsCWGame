#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "SOIL.h"

#include <iostream>
using namespace std;

//Variable definitions
GLfloat pcX = 0; //player character X position
GLfloat pcY = 0; //player character Y position

float pcSpeed = 10.0f; // speed of pc

//Textures
GLuint ground; //ground texture
GLuint background; //background texture

//Function definitions
void display();
void reshape(int w, int h);
void init();
void keyfunction(unsigned char key, int x, int y);
void special(int key, int x, int y);
GLuint loadPNG(char* name);

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    //background
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, background);
            glColor3f(0, 1, 0);
            glBegin(GL_POLYGON);
                glTexCoord2f(0,  0); glVertex2f(3840, 2160);
                glTexCoord2f(0,  1); glVertex2f(3840,    0);
                glTexCoord2f(1, 1); glVertex2f(0,       0);
                glTexCoord2f(1, 0); glVertex2f(0,    3840);
            glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //Player character
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

    //ground
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, ground);
            glColor3f(1, 0, 1);
            glBegin(GL_POLYGON);
                glTexCoord2f(0,  0); glVertex2f(3840, 256);
                glTexCoord2f(0,  1); glVertex2f(3840,   0);
                glTexCoord2f(15, 1); glVertex2f(0,      0);
                glTexCoord2f(15, 0); glVertex2f(0,    256);
            glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glutSwapBuffers();

    glutPostRedisplay();
}

GLuint loadPNG(char* name){
    glEnable(GL_TEXTURE_2D);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height;
    unsigned char* img = SOIL_load_image(name, &width, &height, 0, SOIL_LOAD_RGBA);

    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    SOIL_free_image_data(img);

    return textureID;
}

void init(void){
    glClearColor(0, 0, 0, 0);

    //ground = loadPNG("textures/magic_dirt.png");
    //background = loadPNG("textures/background.png");
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

    glutInitWindowPosition(1800,400);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("CW_Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutKeyboardFunc(keyfunction);
    glutSpecialFunc(special);

    glutMainLoop();
}