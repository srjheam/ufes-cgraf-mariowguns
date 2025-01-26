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

bool Entity::aabb_isoverlapping_x(const Entity &entity) const {
    // =====-1=-=----=-=2-===

    return 
        (srutils::epslt(pimpl->o_x_, entity.o_x() + entity.width()) && srutils::epsgte(pimpl->o_x_, entity.o_x())) // 1
        || (srutils::epsgt(pimpl->o_x_ + pimpl->width_, entity.o_x()) && srutils::epslte(pimpl->o_x_ + pimpl->width_, entity.o_x() + entity.width())); // 2
}

bool Entity::aabb_isoverlapping_y(const Entity &entity) const {
    // ||
    // ||
    // ||
    // ii
    //  1
    // ||
    // ii
    // ||
    // ii
    // ii
    // ii
    // ii
    // ||
    // ii
    // ||
    //  2
    // ii
    // ||
    // ||
    // ||

    return
        (srutils::epsgt(pimpl->o_y_ + pimpl->height_, entity.o_y()) && srutils::epslte(pimpl->o_y_ + pimpl->height_, entity.o_y() + entity.height())) // 1
        || (srutils::epslt(pimpl->o_y_, entity.o_y() + entity.height()) && srutils::epsgte(pimpl->o_y_, entity.o_y())); // 2
}

bool Entity::aabb_isoverlapping(const Entity &entity) const {
    return aabb_isoverlapping_x(entity) && aabb_isoverlapping_y(entity);
}

bool Entity::aabb_isoverlapping_x_dx(const Entity &entity, const GLfloat &dx) const {
    // =====-1=-=----=-=2-===

    GLfloat tleft = pimpl->o_x_ + dx;
    GLfloat lright = tleft + pimpl->width_;
    
    GLfloat eleft = entity.o_x();
    GLfloat eright = eleft + entity.width();
    
    return ((tleft < eright) && (lright > eleft))
        || ((fmin(pimpl->o_x_, tleft) < eleft) && (fmax(lright, pimpl->o_x_ + pimpl->width_) > eright));
}

bool Entity::aabb_isoverlapping_y_dy(const Entity &entity, const GLfloat &dy) const {
    // ||
    // ||
    // ||
    // ii
    //  1
    // ||
    // ii
    // ||
    // ii
    // ii
    // ii
    // ii
    // ||
    // ii
    // ||
    //  2
    // ii
    // ||
    // ||
    // ||

    GLfloat tbot = pimpl->o_y_ + dy;
    GLfloat ttop = tbot + pimpl->height_;
    
    GLfloat ebot = entity.o_y();
    GLfloat etop = ebot + entity.height();
    
    return ((tbot < etop) && (ttop > ebot))
        || ((fmin(pimpl->o_y_, tbot) < ebot) && (fmax(ttop, pimpl->o_y_ + pimpl->height_) > etop));

}

bool Entity::aabb_isoverlapping_delta(const Entity &stationary, const GLfloat &dx, const GLfloat &dy) const {
    return aabb_isoverlapping_x_dx(stationary, dx) && aabb_isoverlapping_y_dy(stationary, dy);
}

bool Entity::aabb_isoverlapping_dx(const Entity &stationary, const GLfloat &dx) const {
    return aabb_isoverlapping_delta(stationary, dx, .0f);
}

bool Entity::aabb_isoverlapping_dy(const Entity &stationary, const GLfloat &dy) const {
    return aabb_isoverlapping_delta(stationary, .0f, dy);
}

GLfloat Entity::aabb_distanceof_x(const Entity &entity) const {
    if (aabb_isoverlapping_x(entity))
        return 0.f;

    if (srutils::epslte(pimpl->o_x_, entity.o_x())) { // T -> E
        return fabs(entity.o_x() - (pimpl->o_x_ + pimpl->width_));
    }
    else { // E -> T
        return (entity.o_x() + entity.width()) - pimpl->o_x_;
    }
}

GLfloat Entity::aabb_distanceof_y(const Entity &entity) const {
    if (aabb_isoverlapping_y(entity))
        return 0.f;

    if (srutils::epslte(pimpl->o_y_, entity.o_y())) { // T -> E
        return fabs(entity.o_y() - (pimpl->o_y_ + pimpl->height_));
    }
    else { // E -> T
        return (entity.o_y() + entity.height()) - pimpl->o_y_;
    }
}

bool Entity::aabb_isinsideof_x(const Entity &entity) const {
    return srutils::epsgte(pimpl->o_x_, entity.o_x()) && srutils::epslte(pimpl->o_x_ + pimpl->width_, entity.o_x() + entity.width());
}

bool Entity::aabb_isinsideof_y(const Entity &entity) const {
    return srutils::epsgte(pimpl->o_y_, entity.o_y()) && srutils::epslte(pimpl->o_y_ + pimpl->height_, entity.o_y() + entity.height());
}

bool Entity::aabb_isinsideof_dx(const Entity &entity, const GLfloat &dx) const {
    GLfloat nx = pimpl->o_x_ + dx;
    GLfloat nxwidth = nx + pimpl->width_;

    GLfloat ex = entity.o_x();
    GLfloat exwidth = ex + entity.width();

    return ((ex < nx) && (nxwidth < exwidth));
}

bool Entity::aabb_isinsideof_dy(const Entity &entity, const GLfloat &dy) const {
    GLfloat ny = pimpl->o_y_ + dy;
    GLfloat nyheight = ny + pimpl->height_;

    GLfloat ey = entity.o_y();
    GLfloat eyheight = ey + entity.height();

    return ((ey < ny) && (nyheight < eyheight));
}

GLfloat Entity::aabb_insideof_x(const Entity &entity) const {
    // colision walls
    // --------------
    // |            |
    // |1          2|
    // |            |
    // --------------

    GLfloat left = fabs((entity.o_x()) - (pimpl->o_x_));
    GLfloat right = fabs((entity.o_x() + entity.width()) - (pimpl->o_x_ + pimpl->width_));

    return left < right ? -left : right;
}

GLfloat Entity::aabb_insideof_y(const Entity &entity) const {
        // colision walls
        // --------------
        // |     4      |
        // |            |
        // |     3      |
        // --------------

    GLfloat top = fabs((entity.o_y() + entity.height()) - (pimpl->o_y_ + pimpl->height_));
    GLfloat bottom = fabs(pimpl->o_y_ - entity.o_y());
    
    return top < bottom ? top : -bottom;
}
