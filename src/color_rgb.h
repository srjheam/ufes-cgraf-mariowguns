#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include <GL/gl.h>

class ColorRgb {
  private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;

  public:
    uint8_t red() const;
    void red(uint8_t red);
    uint8_t green() const;
    void green(uint8_t green);
    uint8_t blue() const;
    void blue(uint8_t blue);

    GLclampf gl_red() const;
    GLclampf gl_green() const;
    GLclampf gl_blue() const;

    ColorRgb(unsigned char red, unsigned char green, unsigned char blue);
    ColorRgb(GLclampf red, GLclampf green, GLclampf blue);
};

#endif
