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

#ifndef OPENMESH_VDPROGMESH_VHIERARCHYNODEINDEX_HH
#define OPENMESH_VDPROGMESH_VHIERARCHYNODEINDEX_HH

//== INCLUDES =================================================================

#include <vector>
#include <assert.h>

//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== CLASS DEFINITION =========================================================

	      
/** Index of vertex hierarchy node
 */


class VHierarchyNodeIndex
{
private:
  unsigned int value_;

public:

  static const VHierarchyNodeIndex  InvalidIndex;

public:

  VHierarchyNodeIndex()
  { value_ = 0; }
  
  VHierarchyNodeIndex(unsigned int _value)
  { value_ = _value; }

  VHierarchyNodeIndex(const VHierarchyNodeIndex &_other)
  { value_ = _other.value_; }

  VHierarchyNodeIndex(unsigned int   _tree_id, 
		      unsigned int   _node_id, 
		      unsigned short _tree_id_bits)
  {
    assert(_tree_id < ((unsigned int) 0x00000001 << _tree_id_bits));
    assert(_node_id < ((unsigned int) 0x00000001 << (32 - _tree_id_bits)));
    value_ = (_tree_id << (32 - _tree_id_bits)) | _node_id;
  }

  bool is_valid(unsigned short _tree_id_bits) const
  { return  node_id(_tree_id_bits) != 0 ? true : false;  }

  unsigned int tree_id(unsigned short _tree_id_bits) const
  { return  value_ >> (32 - _tree_id_bits); }
  
  unsigned int node_id(unsigned short _tree_id_bits) const
  { return  value_ & ((unsigned int) 0xFFFFFFFF >> _tree_id_bits); }

  bool operator< (const VHierarchyNodeIndex &other) const
  { return  (value_ < other.value_) ? true : false; }

  unsigned int value() const
  { return  value_; }
};


/// Container for vertex hierarchy node indices
typedef std::vector<VHierarchyNodeIndex>    VHierarchyNodeIndexContainer;


//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
#endif //  OPENMESH_VDPROGMESH_VHIERARCHYNODEINDEX_HH defined
//=============================================================================
