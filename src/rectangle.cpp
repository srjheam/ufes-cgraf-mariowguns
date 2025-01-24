#include "rectangle.h"
#include <GL/glut.h>

Rectangle::Rectangle(
    int height,
    int width,
    GLclampf bg_red,
    GLclampf bg_green,
    GLclampf bg_blue,
    GLfloat o_x,
    GLfloat o_y) : Entity(height, width, o_x, o_y) {
    _bg_red = bg_red;
    _bg_green = bg_green;
    _bg_blue = bg_blue;
}

const GLclampf &Rectangle::bg_red() const { return _bg_red; }
const GLclampf &Rectangle::bg_green() const { return _bg_green; }
const GLclampf &Rectangle::bg_blue() const { return _bg_blue; }

void Rectangle::draw() const {
    glColor3f(_bg_red, _bg_green, _bg_blue);
    glBegin(GL_QUADS);
        glVertex2f(o_x(), o_y());
        glVertex2f(o_x() + width(), o_y());
        glVertex2f(o_x() + width(), o_y() + height());
        glVertex2f(o_x(), o_y() + height());
    glEnd();
}
