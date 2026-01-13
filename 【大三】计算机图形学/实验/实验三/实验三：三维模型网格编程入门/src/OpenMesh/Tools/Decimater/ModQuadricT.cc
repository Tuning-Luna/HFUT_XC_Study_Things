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
//   $Revision: 1.2 $
//   $Date: 2005/04/18 09:07:11 $
//
//=============================================================================

/** \file ModQuadricT.cc
    Bodies of template member function.
 */

//=============================================================================
//
//  CLASS ModQuadric - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_DECIMATER_MODQUADRIC_CC

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>


//== NAMESPACE ===============================================================

namespace OpenMesh { // BEGIN_NS_OPENMESH
namespace Decimater { // BEGIN_NS_DECIMATER


//== IMPLEMENTATION ==========================================================


template<class DecimaterType>
void
ModQuadricT<DecimaterType>::
initialize()
{
  using Geometry::Quadricd;
  // alloc quadrics
  if (!quadrics_.is_valid())
      mesh_.add_property( quadrics_ );

  // clear quadrics
  typename Mesh::VertexIter  v_it  = mesh_.vertices_begin(),
                             v_end = mesh_.vertices_end();

  for (; v_it != v_end; ++v_it)
    mesh_.property(quadrics_, v_it).clear();

  // calc (normal weighted) quadric
  typename Mesh::FaceIter          f_it  = mesh_.faces_begin(),
                                   f_end = mesh_.faces_end();

  typename Mesh::FaceVertexIter    fv_it;
  typename Mesh::VertexHandle      vh0, vh1, vh2;
  typedef Vec3d                    Vec3;
  double                           a,b,c,d, area;

  for (; f_it != f_end; ++f_it)
  {
    fv_it = mesh_.fv_iter(f_it.handle());
    vh0 = fv_it.handle();  ++fv_it;
    vh1 = fv_it.handle();  ++fv_it;
    vh2 = fv_it.handle();

    Vec3 v0, v1, v2;
    {
      using namespace OpenMesh;

      v0 = vector_cast<Vec3>(mesh_.point(vh0));
      v1 = vector_cast<Vec3>(mesh_.point(vh1));
      v2 = vector_cast<Vec3>(mesh_.point(vh2));
    }

    Vec3 n = (v1-v0) % (v2-v0);
    area = n.norm();
    if (area > FLT_MIN)
    {
      n /= area;
      area *= 0.5;
    }

    a = n[0];
    b = n[1];
    c = n[2];
    d = -(vector_cast<Vec3>(mesh_.point(vh0))|n);

    Quadricd q(a, b, c, d);
    q *= area;

    mesh_.property(quadrics_, vh0) += q;
    mesh_.property(quadrics_, vh1) += q;
    mesh_.property(quadrics_, vh2) += q;
  }
}


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
