#ifndef SRUTILS_H
#define SRUTILS_H

#include <cmath>

namespace srutils
{
   // Isso sem duvida não deveria estar aqui
   // bahianagem total
   const float CHARACTER_SPEED_PX_S = 210.f;

   const float SEN_45 = 0.70710678118f;
   const float COS_45 = 0.70710678118f;
   const float TG_45 = 1.f;

   inline float normalizef(float dx, float dy)
   {
      return sqrtf(dx * dx + dy * dy);
   }

   inline float distance(float x1, float x2)
   {
      return fabs(x1 - x2);
   }

   //! Returns the minimum absolute of two floats with the same sign, when the signs are different, returns the first float.
   inline float minabsf(float a, float b)
   {
      return fabs(a) - fabs(b) < 0
         ? a
         : b;
   }

   inline float minabseqsignf(float a, float b) {
      return a * b <= 0 // when a * b > 0, the signs are the same
      ? a
      : fabs(a) - fabs(b) < 0
         ? a
         : b;
   }

   //
   // Although equalities calculated with epsilons are very important in many
   // cases of floating-point errors, inequalities are not so much and may even
   // provoke errors themselves.
   //
   // I admit that I wrote the following functions fearing that a previous error
   // was caused by a floating-point error. Turns out it was not (as usual).
   //
   // Inequalities with epsilons are not appropriate and I have not removed them
   // because so far no errors have been detected and I am too lazy to remove them.  
   //

   inline float epseq(float a, float b, float eps =  128 * std::numeric_limits<float>::epsilon())
   {
      return fabs(a - b) < eps;
   }

   inline float epslt(float a, float b, float eps =  128 * std::numeric_limits<float>::epsilon())
   {
      return a < b && !epseq(a, b, eps);
   }

   inline float epslte(float a, float b, float eps =  128 * std::numeric_limits<float>::epsilon())
   {
      return a < b || epseq(a, b, eps);
   }

   inline float epsgt(float a, float b, float eps =  128 * std::numeric_limits<float>::epsilon())
   {
      return a > b && !epseq(a, b, eps);
   }

   inline float epsgte(float a, float b, float eps =  128 * std::numeric_limits<float>::epsilon())
   {
      return a > b || epseq(a, b, eps);
   }
}

#endif

