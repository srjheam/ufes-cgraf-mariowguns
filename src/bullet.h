#ifndef BULLET_H_
#define BULLET_H_

#include <memory>

#include "entity.h"
#include "vector.h"

class Bullet : public Entity {
  private:
    class Impl;
    std::unique_ptr<Impl> pimpl;

  public:
    Bullet(GLfloat o_x, GLfloat o_y, GLfloat height, GLfloat width, Vector direction, GLdouble ttl);

    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
    
    Bullet(Bullet&&) noexcept;
    Bullet& operator=(Bullet&&) noexcept = default;

    ~Bullet();

    const Vector &direction() const;

    void draw() const;
};

#endif // BULLET_H_
