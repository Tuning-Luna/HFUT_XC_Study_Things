//=============================================================================
//
//                               OpenMesh
//      Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen
//                           www.openmesh.org
//
//-----------------------------------------------------------------------------
//
//                                License
//
//   This library is free software; you can redistribute it and/or modify it
//   under the terms of the GNU Library General Public License as published
//   by the Free Software Foundation, version 2.
//
//   This library is distributed in the hope that it will be useful, but
//   WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   Library General Public License for more details.
//
//   You should have received a copy of the GNU Library General Public
//   License along with this library; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//-----------------------------------------------------------------------------
//
//   $Revision: 1.1.1.1 $
//   $Date: 2004/09/06 12:36:54 $
//
//=============================================================================

/** \file QuadricT.hh

 */

//=============================================================================
//
//  CLASS QuadricT
//
//=============================================================================

#ifndef OPENMESH_GEOMETRY_QUADRIC_HH
#define OPENMESH_GEOMETRY_QUADRIC_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>

//== NAMESPACE ================================================================

namespace OpenMesh { //BEGIN_NS_OPENMESH
namespace Geometry { //BEGIN_NS_GEOMETRY


//== CLASS DEFINITION =========================================================


/** /class QuadricT QuadricT.hh <OSG/Geometry/Types/QuadricT.hh>

    Stores a quadric as a 4x4 symmetrix matrix. Used by the
    error quadric based mesh decimation algorithms.
**/

template <class Scalar>
class QuadricT
{
public:
  typedef Scalar           value_type;
  typedef QuadricT<Scalar> type;
  typedef QuadricT<Scalar> Self;
  //   typedef VectorInterface<Scalar, VecStorage3<Scalar> > Vec3;
  //   typedef VectorInterface<Scalar, VecStorage4<Scalar> > Vec4;
  //typedef Vector3Elem      Vec3;
  //typedef Vector4Elem      Vec4;

  /// construct with upper triangle of symmetrix 4x4 matrix
  QuadricT(Scalar _a, Scalar _b, Scalar _c, Scalar _d,
                      Scalar _e, Scalar _f, Scalar _g,
                                 Scalar _h, Scalar _i,
                                            Scalar _j)
  : a_(_a), b_(_b), c_(_c), d_(_d),
            e_(_e), f_(_f), g_(_g),
                    h_(_h), i_(_i),
                            j_(_j)
  {
  }


  /// constructor from given plane equation: ax+by+cz+d_=0
  QuadricT( Scalar _a=0.0, Scalar _b=0.0, Scalar _c=0.0, Scalar _d=0.0 )
  : a_(_a*_a), b_(_a*_b),  c_(_a*_c),  d_(_a*_d),
               e_(_b*_b),  f_(_b*_c),  g_(_b*_d),
                           h_(_c*_c),  i_(_c*_d),
                                       j_(_d*_d)
  {}

  template <class _Point>
  QuadricT(const _Point& _pt)
  {
    set_distance_to_point(_pt);
  }

  template <class _Normal, class _Point>
  QuadricT(const _Normal& _n, const _Point& _p)
  {
    set_distance_to_plane(_n,_p);
  }

  //set operator
  void set(Scalar _a, Scalar _b, Scalar _c, Scalar _d,
                      Scalar _e, Scalar _f, Scalar _g,
                                 Scalar _h, Scalar _i,
                                            Scalar _j)
  {
    a_ = _a; b_ = _b; c_ = _c; d_ = _d;
             e_ = _e; f_ = _f; g_ = _g;
                      h_ = _h; i_ = _i;
                               j_ = _j;
  }

  //sets the quadric representing the squared distance to _pt
  template <class _Point>
  void set_distance_to_point(const _Point& _pt)
  {
    set(1, 0, 0, -_pt[0],
           1, 0, -_pt[1],
              1, -_pt[2],
                 dot(_pt,_pt));
  }

  //sets the quadric representing the squared distance to the plane [_a,_b,_c,_d]
  void set_distance_to_plane(Scalar _a, Scalar _b, Scalar _c, Scalar _d)
  {
    a_ = _a*_a; b_ = _a*_b; c_ = _a*_c;  d_ = _a*_d;
                e_ = _b*_b; f_ = _b*_c;  g_ = _b*_d;
                            h_ = _c*_c;  i_ = _c*_d;
                                         j_ = _d*_d;
  }

  //sets the quadric representing the squared distance to the plane
  //determined by the normal _n and the point _p
  template <class _Normal, class _Point>
  void set_distance_to_plane(const _Normal&  _n, const _Point& _p)
  {
    set_distance_to_plane(_n[0], _n[1], _n[2], -dot(_n,_p));
  }

  /// set all entries to zero
  void clear()  { a_ = b_ = c_ = d_ = e_ = f_ = g_ = h_ = i_ = j_ = 0.0; }

  /// add quadrics
  QuadricT<Scalar>& operator+=( const QuadricT<Scalar>& _q )
  {
    a_ += _q.a_;  b_ += _q.b_;  c_ += _q.c_;  d_ += _q.d_;
                  e_ += _q.e_;  f_ += _q.f_;  g_ += _q.g_;
                                h_ += _q.h_;  i_ += _q.i_;
                                              j_ += _q.j_;
    return *this;
  }


  /// multiply by scalar
  QuadricT<Scalar>& operator*=( Scalar _s)
  {
    a_ *= _s;  b_ *= _s;  c_ *= _s;  d_ *= _s;
               e_ *= _s;  f_ *= _s;  g_ *= _s;
                          h_ *= _s;  i_ *= _s;
                                     j_ *= _s;
    return *this;
  }


  /// multiply 4D vector from right: Q*v
  template <class _Vec4>
  _Vec4 operator*(const _Vec4& _v) const
  {
    Scalar x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3]);
    return _Vec4(x*a_ + y*b_ + z*c_ + w*d_,
                 x*b_ + y*e_ + z*f_ + w*g_,
                 x*c_ + y*f_ + z*h_ + w*i_,
                 x*d_ + y*g_ + z*i_ + w*j_);
  }

  /// evaluate quadric Q at (3D or 4D) vector v: v*Q*v
  template <class _Vec>
  Scalar operator()(const _Vec& _v) const
  {
    return evaluate(_v, GenProg::Int2Type<_Vec::size_>());
  }

  Scalar a() const { return a_; }
  Scalar b() const { return b_; }
  Scalar c() const { return c_; }
  Scalar d() const { return d_; }
  Scalar e() const { return e_; }
  Scalar f() const { return f_; }
  Scalar g() const { return g_; }
  Scalar h() const { return h_; }
  Scalar i() const { return i_; }
  Scalar j() const { return j_; }

  Scalar xx() const { return a_; }
  Scalar xy() const { return b_; }
  Scalar xz() const { return c_; }
  Scalar xw() const { return d_; }
  Scalar yy() const { return e_; }
  Scalar yz() const { return f_; }
  Scalar yw() const { return g_; }
  Scalar zz() const { return h_; }
  Scalar zw() const { return i_; }
  Scalar ww() const { return j_; }

protected:

  /// evaluate quadric Q at 3D vector v: v*Q*v
  template <class _Vec3>
  Scalar evaluate(const _Vec3& _v, GenProg::Int2Type<3>/*_dimension*/) const
  {
    Scalar x(_v[0]), y(_v[1]), z(_v[2]);
    return a_*x*x + 2.0*b_*x*y + 2.0*c_*x*z + 2.0*d_*x
                  +     e_*y*y + 2.0*f_*y*z + 2.0*g_*y
                               +     h_*z*z + 2.0*i_*z
                                            +     j_;
  }

  /// evaluate quadric Q at 4D vector v: v*Q*v
  template <class _Vec4>
  Scalar evaluate(const _Vec4& _v, GenProg::Int2Type<4>/*_dimension*/) const
  {
    Scalar x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3]);
    return a_*x*x + 2.0*b_*x*y + 2.0*c_*x*z + 2.0*d_*x*w
                  +     e_*y*y + 2.0*f_*y*z + 2.0*g_*y*w
                               +     h_*z*z + 2.0*i_*z*w
                                            +     j_*w*w;
  }

private:

  Scalar a_, b_, c_, d_,
             e_, f_, g_,
                 h_, i_,
                     j_;
};


/// Quadric using floats
typedef QuadricT<float> Quadricf;

/// Quadric using double
typedef QuadricT<double> Quadricd;


//=============================================================================
} // END_NS_GEOMETRY
} // END_NS_OPENMESH
//============================================================================
#endif // OPENMESH_GEOMETRY_HH defined
//=============================================================================
