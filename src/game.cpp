#include "game.h"

#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "srutils.h"

#include "tinyxml2.h"

class Game::Impl {
  public:
    size_t windowSideLength_;
    bool keyboardKeyPressed_[256] = {0};

    Arena *arena_;

    Impl(size_t windowSideLength)
        : windowSideLength_(windowSideLength), arena_(new Arena(windowSideLength)) { }

    ~Impl() { delete arena_; }

    GLfloat get_gravity_dt(GLdouble dt) {
        return 9.8f * (dt / 1000.0f);
    }

    bool moveCharacterAndCheckCollisions(const Character &character, GLdouble dt) {
        auto min_deltas_check_all_zero = [&character](const auto &entities, GLfloat &dx, GLfloat &dy) {
            for (auto &entity : entities) {                
                if (dynamic_cast<const Entity*>(&entity) == dynamic_cast<const Entity*>(&character)) continue;

                dx = srutils::mineqsignf(dx, character.distanceof_x(entity));
                dy = srutils::mineqsignf(dy, character.distanceof_y(entity));
            }

            if (dx == .0f && dy == .0f)
                return true;

            return false;
        };

        GLfloat seconds = dt / 1000.0f;
        
        GLfloat dx = character.vector_get_dx_dt(seconds);
        GLfloat dy = character.vector_get_dy_dt(seconds);

        // colision entity
        if (min_deltas_check_all_zero(arena_->platforms(), dx, dy)
            || min_deltas_check_all_zero(arena_->foes(), dx, dy)
            || min_deltas_check_all_zero(arena_->players(), dx, dy))
            return true;

        // colision walls
        dx = srutils::mineqsignf(
            dx,
            character.distanceinsideof_x(arena_->background())
        );
        dy = srutils::mineqsignf(
            dy,
            character.distanceinsideof_y(arena_->background())
        );

        std::cout << "Character: " << std::endl;
        std::cout << "o_x: " << character.o_x() << " o_y: " << character.o_y() << std::endl;
        std::cout << "dx: " << dx << " dy: " << dy << std::endl << std::endl;

        character.o_x(character.o_x() + dx);
        character.o_y(character.o_y() + dy);

        character.vector_set(dx, dy);

        return dx != 0.0f || dy != 0.0f;
    }
};

Game::Game(size_t windowSideLength)
    : pimpl(new Impl(windowSideLength)) { }

Game::~Game() = default;

int Game::windowSideLength() const { return pimpl->windowSideLength_; }

bool Game::loadArena(const char *path) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError fileLoaded = doc.LoadFile(path);

    if (fileLoaded != tinyxml2::XML_SUCCESS) {
        return false;
    }

    bool arenaLoaded = pimpl->arena_->loadFrom(doc);
    if (!arenaLoaded)
        return false;

    for (const auto &foe : pimpl->arena_->foes()) {
        foe.vector_set_direction(1.0f, 0.0f);
        foe.vector_velocity(foe.speed_px_per_second());
    }

    return true;
}

void Game::display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // a model-view está ativa

    glLoadIdentity();

    // mover "camera"
    float playerCenterX = pimpl->arena_->player().o_x() + pimpl->arena_->player().width()/2.0f;
    float offsetX = windowSideLength()/2.0f - playerCenterX;
    glTranslatef(offsetX, 0, 0);

    pimpl->arena_->draw();

    glutSwapBuffers();
}

void Game::keyboard(unsigned char key, int x, int y) {
    if (x) {
    }
    if (y) {
    }

    pimpl->keyboardKeyPressed_[key] = true;

    glutPostRedisplay();
}

void Game::keyboardUp(unsigned char key, int x, int y) {
    if (x) {
    }
    if (y) {
    }

    pimpl->keyboardKeyPressed_[key] = false;

    glutPostRedisplay();
}

void Game::idle() {
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);

    GLdouble currentTime = glutGet(GLUT_ELAPSED_TIME);

    // interval in milliseconds since last idle callback
    GLdouble dt = currentTime - previousTime;
    previousTime = currentTime;

    auto &player = pimpl->arena_->player();
    if (pimpl->keyboardKeyPressed_['a'] || pimpl->keyboardKeyPressed_['A']) {
        player.vector_sum(-1.0f, 0.0f, player.speed_px_per_second());
    } else if (pimpl->keyboardKeyPressed_['d'] || pimpl->keyboardKeyPressed_['D']) {
        player.vector_sum(1.0f, 0.0f, player.speed_px_per_second());
    }

    if (pimpl->keyboardKeyPressed_['w'] || pimpl->keyboardKeyPressed_['W']) {
        player.vector_sum(0.0f, 1.0f, player.speed_px_per_second());
    }

    // gravity
    player.vector_sum(0.0f, -1.0f, pimpl->get_gravity_dt(dt));

    std::cout << "MOVENDO PLAYER" << dt << std::endl;
    std::cout << "Player: " << std::endl;
    std::cout << "o_x: " << player.o_x() << " o_y: " << player.o_y() << std::endl;
    pimpl->moveCharacterAndCheckCollisions(player, dt);
    std::cout << "FIM MOVIMENTO PLAYER" << std::endl << std::endl;

    glutPostRedisplay();
}

void Game::mouse(int button, int state, int x, int y) {
    
}

void Game::motion(int x, int y) {
    
}
