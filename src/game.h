#ifndef GAME_H
#define GAME_H

#include <memory>

#include "arena.h"

class Game {
  private:
    // PIMPL idiom
    class Impl;
    std::unique_ptr<Impl> pimpl;

  public:
    int windowSideLength() const;

    Game(size_t windowSideLength);
    ~Game();

    bool loadArena(const char *path);

    void display(void);
    void keyboard(unsigned char key, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
    void idle();
    void mouse(int button, int state, int x, int y);
    void motion(int x, int y);
};

#endif