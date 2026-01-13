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
//  CLASS newClass
//
//=============================================================================

#ifndef OPENMESH_VDPROGMESH_VFRONT_HH
#define OPENMESH_VDPROGMESH_VFRONT_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Tools/VDPM/VHierarchyNode.hh>
#include <vector>


//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== CLASS DEFINITION =========================================================

	      
/** Active nodes in vertex hierarchy.
    \todo VFront documentation
*/
class VFront
{
private:

  typedef VHierarchyNodeHandleList::iterator  VHierarchyNodeHandleListIter;
  enum VHierarchyNodeStatus { kSplit, kActive, kCollapse };
  
  VHierarchyNodeHandleList                    front_;
  VHierarchyNodeHandleListIter                front_it_;
  std::vector<VHierarchyNodeHandleListIter>   front_location_;

public:

  VFront();

  void clear() { front_.clear(); front_location_.clear(); }
  void begin() { front_it_ = front_.begin(); }
  bool end()   { return (front_it_ == front_.end()) ? true : false; }
  void next()  { ++front_it_; }
  int size()   { return (int) front_.size(); }
  VHierarchyNodeHandle node_handle()    { return  *front_it_; }

  void add(VHierarchyNodeHandle _node_handle);
  void remove(VHierarchyNodeHandle _node_handle);
  bool is_active(VHierarchyNodeHandle _node_handle);
  void init(VHierarchyNodeHandleContainer &_roots, unsigned int _n_details);  
};


//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_VDPROGMESH_VFRONT_HH defined
//=============================================================================
