#ifndef ARENA_H
#define ARENA_H

#include <vector>

#include "rectangle.h"
#include "character.h"

#include "tinyxml2.h"

class Arena {
    private:
        int _height;
        int _width;

        Rectangle *_background;
        std::vector<Rectangle> _platforms;
        std::vector<Character> _foes;
        std::vector<Character> _players; // only one player

    public:
        const int &height() const;
        const int &width() const;

        const Rectangle &background() const;
        const std::vector<Rectangle> &platforms() const;
        const std::vector<Character> &foes() const;
        const Character &player() const;
        const std::vector<Character> &players() const; //! only one player

        Arena(int height);
        ~Arena();

        bool loadFrom(const tinyxml2::XMLDocument &doc);

        void draw() const;
};

#endif
