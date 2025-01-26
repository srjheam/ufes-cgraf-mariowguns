#include "platform.h"

#include <GL/glut.h>

#include "color_rgb.h"

Platform::Platform(
    //! The x coordinate of the origin of the rectangle in its parent's coordinate system.
    GLfloat o_x,
    //! The y coordinate of the origin of the rectangle in its parent's coordinate system.
    GLfloat o_y,
    int height,
    int width,
    ColorRgb background) : Entity(o_x, o_y, height, width), _background(background) {
}

void Platform::draw() const {
    glColor3f(_background.gl_red(), _background.gl_green(), _background.gl_blue());
    glBegin(GL_QUADS);
        glVertex2f(o_x(), o_y());
        glVertex2f(o_x() + width(), o_y());
        glVertex2f(o_x() + width(), o_y() + height());
        glVertex2f(o_x(), o_y() + height());
    glEnd();
}
