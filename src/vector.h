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
        Vector(const Vector&);
        Vector& operator=(const Vector&);
        Vector(Vector&&) noexcept;
        Vector& operator=(Vector&&) noexcept;
        ~Vector();

        const float& direction_x() const;
        void set_direction_x(const float &dx);
        const float& direction_y() const;
        void set_direction_y(const float &dy);

        //! Does not change the velocity
        void set_direction(const float &dx, const float &dy);

        const float& velocity() const;
        void set_velocity(const float &velocity);

        void set_vector(const float &dx, const float &dy) const;

        void set_zero() const;

        //! Vector must be normalized
        void sum(const float &dx, const float &dy, const float &velocity);

        float calc_dx_dt(const float &dt) const;
        float calc_dy_dt(const float &dt) const;
};


#endif
