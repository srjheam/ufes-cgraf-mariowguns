#include "vector.h"

#include <cmath>

#include "srutils.h"

class Vector::Impl
{
  public:
    float direction_x_;
    float direction_y_;
    float velocity_;

    Impl() : direction_x_(.0f), direction_y_(.0f), velocity_(.0f) { }
};

Vector::Vector() : pimpl(new Impl) { }

Vector::Vector(const Vector &vector) : pimpl(new Impl) {
    pimpl->direction_x_ = vector.pimpl->direction_x_;
    pimpl->direction_y_ = vector.pimpl->direction_y_;
    pimpl->velocity_ = vector.pimpl->velocity_;
}

Vector& Vector::operator=(const Vector &vector) {
    pimpl->direction_x_ = vector.pimpl->direction_x_;
    pimpl->direction_y_ = vector.pimpl->direction_y_;
    pimpl->velocity_ = vector.pimpl->velocity_;

    return *this;
}

Vector::Vector(Vector &&vector) noexcept = default;

Vector& Vector::operator=(Vector &&vector) noexcept = default;

Vector::~Vector() = default;

const float& Vector::direction_x() const { return pimpl->direction_x_; }
void Vector::set_direction_x(const float &dx) { pimpl->direction_x_ = dx; }

const float& Vector::direction_y() const { return pimpl->direction_y_; }
void Vector::set_direction_y(const float &dy) { pimpl->direction_y_ = dy; }

const float& Vector::velocity() const { return pimpl->velocity_; }
void Vector::set_velocity(const float &velocity) { pimpl->velocity_ = velocity; }

void Vector::set_direction(const float &dx, const float &dy) {
    float length = srutils::normalizef(dx, dy);

    if (length > 0) {
        pimpl->direction_x_ = dx / length;
        pimpl->direction_y_ = dy / length;
    }
    else {
        pimpl->direction_x_ = .0f;
        pimpl->direction_y_ = .0f;
    }
}

void Vector::set_vector(const float &dx, const float &dy) const {
    pimpl->velocity_ = srutils::normalizef(dx, dy);
    
    if (pimpl->velocity_ > 0) {
        pimpl->direction_x_ = dx / pimpl->velocity_;
        pimpl->direction_y_ = dy / pimpl->velocity_;
    }
    else {
        pimpl->direction_x_ = .0f;
        pimpl->direction_y_ = .0f;
    }
}

void Vector::set_zero() const {
    pimpl->direction_x_ = .0f;
    pimpl->direction_y_ = .0f;
    pimpl->velocity_ = .0f;
}

void Vector::sum(const float &dx, const float &dy, const float &velocity) {
    float cvx = pimpl->direction_x_ * pimpl->velocity_;
    float cvy = pimpl->direction_y_ * pimpl->velocity_;
    
    float nvx = cvx + (dx * velocity);
    float nvy = cvy + (dy * velocity);
    
    pimpl->velocity_ = srutils::normalizef(nvx, nvy);
    
    if (pimpl->velocity_ > 0) {
        pimpl->direction_x_ = nvx / pimpl->velocity_;
        pimpl->direction_y_ = nvy / pimpl->velocity_;
    }
    else {
        pimpl->direction_x_ = .0f;
        pimpl->direction_y_ = .0f;
    }
}


float Vector::calc_dx_dt(const float &dt) const {
    return pimpl->direction_x_ * pimpl->velocity_ * dt;
}

float Vector::calc_dy_dt(const float &dt) const {
    return pimpl->direction_y_ * pimpl->velocity_ * dt;
}
