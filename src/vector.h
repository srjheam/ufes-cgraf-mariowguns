#ifndef VECTOR_H
#define VECTOR_H

#include <memory>

class Vector
{
    private:
        // PIMPL idiom
        class Impl;
        std::unique_ptr<Impl> pimpl;

    public:
        Vector();
        Vector(float dx, float dy);
        Vector(const Vector&);
        Vector& operator=(const Vector&);
        Vector(Vector&&) noexcept;
        Vector& operator=(Vector&&) noexcept;
        ~Vector();

        float angle() const;

        float direction_x() const;
        void set_direction_x(float dx);
        float direction_y() const;
        void set_direction_y(float dy);

        //! Does not change the velocity
        void set_direction(float dx, float dy);

        float velocity() const;
        void set_velocity(float velocity);

        void set_vector(float dx, float dy) const;

        void set_zero() const;

        //! Vector must be normalized
        void sum(float dx, float dy, float velocity);

        float calc_dx_dt(float dt) const;
        float calc_dy_dt(float dt) const;
};


#endif
