#include "Utility.h"
#include "Scene.h"

//Function definitions
void update();
void display();
void reshape(int w, int h);
void init();
void keyfunction(unsigned char key, int x, int y);

int viewWidth;
int viewHeight;

Scene scene = Scene();

void update(){
    updateTime();
    scene.sceneUpdate();

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

    scene.sceneInit();
}

void reshape(int w, int h){
    viewWidth = w;
    viewHeight = h;
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0,w,0,h);

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

void mouseFunc(int button, int state, int x, int y){
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            std::cout << x << ", " << viewHeight - y << std::endl;
            break;
        default:
            break;
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(0,200);
    glutInitWindowSize(3820, 1500);
    glutCreateWindow("CW_Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutKeyboardFunc(keyfunction);
    glutMouseFunc(mouseFunc);

    glutMainLoop();
}