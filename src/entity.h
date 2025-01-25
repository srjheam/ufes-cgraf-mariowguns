#ifndef ENTITY_H
#define ENTITY_H

#include <GL/gl.h>

#include <memory>

class Entity {
  private:
    class Impl;
    std::unique_ptr<Impl> pimpl;

  public:
    const GLfloat &o_x() const;
    void o_x(const GLfloat &o_x) const;

    const GLfloat &o_y() const;
    void o_y(const GLfloat &o_y) const;

    const int &height() const;
    const int &width() const;

    const GLdouble &ttl() const;
    void ttl(const GLdouble &ttl) const;

    const bool &hidden() const;
    void hidden(const bool &hidden) const;

    const GLfloat &vector_velocity() const;
    void vector_velocity(const GLfloat &velocity) const;

    const GLfloat &vector_direction_x() const;
    const GLfloat &vector_direction_y() const;
    //! Does not change the velocity
    void vector_set_direction(const GLfloat &dx, const GLfloat &dy) const;
    
    void vector_set_zero() const;
    //! Does change the velocity
    void vector_set(const GLfloat &dx, const GLfloat &dy) const;
    //! Vector must be normalized
    void vector_set(const GLfloat &dx, const GLfloat &dy, const GLfloat &velocity) const;
    //! Vector must be normalized
    void vector_sum(const GLfloat &vx, const GLfloat &vy, const GLfloat &velocity) const;

    GLfloat vector_get_dx_dt(const GLfloat &dt) const;
    GLfloat vector_get_dy_dt(const GLfloat &dt) const;

    //! Only for entities who are not overlapping at all
    GLfloat distanceof_x(const Entity &entity) const;
    //! Only for entities who are not overlapping at all
    GLfloat distanceof_y(const Entity &entity) const;

    //! Only for instances that are completely inside param entity
    GLfloat distanceinsideof_x(const Entity &entity) const;
    //! Only for instances that are completely inside param entity
    GLfloat distanceinsideof_y(const Entity &entity) const;

    Entity(GLfloat o_x, GLfloat o_y, int height, int width);

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    
    Entity(Entity&&) noexcept;
    Entity& operator=(Entity&&) noexcept = default;

    virtual ~Entity();

    virtual void draw() const = 0;
};

#endif
