#include "game.h"

#include <iostream>
#include <limits>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "srutils.h"

#include "tinyxml2.h"

enum GameStatus { RUNNING, WON, OVER };

class Game::Impl {
  public:
    const char *path_;
    size_t windowSideLength_;
    bool keyboardKeyPressed_[256] = {0};
    bool mouseButtonPressed_[3] = {0};

    const GLfloat gravity_px_s_ = 42.f + 28.f; // 294.f;
    const GLfloat characters_speed_px_s_ = srutils::CHARACTER_SPEED_PX_S;
    const GLfloat characters_jump_px_s_ = 126.f + 28.f; // 630.f;

    GameStatus status_ = GameStatus::RUNNING;

    Arena *arena_;

    Impl(size_t windowSideLength)
        : path_(nullptr), windowSideLength_(windowSideLength),
            arena_(new Arena(windowSideLength)) { }

    ~Impl() { delete arena_; }

    void setUp() {
        // set factors
        //pimpl->gravity_px_s_ *= factor;
        //pimpl->characters_speed_px_s_ *= factor;
        //pimpl->characters_jump_px_s_ *= factor;

        for (const auto &foe : arena_->foes())
            foe.vector_sum(1.0f, 0.0f, characters_speed_px_s_);
    }

    void reset() {
        auto *narena = new Arena(windowSideLength_);
        narena->loadFrom(path_);

        auto *oarena = arena_;
        arena_ = narena;

        delete oarena;

        for (auto &key : keyboardKeyPressed_)
            key = false;
        
        for (auto &button : mouseButtonPressed_)
            button = false;

        setUp();
    }

    void init(void) {
        /* selecionar cor de fundo (azul rgb(158 158 255)) */
        glClearColor(0.620f, 0.620f, 1.0f, 1.0f);

        /* inicializar sistema de visualizacao */
        glMatrixMode(GL_PROJECTION);
        glOrtho(0.0, windowSideLength_, 0.0, windowSideLength_, -100.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    bool checkEndGame() {
        if (keyboardKeyPressed_['r'] || keyboardKeyPressed_['R']) {
            status_ = GameStatus::RUNNING;

            reset();

            return true;
        }

        if (status_ == GameStatus::WON)
            return true;

        return false;
    }

    bool moveCharacterAndCheckCollisions(const Character &character, GLdouble dt) {
        character.colisions_reset();

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

                if (character.aabb_isoverlapping_dx(entity, dx)) {
                    character.colisions_set_last_x(entity);
                    //character.jump_end();
                    dx = srutils::minabsf(dx, character.aabb_distanceof_x(entity));
                    //dx = .0f;
                }
                if (character.aabb_isoverlapping_dy(entity, dy)) {
                    character.colisions_set_last_y(entity);
                    character.jump_end();
                    dy = srutils::minabsf(dy, character.aabb_distanceof_y(entity));
                    //dy = .0f;
                }
            }

            if (dx == .0f && dy == .0f)
                return true;

            return false;
        };

        GLfloat seconds = dt / 1000.0f;
        
        GLfloat dx = character.vector_current().calc_dx_dt(seconds);
        GLfloat dy = character.vector_current().calc_dy_dt(seconds);

        // colision entity
        min_deltas_check_all_zero(arena_->platforms(), dx, dy);
        min_deltas_check_all_zero(arena_->foes(), dx, dy);
        min_deltas_check_all_zero(arena_->players(), dx, dy);

        // colision walls
        auto &wall = arena_->background();
        if (!character.aabb_isinsideof_dx(wall, dx)) {
            character.colisions_set_last_x(wall);
            //character.jump_end();
            dx = srutils::minabsf(dx, character.aabb_insideof_x(wall, dx));
            //dx = .0f;
        }
        if (!character.aabb_isinsideof_dy(wall, dy)) {
            character.colisions_set_last_y(wall);
            character.jump_end();
            dy = srutils::minabsf(dy, character.aabb_insideof_y(wall, dy));
            //dy = .0f;
        }

        character.movement_translate(dx, dy);

        return dx != 0.0f || dy != 0.0f;
    }

    void movePlayer(GLdouble dt) {
        // set up player movement vector
        const auto &player = arena_->player();
        if (keyboardKeyPressed_['a'] || keyboardKeyPressed_['A'])
            player.vector_sum(-1.0f, 0.0f, characters_speed_px_s_);
        else if (keyboardKeyPressed_['d'] || keyboardKeyPressed_['D'])
            player.vector_sum(1.0f, 0.0f, characters_speed_px_s_);
        
        if (mouseButtonPressed_[GLUT_RIGHT_BUTTON]) {
            if (player.jump_can_they()) {
                if (!player.jumping())
                    player.jump_start();

                player.vector_sum(0.0f, 1.0f, characters_jump_px_s_);
            }
        } else {
            player.jump_end();
        }

        // set up player gravity
        player.vector_sum(0.0f, -1.0f, gravity_px_s_);

        if (keyboardKeyPressed_['i'] || keyboardKeyPressed_['I']) {
            player.vector_current().set_zero();
            player.vector_sum(0.0f, 1.0f, characters_speed_px_s_ * 5);
        }
        else if (keyboardKeyPressed_['k'] || keyboardKeyPressed_['K']) {
            player.vector_current().set_zero();
            player.vector_sum(0.0f, -1.0f, characters_speed_px_s_ * 5);
        }
        else if (keyboardKeyPressed_['j'] || keyboardKeyPressed_['J']) {
            player.vector_current().set_zero();
            player.vector_sum(-1.0f, 0.0f, characters_speed_px_s_ * 5);
        }
        else if (keyboardKeyPressed_['l'] || keyboardKeyPressed_['L']) {
            player.vector_current().set_zero();
            player.vector_sum(1.0f, 0.0f, characters_speed_px_s_ * 5);
        }

        moveCharacterAndCheckCollisions(player, dt);
        player.vector_save_current_set_zero();

        if (player.colisions_last_right() == &arena_->background())
            status_ = GameStatus::WON;
    }

    void moveFoes(GLdouble dt) {
        for (const auto &foe : arena_->foes()) {
            if (foe.direction() == CharacterDirection::LEFT) {
                foe.vector_sum(-1.0f, 0.0f, characters_speed_px_s_);
            } else {
                foe.vector_sum(1.0f, 0.0f, characters_speed_px_s_);
            }

            // set up player gravity
            foe.vector_sum(0.0f, -1.0f, gravity_px_s_);

            if (foe.colisions_last_bottom() != nullptr && foe.colisions_last_bottom() != &arena_->background()) {
                auto dx = foe.vector_current().calc_dx_dt(dt / 1000.0f);
                auto dy = foe.vector_current().calc_dy_dt(dt / 1000.0f);

                auto platform = foe.colisions_last_bottom();

                auto max_mov = foe.aabb_insideof_x(*platform, dx);
                if (fabsf(max_mov) <= fabs(dx)) {
                    dx = max_mov;

                    if (foe.direction() == CharacterDirection::LEFT)
                        foe.direction(CharacterDirection::RIGHT);
                    else
                        foe.direction(CharacterDirection::LEFT);
                    foe.vector_current().set_vector(dx, dy);
                }

            }

            moveCharacterAndCheckCollisions(foe, dt);
            foe.vector_save_current_set_zero();

            if (foe.direction() == CharacterDirection::LEFT && foe.colisions_last_left() != nullptr)
                foe.direction(CharacterDirection::RIGHT);
            else if (foe.direction() == CharacterDirection::RIGHT && foe.colisions_last_right() != nullptr)
                foe.direction(CharacterDirection::LEFT);
        }
    }

    void drawInfo() {
        glLoadIdentity();
    
        glColor3f(.059f, .741f, .059f); // rgb(15, 189, 15)

        glRasterPos2f(windowSideLength_ - 165, windowSideLength_ - 25);
        const char* tmove = "Press A, D to move";
        for (const char* c = tmove; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        glRasterPos2f(windowSideLength_ - 165, windowSideLength_ - 50);
        const char* tjump = "Press RMB to jump";
        for (const char* c = tjump; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);        

        glRasterPos2f(windowSideLength_ - 150, windowSideLength_ - 75);
        const char* trestart = "Press R to restart";
        for (const char* c = trestart; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
};

Game::Game(size_t windowSideLength)
    : pimpl(new Impl(windowSideLength)) { }

Game::~Game() = default;

bool Game::loadArena(const char *path) {
    pimpl->path_ = path;

    float factor = pimpl->arena_->loadFrom(path);
    if (factor == .0f)
        return false;

    return true;
}

bool Game::start() {
    pimpl->init();

    pimpl->setUp();

    return true;
}

void Game::display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // a model-view está ativa

    glLoadIdentity();

    if (pimpl->status_ == GameStatus::WON) {
        glColor3f(.059f, .741f, .059f); // rgb(15, 189, 15)
        glRasterPos2f(pimpl->windowSideLength_/2.0f - 40, pimpl->windowSideLength_/2.0f);
        const char* twink = "You win!";
        for (const char* c = twink; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        glColor3f(1.f, 1.f, 1.f); // rgb(255, 255, 255)
        glRasterPos2f(pimpl->windowSideLength_/2.0f - 60, 10);
        const char* tr = "Press R to restart";
        for (const char* c = tr; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        glutSwapBuffers();

        return;
    }

    // mover "camera"
    float playerCenterX = pimpl->arena_->player().o_x() + pimpl->arena_->player().width()/2.0f;
    float offsetX = pimpl->windowSideLength_/2.0f - playerCenterX;
    glTranslatef(offsetX, 0, 0);

    pimpl->arena_->draw();

    pimpl->drawInfo();

    glutSwapBuffers();
}

void Game::keyboard(unsigned char key, int x, int y) {
    if (x) { }
    if (y) { }

    pimpl->keyboardKeyPressed_[key] = true;
}

void Game::keyboardUp(unsigned char key, int x, int y) {
    if (x) { }
    if (y) { }

    pimpl->keyboardKeyPressed_[key] = false;
}

void Game::idle() {
    if (pimpl->checkEndGame())
        return;

    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);

    GLdouble currentTime = glutGet(GLUT_ELAPSED_TIME);

    // interval in milliseconds since last idle callback
    GLdouble dt = currentTime - previousTime;
    previousTime = currentTime;

    pimpl->movePlayer(dt);

    pimpl->moveFoes(dt);

    glutPostRedisplay();
}

void Game::mouse(int button, int state, int x, int y) {
    if (x) { }
    if (y) { }

    pimpl->mouseButtonPressed_[button] = state == GLUT_DOWN;
}

void Game::motion(int x, int y) {
    auto gl_x = x;
    auto gl_y = pimpl->windowSideLength_ - y;

    auto player_x = pimpl->windowSideLength_ / 2.0f;
    auto player_y = pimpl->arena_->player().height() / 2.0f + pimpl->arena_->player().o_y();

    pimpl->arena_->player().aim(gl_x - player_x, gl_y - player_y);

    glutPostRedisplay();
}
