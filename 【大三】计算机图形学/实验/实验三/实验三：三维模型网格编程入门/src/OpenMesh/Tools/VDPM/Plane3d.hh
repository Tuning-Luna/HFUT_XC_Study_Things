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

//=============================================================================
//
//  CLASS Plane3D
//
//=============================================================================


#ifndef OPENMESH_PLANE3D_HH
#define OPENMESH_PLANE3D_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Math/VectorT.hh>


//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== CLASS DEFINITION =========================================================

	      
/** \class Plane3d Plane3d.hh <OpenMesh/Tools/VDPM/Plane3d.hh>

    ax + by + cz + d = 0
*/


class Plane3d
{
public:

  typedef OpenMesh::Vec3f         vector_type;
  typedef vector_type::value_type value_type;

public:

  Plane3d()
    : d_(0)
  { }

  Plane3d(const vector_type &_dir, const vector_type &_pnt)
    : n_(_dir), d_(0)
  { 
    n_.normalize();
    d_ = -dot(n_,_pnt); 
  }

  value_type signed_distance(const OpenMesh::Vec3f &_p)
  {
    return  dot(n_ , _p) + d_;
  }

  // back compatibility
  value_type singed_distance(const OpenMesh::Vec3f &point)
  { return signed_distance( point ); }

public:

  vector_type n_;
  value_type  d_;

};

//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_PLANE3D_HH defined
//=============================================================================
