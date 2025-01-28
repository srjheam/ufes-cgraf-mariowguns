#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>

#include <GL/gl.h>

#include "entity.h"
#include "color_rgb.h"
#include "bullet.h"

enum CharacterDirection { RIGHT, LEFT };

class Character : public Entity {
  private:
    class Impl;
    std::unique_ptr<Impl> pimpl;

  public:
    Character(GLfloat o_x, GLfloat o_y, int height, int width, ColorRgb body);
    
    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
    
    Character(Character&&) noexcept;
    Character& operator=(Character&&) noexcept = default;
    
    ~Character() override;

    const enum CharacterDirection &direction() const;
    void direction(const enum CharacterDirection &direction) const;

    void movement_translate(GLfloat dx, GLfloat dy) const override;

    const bool &jumping() const;
    void jumping(const bool &direction) const;

    bool jump_can_they() const;
    void jump_start() const;
    void jump_end() const;

    bool shot_add_dt(GLdouble dt) const;
    void aim(GLfloat dx, GLfloat dy) const;
    Bullet shoot() const;

    void draw() const;
};

#endif
