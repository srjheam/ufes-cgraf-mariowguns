#include "bullet.h"
#include <GL/gl.h>

class Bullet::Impl {
public:
    Vector direction;
    GLdouble angle;
    
    Impl(Vector dir) 
        : direction(dir) {
            angle = direction.angle();
        }
};

Bullet::Bullet(GLfloat o_x, GLfloat o_y, GLfloat height, GLfloat width, Vector direction, GLdouble pttl)
    : Entity(o_x, o_y, height, width), pimpl(new Impl(direction)) {
        ttl(pttl);
    }

Bullet::Bullet(Bullet&&) noexcept = default;

Bullet::~Bullet() = default;

const Vector &Bullet::direction() const {
    return pimpl->direction;
}

void Bullet::draw() const {
    glPushMatrix();
        glTranslatef(o_x(), o_y(), 0.0f);
        glRotatef(pimpl->angle, 0.0f, 0.0f, 1.0f);

        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(width(), 0.0f);
            glVertex2f(width(), height());
            glVertex2f(0.0f, height());
        glEnd();    
    glPopMatrix();
}
