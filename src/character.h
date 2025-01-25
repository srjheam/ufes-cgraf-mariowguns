#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>

#include <GL/gl.h>

#include "entity.h"
#include "color_rgb.h"

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
    
    virtual ~Character() override;

    const enum CharacterDirection &direction() const;
    void direction(const enum CharacterDirection &direction);

    const GLfloat &speed_px_per_second() const;

    void draw() const;
};

#endif
