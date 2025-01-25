#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <GL/gl.h>

#include "entity.h"
#include "color_rgb.h"

class Rectangle : public Entity {
  private:
    ColorRgb _background;

  public:
    Rectangle(GLfloat o_x, GLfloat o_y, int height, int width, ColorRgb background);
    
    Rectangle(const Rectangle&) = delete;
    Rectangle& operator=(const Rectangle&) = delete;
    
    Rectangle(Rectangle&&) noexcept = default;
    Rectangle& operator=(Rectangle&&) noexcept = default;

    virtual ~Rectangle() override = default;

    void draw() const;
};

#endif
