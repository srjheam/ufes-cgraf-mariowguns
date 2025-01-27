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
    Bullet(GLfloat o_x, GLfloat o_y, Vector direction);
    Bullet(Bullet&&) noexcept;
    ~Bullet();

    void draw() const;
};

#endif // BULLET_H_
