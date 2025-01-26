#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <GL/gl.h>

#include "entity.h"
#include "color_rgb.h"

class Platform : public Entity {
  private:
    ColorRgb _background;

  public:
    Platform(GLfloat o_x, GLfloat o_y, int height, int width, ColorRgb background);
    
    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;
    
    Platform(Platform&&) noexcept = default;
    Platform& operator=(Platform&&) noexcept = default;

    virtual ~Platform() override = default;

    void draw() const;
};

#endif
