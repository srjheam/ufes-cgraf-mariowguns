#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <GL/gl.h>

#include "entity.h"

class Character : public Entity {
  private:
    GLclampf _bg_red;
    GLclampf _bg_green;
    GLclampf _bg_blue;

  public:
    const int &height() const;
    const int &width() const;

    const GLclampf &bg_red() const;
    const GLclampf &bg_green() const;
    const GLclampf &bg_blue() const;

    const GLfloat &o_x() const;
    void o_x(const GLfloat &o_x);

    const GLfloat &o_y() const;
    void o_y(const GLfloat &o_y);

    Character(int height, int width, GLclampf bg_red, GLclampf bg_green,
              GLclampf bg_blue, GLfloat o_x, GLfloat o_y);

    void draw() const;
};

#endif
