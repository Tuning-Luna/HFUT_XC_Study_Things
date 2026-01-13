/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Library General Public License as published   *
 *  by the Free Software Foundation, version 2.                              *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Library General Public License for more details.                         *
 *                                                                           *
 *  You should have received a copy of the GNU Library General Public        *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/

#ifndef MATHDEFS_HH
#define MATHDEFS_HH

#include <math.h>
//#include <float.h>

#ifndef M_PI
  #define M_PI      3.14159265359
#endif

namespace OpenMesh
{

/** comparison operators with user-selected precision control
*/
template <class T, typename Real>
inline bool                             is_zero(const T& _a, Real _eps)
{
  return fabs(_a) < _eps;
}

template <class T1, class T2, typename Real>
inline bool                             is_eq(const T1& a, const T2& b, Real _eps)
{
  return is_zero(a-b, _eps);
}

template <class T1, class T2, typename Real>
inline bool                             is_gt(const T1& a, const T2& b, Real _eps)
{
  return (a > b) && !is_eq(a,b,_eps);
}

template <class T1, class T2, typename Real>
inline bool                             is_ge(const T1& a, const T2& b, Real _eps)
{
  return (a > b) || is_eq(a,b,_eps);
}

template <class T1, class T2, typename Real>
inline bool                             is_lt(const T1& a, const T2& b, Real _eps)
{
  return (a < b) && !is_eq(a,b,_eps);
}

template <class T1, class T2, typename Real>
inline bool                             is_le(const T1& a, const T2& b, Real _eps)
{
  return (a < b) || is_eq(a,b,_eps);
}

/** comparison operators with automatic precision control
    depends on the floating point precision of the arguments
    NOTE: __flt_eps and __dbl_eps might be replaced later with
    arch-dependent values taken from float.h
*/

const double __flt_eps = 1e-5;
const double __dbl_eps = 1e-11;

inline bool                             is_zero(const float a)
{
  return is_zero(a, __flt_eps);
}

inline bool                             is_zero(const double a)
{
  return is_zero(a, __dbl_eps);
}

template <class T1, class T2>
inline bool                             is_eq(const T1& a, const T2& b)
{
  return is_zero(a-b);
}

template <class T1, class T2>
inline bool                             is_gt(const T1& a, const T2& b)
{
  return (a > b) && !is_eq(a,b);
}

template <class T1, class T2>
inline bool                             is_ge(const T1& a, const T2& b)
{
  return (a > b) || is_eq(a,b);
}

template <class T1, class T2>
inline bool                             is_lt(const T1& a, const T2& b)
{
  return (a < b) && !is_eq(a,b);
}

template <class T1, class T2>
inline bool                             is_le(const T1& a, const T2& b)
{
  return (a < b) || is_eq(a,b);
}

/// Trigonometry/angles - related

template <class T>
T                                      sane_aarg(T _aarg)
{
  if (_aarg < -1)
  {
    _aarg = -1;
  }
  else if (_aarg >  1)
  {
    _aarg = 1;
  }
  return _aarg;
}

/** returns the angle determined by its cos and the sign of its sin
    result is positive if the angle is in [0:pi]
    and negative if it is in [pi:2pi]
*/
template <class T>
T                                           angle(T _cos_angle, T _sin_angle)
{//sanity checks - otherwise acos will return nan
  _cos_angle = sane_aarg(_cos_angle);
  return (T) _sin_angle >= 0 ? acos(_cos_angle) : -acos(_cos_angle);
}

template <class T>
inline T                               positive_angle(T _angle)
{
  return _angle < 0 ? (2*M_PI + _angle) : _angle;
}

template <class T>
inline T                               positive_angle(T _cos_angle, T _sin_angle)
{
  return positive_angle(angle(_cos_angle, _sin_angle));
}

template <class T>
inline T                               deg_to_rad(const T& _angle)
{
  return M_PI*(_angle/180);
}

template <class T>
inline T                               rad_to_deg(const T& _angle)
{
  return 180*(_angle/M_PI);
}

};//namespace OpenMesh

#endif//MATHDEFS_HH
