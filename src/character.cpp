#include "character.h"
#include <GL/glut.h>

class Character::Impl {
  public:
    ColorRgb body_;

    CharacterDirection direction_ = RIGHT;

    Impl(ColorRgb body) : body_(body) { }
};

Character::Character(GLfloat o_x, GLfloat o_y, int height, int width, ColorRgb body) 
    : Entity(o_x, o_y, height, width), pimpl(new Impl(body)) {
    
}

Character::Character(Character&&) noexcept = default;

Character::~Character() = default;

const enum CharacterDirection &Character::direction() const {
    return pimpl->direction_;
}

void Character::direction(const enum CharacterDirection &direction) {
    pimpl->direction_ = direction;
}

void Character::draw() const {
    glColor3f(pimpl->body_.gl_red(), pimpl->body_.gl_green(), pimpl->body_.gl_blue());
    glBegin(GL_QUADS);
        glVertex2f(o_x(), o_y());
        glVertex2f(o_x() + width(), o_y());
        glVertex2f(o_x() + width(), o_y() + height());
        glVertex2f(o_x(), o_y() + height());
    glEnd();
}
