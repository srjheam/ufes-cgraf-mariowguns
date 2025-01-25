#ifndef SRUTILS_H
#define SRUTILS_H

#include <cmath>

namespace srutils
{
   inline float normalizef(const float &dx, const float &dy)
   {
      return sqrtf(dx * dx + dy * dy);
   }

   inline float distance(const float &x1, const float &x2)
   {
      return fabs(x1 - x2);
   }

   //! Returns the minimum absolute of two floats with the same sign, when the signs are different, returns the first float.
   inline float mineqsignf(const float &a, const float &b)
   {
      return a * b <= 0 // when a * b > 0, the signs are the same
         ? a
         : fabs(a) - fabs(b) < 0
            ? a
            : b;
   }
}

#endif

