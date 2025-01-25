#include "color_rgb.h"

ColorRgb::ColorRgb(uint8_t red, uint8_t green, uint8_t blue) {
    _red = red;
    _green = green;
    _blue = blue;
}

ColorRgb::ColorRgb(GLclampf red, GLclampf green, GLclampf blue) {
    _red = static_cast<uint8_t>(red * 255.0f);
    _green = static_cast<uint8_t>(green * 255.0f);
    _blue = static_cast<uint8_t>(blue * 255.0f);
}

uint8_t ColorRgb::red() const { return _red; }
void ColorRgb::red(uint8_t red) { _red = red; }

uint8_t ColorRgb::green() const { return _green; }
void ColorRgb::green(uint8_t green) { _green = green; }

uint8_t ColorRgb::blue() const { return _blue; }
void ColorRgb::blue(uint8_t blue) { _blue = blue; }

GLclampf ColorRgb::gl_red() const {
    return _red / 255.0f;
}

GLclampf ColorRgb::gl_green() const {
    return _green / 255.0f;
}

GLclampf ColorRgb::gl_blue() const {
    return _blue / 255.0f;
}
