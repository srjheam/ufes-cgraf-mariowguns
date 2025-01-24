#ifndef ARENA_H
#define ARENA_H

#include "rectangle.h"

#include <vector>

class Arena {
    private:
        int _height;
        int _width;

        Rectangle *_background;
        std::vector<Rectangle> _platforms;
        std::vector<Character> _foes;
        Character *_player;

    public:
        const int &height() const;

        Arena(int height);
        bool loadFrom(const tinyxml2::XMLDocument &doc);
};

#endif
