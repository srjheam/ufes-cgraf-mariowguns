#include "entity.h"

#include <cmath>
#include <iostream>
#include <limits>

#include <GL/glut.h>

#include "srutils.h"

class Entity::Impl {
  public:
    GLfloat o_x_;
    GLfloat o_y_;

    int height_;
    int width_;

    GLdouble ttl_;
    bool hidden_;

    GLfloat vector_velocity_;
    GLfloat vector_direction_x_;
    GLfloat vector_direction_y_;

    Impl(GLfloat o_x, GLfloat o_y, int height, int width)
        : o_x_(o_x), o_y_(o_y), height_(height), width_(width),
            ttl_(std::numeric_limits<GLdouble>::max()), hidden_(false),
            vector_velocity_(.0f), vector_direction_x_(.0f), vector_direction_y_(.0f) { }
};

Entity::Entity(GLfloat o_x, GLfloat o_y, int height, int width)
    : pimpl(new Impl(o_x, o_y, height, width)) { }

Entity::Entity(Entity&&) noexcept = default;

Entity::~Entity() = default;

const GLfloat &Entity::o_x() const { return pimpl->o_x_; }
void Entity::o_x(const GLfloat &o_x) const { pimpl->o_x_ = o_x; }

const GLfloat &Entity::o_y() const { return pimpl->o_y_; }
void Entity::o_y(const GLfloat &o_y) const { pimpl->o_y_ = o_y; }

const int &Entity::height() const { return pimpl->height_; }
const int &Entity::width() const { return pimpl->width_; }

const GLdouble &Entity::ttl() const { return pimpl->ttl_; }
void Entity::ttl(const GLdouble &ttl) const { pimpl->ttl_ = ttl; }

const bool &Entity::hidden() const { return pimpl->hidden_; }
void Entity::hidden(const bool &hidden) const { pimpl->hidden_ = hidden; }

const GLfloat &Entity::vector_velocity() const { return pimpl->vector_velocity_; }
void Entity::vector_velocity(const GLfloat &velocity) const { pimpl->vector_velocity_ = velocity; }

const GLfloat &Entity::vector_direction_x() const { return pimpl->vector_direction_x_; }
const GLfloat &Entity::vector_direction_y() const { return pimpl->vector_direction_y_; }

void Entity::vector_set_direction(const GLfloat &dx, const GLfloat &dy) const {
    GLfloat length = srutils::normalizef(dx, dy);

    if (length > 0) {
        pimpl->vector_direction_x_ = dx / length;
        pimpl->vector_direction_y_ = dy / length;
    }
    else {
        pimpl->vector_direction_x_ = .0f;
        pimpl->vector_direction_y_ = .0f;
    }
}

void Entity::vector_set_zero() const {
    pimpl->vector_direction_x_ = .0f;
    pimpl->vector_direction_y_ = .0f;
    pimpl->vector_velocity_ = .0f;
}

void Entity::vector_set(const GLfloat &dx, const GLfloat &dy) const {
    vector_set_direction(dx, dy);
    pimpl->vector_velocity_ = srutils::normalizef(dx, dy);
}

void Entity::vector_set(const GLfloat &dx, const GLfloat &dy, const GLfloat &v) const {
    pimpl->vector_direction_x_ = dx;
    pimpl->vector_direction_y_ = dy;
    pimpl->vector_velocity_ = v;
}

void Entity::vector_sum(const GLfloat &vx, const GLfloat &vy, const GLfloat &v) const {
    GLfloat cvx = pimpl->vector_direction_x_ * pimpl->vector_velocity_;
    GLfloat cvy = pimpl->vector_direction_y_ * pimpl->vector_velocity_;
    
    GLfloat nvx = cvx + (vx * v);
    GLfloat nvy = cvy + (vy * v);
    
    pimpl->vector_velocity_ = srutils::normalizef(nvx, nvy);
    
    if (pimpl->vector_velocity_ > 0) {
        pimpl->vector_direction_x_ = nvx / pimpl->vector_velocity_;
        pimpl->vector_direction_y_ = nvy / pimpl->vector_velocity_;
    }
    else {
        pimpl->vector_direction_x_ = .0f;
        pimpl->vector_direction_y_ = .0f;
    }
}

GLfloat Entity::vector_get_dx_dt(const GLfloat &dt) const {
    return pimpl->vector_direction_x_ * pimpl->vector_velocity_ * dt;
}

GLfloat Entity::vector_get_dy_dt(const GLfloat &dt) const {
    return pimpl->vector_direction_y_ * pimpl->vector_velocity_ * dt;
}

GLfloat Entity::distanceof_x(const Entity &entity) const {
    if (pimpl->o_x_ < entity.o_x()) { // T -> E
        return entity.o_x() - (pimpl->o_x_ + pimpl->width_);
    }
    else { // E -> T
        return pimpl->o_x_ - (entity.o_x() + entity.width());
    }
}

GLfloat Entity::distanceof_y(const Entity &entity) const {
    if (pimpl->o_y_ < entity.o_y()) { // T -> E
        return entity.o_y() - (pimpl->o_y_ + pimpl->height_);
    }
    else { // E -> T
        return pimpl->o_y_ - (entity.o_y() + entity.height());
    }
}

GLfloat Entity::distanceinsideof_x(const Entity &entity) const {
    // colision walls
    // --------------
    // |            |
    // |1          2|
    // |            |
    // --------------
    GLfloat left = (entity.o_x()) - (pimpl->o_x_);
    GLfloat right = (entity.o_x() + entity.width()) - (pimpl->o_x_ + pimpl->width_);  // Distance to right wall
    
    std::cout << "left: " << left << " right: " << right << std::endl;

    return (fabs(left) < fabs(right)) ? left : right;
}

GLfloat Entity::distanceinsideof_y(const Entity &entity) const {
        // colision walls
        // --------------
        // |     4      |
        // |            |
        // |     3      |
        // --------------
    GLfloat top = entity.o_y() + entity.height() - pimpl->o_y_ + pimpl->height_;
    GLfloat bottom = (entity.o_y() + entity.height()) - pimpl->o_y_;
    if (fabs(top) < fabs(bottom)) {
        return top;
    }
    else {
        return bottom;
    }
}
