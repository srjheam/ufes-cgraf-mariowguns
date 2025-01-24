#ifndef GAME_H
#define GAME_H

#include "arena.h"

class Game {
  private:
    int _windowSideLength;

    Arena _arena;

  public:
    const int &windowSideLength() const;

    Game(int windowSideLength);
    bool loadArena(const char *path);
};

#endif