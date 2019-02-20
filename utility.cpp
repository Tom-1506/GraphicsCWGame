#include <GL/glu.h>
#include <GL/gl.h>
#include "utility.h"

void drawBox(float centreX, float centreY, float w, float h){
    glBegin(GL_LINE_LOOP);
            glVertex2f(centreX - w/2, centreY - h/2);
            glVertex2f(centreX - w/2, centreY + h/2);
            glVertex2f(centreX + w/2, centreY + h/2);
            glVertex2f(centreX + w/2, centreY - h/2);
    glEnd();
}

void drawQuad(float centreX, float centreY, float w, float h, int texRepeatX, int texRepeatY){
    glBegin(GL_POLYGON);
        glTexCoord2f(0,                   0); glVertex2f(centreX - w/2, centreY - h/2);
        glTexCoord2f(0,          texRepeatY); glVertex2f(centreX - w/2, centreY + h/2);
        glTexCoord2f(texRepeatX, texRepeatY); glVertex2f(centreX + w/2, centreY + h/2);
        glTexCoord2f(texRepeatY,          0); glVertex2f(centreX + w/2, centreY - h/2);
    glEnd();
}