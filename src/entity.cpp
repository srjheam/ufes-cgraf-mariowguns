#include "entity.h"
#include <GL/glut.h>

Entity::Entity(int height, int width, GLfloat o_x, GLfloat o_y) {
    _height = height;
    _width = width;
    _o_x = o_x;
    _o_y = o_y;
}

const int &Entity::height() const { return _height; }
const int &Entity::width() const { return _width; }

const GLfloat &Entity::o_x() const { return _o_x; }
void Entity::o_x(const GLfloat &o_x) { _o_x = o_x; }

const GLfloat &Entity::o_y() const { return _o_y; }
void Entity::o_y(const GLfloat &o_y) { _o_y = o_y; }
