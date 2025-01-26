#include "game.h"

#include <iostream>
#include <limits>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "srutils.h"

#include "tinyxml2.h"

class Game::Impl {
  public:
    size_t windowSideLength_;
    bool keyboardKeyPressed_[256] = {0};

    GLfloat gravity_px_s_ = 294.0f;
    GLfloat characters_speed_px_s_ = 336.0f;
    GLfloat characters_jump_px_s_ = 630.0f;

    Arena *arena_;

    Impl(size_t windowSideLength)
        : windowSideLength_(windowSideLength),
            arena_(new Arena(windowSideLength)) { }

    ~Impl() { delete arena_; }

    bool moveCharacterAndCheckCollisions(const Character &character, GLdouble dt) {
        auto min_deltas_check_all_zero = [&character](const auto &entities, GLfloat &dx, GLfloat &dy) {
            for (auto &entity : entities) {                
                if (dynamic_cast<const Entity*>(&entity) == dynamic_cast<const Entity*>(&character)) continue;

                if (!character.aabb_isoverlapping_delta(entity, dx, dy))
                    continue;

                //
                // BUG:
                //   Se um character bater na quina de uma platform (leia-se, se
                //   uma entity qualquer se deslocar diagonalmente em direção a
                //   um eixo de outra entidade qualquer), por algum motivo ela
                //   fica garrada lá até haver um vetor de força horizontalmente
                //   contrario a entidade.
                //
                //   att: 2025-01-26
                //     Antes, as funcoes aabb_distanceof_x e aabb_distanceof_y retornavam 0
                //     quando havia overlapping (justamente o caso da quina), quanto
                //     removida essa condicao, o personagem nao fica mais garrado
                //     mas sim atravessa a plataforma.
                //

                if (character.aabb_isoverlapping_dx(entity, dx))
                    dx = srutils::minabsf(dx, character.aabb_distanceof_x(entity));
                    //dx = .0f;
                if (character.aabb_isoverlapping_dy(entity, dy))
                    dy = srutils::minabsf(dy, character.aabb_distanceof_y(entity));
                    //dy = .0f;
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
        auto &wall = arena_->background();
        if (!character.aabb_isinsideof_dx(wall, dx))
            dx = srutils::minabsf(dx, character.aabb_insideof_x(wall));
            //dx = .0f;
        if (!character.aabb_isinsideof_dy(wall, dy))
            dy = srutils::minabsf(dy, character.aabb_insideof_y(wall));
            //dy = .0f;

        character.o_x(character.o_x() + dx);
        character.o_y(character.o_y() + dy);

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

    float factor = pimpl->arena_->loadFrom(doc);
    if (factor == .0f)
        return false;

    // set factors
    //pimpl->gravity_px_s_ *= factor;
    //pimpl->characters_speed_px_s_ *= factor;
    //pimpl->characters_jump_px_s_ *= factor;

    for (const auto &foe : pimpl->arena_->foes()) {
        foe.vector_set_direction(1.0f, 0.0f);
        foe.vector_velocity(pimpl->characters_speed_px_s_);
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
        player.vector_sum(-1.0f, 0.0f, pimpl->characters_speed_px_s_);
    } else if (pimpl->keyboardKeyPressed_['d'] || pimpl->keyboardKeyPressed_['D']) {
        player.vector_sum(1.0f, 0.0f, pimpl->characters_speed_px_s_);
    }

    if (pimpl->keyboardKeyPressed_['w'] || pimpl->keyboardKeyPressed_['W']) {
        player.vector_sum(0.0f, 1.0f, pimpl->characters_jump_px_s_);
    }

    // gravity
    player.vector_sum(0.0f, -1.0f, pimpl->gravity_px_s_);

    pimpl->moveCharacterAndCheckCollisions(player, dt);
    player.vector_set_zero();

    glutPostRedisplay();
}

void Game::mouse(int button, int state, int x, int y) {
    
}

void Game::motion(int x, int y) {
    
}
