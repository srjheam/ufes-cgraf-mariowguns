#include "character.h"

#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <algorithm>

#include "srutils.h"

class Character::Impl {
  public:
    ColorRgb body_;

    float currJumpYAcc = .0f;
    bool jumping = false;


    GLdouble sinceLastShotDt = .0f;

    //! [-45.0, +45.0]
    GLdouble gunAngleDeg = 0.0f;

    CharacterDirection direction_ = RIGHT;

    Impl(ColorRgb body) : body_(body) { }
    ~Impl() = default;

    void draw_legs(const Character &ref) const {
        glColor3f(1.0f, .0f, .0f); // rgb(255, 0, 0)

        auto legsWidth = 2 * (ref.width() / 3);
        auto legsGap = ref.width() / 12;
        auto legWidth = legsWidth / 2 - legsGap / 2;
        auto legsHeight = (2 * ref.height() / 5);
        auto legsX0 = ref.o_x() + (ref.width() - legsWidth) / 2;
        auto legsY0 = ref.o_y();

        glBegin(GL_QUADS);
            glVertex2f(legsX0, legsY0);
            glVertex2f(legsX0 + legWidth, legsY0);
            glVertex2f(legsX0 + legWidth, legsY0 + legsHeight);
            glVertex2f(legsX0, legsY0 + legsHeight);
        glEnd();

        glBegin(GL_QUADS);
            glVertex2f(legsX0 + legsWidth - legWidth, legsY0);
            glVertex2f(legsX0 + legsWidth, legsY0);
            glVertex2f(legsX0 + legsWidth, legsY0 + legsHeight);
            glVertex2f(legsX0 + legsWidth - legWidth, legsY0 + legsHeight);
        glEnd();

        // se eu pudesse eu anexava um desenho aqui com a ilustração dessa bomba

        /* const auto legs_angle = 45.0f;

        auto legs_height = 2 * ref.height() / 5;
        auto leg_one_width = ref.width() / 4;

        auto leg_x0 = ref.o_x() + ref.width() / 2 - leg_one_width / 2;
        auto leg_y0 = ref.o_y() + legs_height;

        auto leg_joint_y = leg_y0 - ((legs_height / 2) / srutils::COS_45) * .8f;
        auto leg_joint_length = (legs_height / 2) * srutils::TG_45;

        auto leg_foot_y = ref.o_y();
        auto leg_foot_length = legs_height * srutils::TG_45;

        auto calc_harmony = [&ref](float dpi) {
            return sinf(((ref.o_x() / srutils::CHARACTER_SPEED_PX_S) + dpi) * M_PI);
        };

        auto jointa_harmony = calc_harmony(0.0f);
        auto jointa_x = jointa_harmony * leg_joint_length;
        auto jointa_dx = cosf(90 - legs_angle * jointa_harmony) * leg_one_width / 2;
        auto jointa_dy = sinf(90 - legs_angle * jointa_harmony) * leg_one_width / 2;

        glBegin(GL_QUADS);
            glVertex2f(leg_x0 - jointa_dx, leg_y0 - jointa_dy);
            glVertex2f(leg_x0 + jointa_x - jointa_dx, leg_y0 - leg_joint_y - jointa_dy);
            glVertex2f(leg_x0 + jointa_x + jointa_dx, leg_y0 - leg_joint_y + jointa_dy);
            glVertex2f(leg_x0 + jointa_dx, leg_y0 + jointa_dy);
        glEnd();

        auto jointb_harmony = calc_harmony(0.5f);
        auto jointb_x = jointb_harmony * leg_joint_length;
        auto jointb_dx = cosf(90 - legs_angle * jointb_harmony) * leg_one_width / 2;
        auto jointb_dy = sinf(90 - legs_angle * jointb_harmony) * leg_one_width / 2;

        glBegin(GL_QUADS);
            glVertex2f(leg_x0 - jointb_dx, leg_y0 - jointb_dy);
            glVertex2f(leg_x0 + jointb_x - jointb_dx, leg_y0 - leg_joint_y - jointb_dy);
            glVertex2f(leg_x0 + jointb_x + jointb_dx, leg_y0 - leg_joint_y + jointb_dy);
            glVertex2f(leg_x0 + jointb_dx, leg_y0 + jointb_dy);
        glEnd(); */
    }

    void draw_body(const Character &ref) const {
        glColor3f(body_.gl_red(), body_.gl_green(), body_.gl_blue());
        
        auto bodyX0 = ref.o_x();
        auto bodyY0 = ref.o_y() + (2 * ref.height() / 5);
        auto bodyWidth = ref.width();
        auto bodyHeight = 3 * (ref.height() / 10);

        glBegin(GL_QUADS);
            glVertex2f(bodyX0, bodyY0);
            glVertex2f(bodyX0 + bodyWidth, bodyY0);
            glVertex2f(bodyX0 + bodyWidth, bodyY0 + bodyHeight);
            glVertex2f(bodyX0, bodyY0 + bodyHeight);
        glEnd();
    }

    void draw_head(const Character &ref) const {
        glColor3f(body_.gl_red(), body_.gl_green(), body_.gl_blue());

        auto r = 3 * ref.height() / 20;
        auto cx = ref.o_x() + ref.width() / 2;
        auto cy = r + 7 * (ref.height() / 10) + ref.o_y();

        const int num_segments = 50;
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for (int i = 0; i <= num_segments; i++) {
                float theta = 2.0f * M_PI * float(i) / float(num_segments);
                float x = cx + r * cosf(theta);
                float y = cy + r * sinf(theta);
                glVertex2f(x, y);
            }
        glEnd();
    }

    void draw_arm(const Character &ref) const {
        glPushMatrix();
            glTranslatef(ref.o_x() + ref.width() / 2, ref.o_y() + 2 * ref.height() / 5 + 3 * (ref.height() / 20), 0);

            glRotatef(direction_ == CharacterDirection::RIGHT ? gunAngleDeg : 180. - gunAngleDeg, 0, 0, 1);

            glColor3f(0.902f, 1.0f, 0.0f); // rgb(230, 255, 0)

            glBegin(GL_QUADS);
                glVertex2f(0, -ref.height()/20);
                glVertex2f(3 * ref.width() / 4, -ref.height()/20); 
                glVertex2f(3 * ref.width() / 4, ref.height()/20);
                glVertex2f(0, ref.height()/20);
            glEnd();
        glPopMatrix();
    }
};

Character::Character(GLfloat o_x, GLfloat o_y, int height, int width, ColorRgb body) 
    : Entity(o_x, o_y, height, width), pimpl(new Impl(body)) {
    
}

Character::Character(Character&&) noexcept = default;

Character::~Character() = default;

const enum CharacterDirection &Character::direction() const {
    return pimpl->direction_;
}

void Character::direction(const enum CharacterDirection &direction) const {
    pimpl->direction_ = direction;
}

void Character::movement_translate(GLfloat dx, GLfloat dy) const {
    if (dx > 0)
        pimpl->direction_ = RIGHT;
    else if (dx < 0)
        pimpl->direction_ = LEFT;

    if (pimpl->jumping) {
        const float maxJumpYAcc = 3 * height();
        dy = srutils::minabseqsignf(dy, maxJumpYAcc - pimpl->currJumpYAcc);
        pimpl->currJumpYAcc += dy;
        pimpl->jumping = pimpl->jumping && (pimpl->currJumpYAcc < maxJumpYAcc);
    }

    Entity::movement_translate(dx, dy);
}

const bool &Character::jumping() const {
    return pimpl->jumping;
}

void Character::jumping(const bool &direction) const {
    pimpl->jumping = direction;
}

bool Character::jump_can_they() const {
    return pimpl->jumping || colisions_last_bottom() != nullptr;
}

void Character::jump_start() const {
    pimpl->currJumpYAcc = 0;
    pimpl->jumping = true;
}

void Character::jump_end() const {
    pimpl->jumping = false;
}

void Character::draw() const {
    //
    // given that a human head is 1/8 of their body height, the body is 3/8 and the legs are 4/8
    // https://www.google.com/search?q=head+proportion+to+body
    //
    // but this proportion is too strange here, so I will swap the body and legs proportions
    //

    if (hidden())
        return;

    pimpl->draw_legs(*this);

    pimpl->draw_body(*this);

    pimpl->draw_head(*this);

    pimpl->draw_arm(*this);
}

bool Character::shot_add_dt(GLdouble dt) const {
    pimpl->sinceLastShotDt += dt;
    return pimpl->sinceLastShotDt > 3.0f * 1000;
}

void Character::aim(GLfloat dx, GLfloat dy) const {
    auto angle = (std::atan2(dy, dx) * 180 / M_PI);
    auto angle2 = (std::atan2(dy, -dx) * 180 / M_PI);

    if (direction() == RIGHT)
        pimpl->gunAngleDeg = std::clamp((angle > -90. && angle < 90.) ? angle : angle2, -45.0, 45.0);
    else
    {
        if (angle > 90.0)
            pimpl->gunAngleDeg = std::clamp(angle2, 0.0, 45.0);
        else if (angle < -90.0)
            pimpl->gunAngleDeg = std::clamp(angle2, -45.0, 0.0);
        else
            pimpl->gunAngleDeg = std::clamp(angle, -45.0, 45.0);
    }
}

Bullet Character::shoot() const {
    pimpl->sinceLastShotDt = 0.0f;

    auto angle = pimpl->gunAngleDeg * M_PI / 180.0;
    auto dx = cos(angle);
    auto dy = sin(angle);

    auto escalar_leave_x = 1;
    if (direction() == LEFT) {
        dx = -dx;
        escalar_leave_x = (-1 * width() + dx) / dx;
    }
    else
        escalar_leave_x = (1 * width() + dx) / dx;

    auto x0 = (o_x() + width() / 2) + dx * escalar_leave_x;
    auto y0 = (o_y() + height() / 2) + dy * escalar_leave_x;
    auto size = height() / 10;
    return Bullet(x0, y0, size, size, Vector(dx, dy), srutils::CHARACTER_SPEED_PX_S * 2);
}
