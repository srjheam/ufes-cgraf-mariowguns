#ifndef ENTITY_H
#define ENTITY_H

#include <GL/gl.h>

class Entity {
  private:
    int _height;
    int _width;

    GLfloat _o_x;
    GLfloat _o_y;

  public:
    const int &height() const;
    const int &width() const;

    const GLfloat &o_x() const;
    void o_x(const GLfloat &o_x);

    const GLfloat &o_y() const;
    void o_y(const GLfloat &o_y);

    Entity(int height, int width, GLfloat o_x, GLfloat o_y);

    virtual void draw() const = 0;
};

#endif
