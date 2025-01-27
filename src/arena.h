#ifndef ARENA_H
#define ARENA_H

#include <vector>

#include "platform.h"
#include "character.h"

#include "tinyxml2.h"

class Arena {
    private:
        int _height;
        int _width;

        Platform *_background;
        std::vector<Platform> _platforms;
        std::vector<Character> _foes;
        std::vector<Character> _players; // only one player

    public:
        const int &height() const;
        const int &width() const;

        const Platform &background() const;
        const std::vector<Platform> &platforms() const;
        const std::vector<Character> &foes() const;
        const Character &player() const;
        const std::vector<Character> &players() const; //! only one player

        Arena(int height);
        ~Arena();

        float loadFrom(const char *doc);

        void draw() const;
};

#endif
