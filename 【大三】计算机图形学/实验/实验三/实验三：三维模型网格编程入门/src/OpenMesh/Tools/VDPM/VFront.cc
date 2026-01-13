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
//  CLASS newClass - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <OpenMesh/Tools/VDPM/VFront.hh>

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== IMPLEMENTATION ========================================================== 


VFront::VFront()
{  
}


void
VFront::
add(VHierarchyNodeHandle _node_handle)
{
  front_location_[_node_handle.idx()] = front_.insert(front_.end(), _node_handle);
}


void
VFront::
remove(VHierarchyNodeHandle _node_handle)
{
  VHierarchyNodeHandleListIter node_it = front_location_[_node_handle.idx()];
  VHierarchyNodeHandleListIter next_it = front_.erase(node_it);
  front_location_[_node_handle.idx()] = front_.end();

  if (front_it_ == node_it)
    front_it_ = next_it;
}

bool
VFront::
is_active(VHierarchyNodeHandle _node_handle)
{
  return  (front_location_[_node_handle.idx()] != front_.end()) ? true : false;
}

void 
VFront::
init(VHierarchyNodeHandleContainer &_roots, unsigned int _n_details)
{
  unsigned int i;

  front_location_.resize(_roots.size() + 2*_n_details);
  for (i=0; i<front_location_.size(); ++i)
    front_location_[i] = front_.end();

  for (i=0; i<_roots.size(); ++i)
    add(_roots[i]);
}


//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
