#include "utility.h"
#include "scene.h"

//Variable definitions
GLfloat pcX = 512; //player character X position
GLfloat pcY = 512; //player character Y position
GLfloat pcTransX = 0; //player character X transform
GLfloat pcTransY = 0; //player character Y transform

float pcSpeed = 10.0f; // speed of pc

//Textures
GLuint player; //player texture

//Flags
bool drawCollisionBoxes = true;

//Function definitions
void update();
void display();
void reshape(int w, int h);
void init();
void keyfunction(unsigned char key, int x, int y);
void special(int key, int x, int y);
GLuint loadPNG(char* name);

void update(){
    sceneUpdate();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    update();

    //Player character
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, player);
            glTranslatef(pcTransX, pcTransY, 0);
            glColor3f(1, 0, 0);
            drawQuad(pcX, pcY, 256, 256, 1, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, 1, 1);
        if(drawCollisionBoxes){
            drawBox(512, 512, 256, 256);
        }
    glPopMatrix();

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void){
    glClearColor(0, 0, 0, 0);

    sceneInit();
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
            pcTransX -= pcSpeed;
            break;
        case GLUT_KEY_RIGHT:
            pcTransX += pcSpeed;
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