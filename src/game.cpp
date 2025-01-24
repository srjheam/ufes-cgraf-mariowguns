#include "game.h"

#include "tinyxml2.h"

Game::Game(int windowSideLength) {
    _windowSideLength = windowSideLength;
    _arena = Arena(windowSideLength);
}

const int &Game::windowSideLength() const { return _windowSideLength; }

bool Game::loadArena(const char *path) {
    tinyxml2::XMLDocument doc;
    // Load the SVG file
    tinyxml2::XMLError result = doc.LoadFile(path);

    // Check if file loaded successfully
    if (result != tinyxml2::XML_SUCCESS) {
        return false;
    }

    return _arena.loadFrom(doc);
}
