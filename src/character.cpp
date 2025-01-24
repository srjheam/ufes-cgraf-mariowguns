#include "rectangle.h"
#include <GL/glut.h>

Rectangle::Rectangle(int height, int width, GLclampf bg_red, GLclampf bg_green,
                     GLclampf bg_blue, GLfloat o_x, GLfloat o_y) {
    _height = height;
    _width = width;
    _bg_red = bg_red;
    _bg_green = bg_green;
    _bg_blue = bg_blue;
    _o_x = o_x;
    _o_y = o_y;
}

const int &Rectangle::height() const { return _height; }
const int &Rectangle::width() const { return _width; }

const GLclampf &Rectangle::bg_red() const { return _bg_red; }
const GLclampf &Rectangle::bg_green() const { return _bg_green; }
const GLclampf &Rectangle::bg_blue() const { return _bg_blue; }

const GLfloat &Rectangle::o_x() const { return _o_x; }
void Rectangle::o_x(const GLfloat &o_x) { _o_x = o_x; }

const GLfloat &Rectangle::o_y() const { return _o_y; }
void Rectangle::o_y(const GLfloat &o_y) { _o_y = o_y; }

void Rectangle::draw() const {
    glColor3f(_bg_red, _bg_green, _bg_blue);
    glBegin(GL_QUADS);
        glVertex2f(_o_x, _o_y);
        glVertex2f(_o_x + _width, _o_y);
        glVertex2f(_o_x + _width, _o_y + _height);
        glVertex2f(_o_x, _o_y + _height);
    glEnd();
}
