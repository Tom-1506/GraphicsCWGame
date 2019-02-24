#include "utility.h"

/*bool drawCollisionBoxes = true;*/

void drawBox(float centreX, float centreY, float w, float h){
    glBegin(GL_LINE_LOOP);
            glVertex2f(centreX - w/2, centreY - h/2); //bottom left
            glVertex2f(centreX - w/2, centreY + h/2); //top left
            glVertex2f(centreX + w/2, centreY + h/2); //top right
            glVertex2f(centreX + w/2, centreY - h/2); //bottom right
    glEnd();
}

void drawQuad(float centreX, float centreY, float w, float h, int texRepeatX, int texRepeatY){
    glBegin(GL_POLYGON);
        glTexCoord2f(0,                   0); glVertex2f(centreX - w/2, centreY + h/2);
        glTexCoord2f(0,          texRepeatY); glVertex2f(centreX - w/2, centreY - h/2);
        glTexCoord2f(texRepeatX, texRepeatY); glVertex2f(centreX + w/2, centreY - h/2);
        glTexCoord2f(texRepeatX,          0); glVertex2f(centreX + w/2, centreY + h/2);
    glEnd();
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