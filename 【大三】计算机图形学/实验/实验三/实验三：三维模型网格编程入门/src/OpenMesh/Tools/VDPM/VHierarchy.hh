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

#ifndef OPENMESH_VDPROGMESH_VHIERARCHY_HH
#define OPENMESH_VDPROGMESH_VHIERARCHY_HH


//== INCLUDES =================================================================

#include <vector>
#include <OpenMesh/Tools/VDPM/VHierarchyNode.hh>


//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== CLASS DEFINITION =========================================================

	      
/** Keeps the vertex hierarchy build during analyzing a progressive mesh.
 */
class VHierarchy
{
public:

  typedef unsigned int    id_t; ///< Type for tree and node ids

private:

  VHierarchyNodeContainer nodes_;
  unsigned int            n_roots_;
  unsigned char           tree_id_bits_; // node_id_bits_ = 32-tree_id_bits_;

public:
  
  VHierarchy();

  void clear()                        { nodes_.clear();   n_roots_ = 0; }
  unsigned char tree_id_bits() const  { return tree_id_bits_; }
  unsigned int num_roots() const      { return n_roots_; }
  unsigned int num_nodes() const      { return nodes_.size(); }

  VHierarchyNodeIndex generate_node_index(id_t _tree_id, id_t _node_id)
  {
    return  VHierarchyNodeIndex(_tree_id, _node_id, tree_id_bits_);
  }


  void set_num_roots(unsigned int _n_roots);
  
  VHierarchyNodeHandle root_handle(unsigned int i) const
  {
    return  VHierarchyNodeHandle( (int)i );
  }


  const VHierarchyNode& node(VHierarchyNodeHandle _vhierarchynode_handle) const
  {
    return nodes_[_vhierarchynode_handle.idx()];
  }


  VHierarchyNode& node(VHierarchyNodeHandle _vhierarchynode_handle)
  {
    return nodes_[_vhierarchynode_handle.idx()];
  }

  VHierarchyNodeHandle add_node();
  VHierarchyNodeHandle add_node(const VHierarchyNode &_node);

  void make_children(VHierarchyNodeHandle &_parent_handle);

  bool is_ancestor(VHierarchyNodeIndex _ancestor_index, 
		   VHierarchyNodeIndex _descendent_index);
  
  bool is_leaf_node(VHierarchyNodeHandle _node_handle)  
  { return nodes_[_node_handle.idx()].is_leaf(); }

  bool is_root_node(VHierarchyNodeHandle _node_handle)  
  { return nodes_[_node_handle.idx()].is_root(); }


  const OpenMesh::Vec3f& normal(VHierarchyNodeHandle _node_handle) const  
  {
    return  nodes_[_node_handle.idx()].normal(); 
  }

  const VHierarchyNodeIndex& 
  node_index(VHierarchyNodeHandle _node_handle) const
  { return  nodes_[_node_handle.idx()].node_index(); }

  VHierarchyNodeIndex& node_index(VHierarchyNodeHandle _node_handle)
  { return  nodes_[_node_handle.idx()].node_index(); }

  const VHierarchyNodeIndex& 
  fund_lcut_index(VHierarchyNodeHandle _node_handle) const
  { return  nodes_[_node_handle.idx()].fund_lcut_index(); }

  VHierarchyNodeIndex& fund_lcut_index(VHierarchyNodeHandle _node_handle)
  { return  nodes_[_node_handle.idx()].fund_lcut_index(); }

  const VHierarchyNodeIndex& 
  fund_rcut_index(VHierarchyNodeHandle _node_handle) const
  { return  nodes_[_node_handle.idx()].fund_rcut_index(); }

  VHierarchyNodeIndex& fund_rcut_index(VHierarchyNodeHandle _node_handle)
  { return  nodes_[_node_handle.idx()].fund_rcut_index(); }     
  
  VertexHandle  vertex_handle(VHierarchyNodeHandle _node_handle)
  { return  nodes_[_node_handle.idx()].vertex_handle(); }

  VHierarchyNodeHandle  parent_handle(VHierarchyNodeHandle _node_handle)
  { return nodes_[_node_handle.idx()].parent_handle(); }

  VHierarchyNodeHandle  lchild_handle(VHierarchyNodeHandle _node_handle)
  { return nodes_[_node_handle.idx()].lchild_handle(); }

  VHierarchyNodeHandle  rchild_handle(VHierarchyNodeHandle _node_handle)
  { return nodes_[_node_handle.idx()].rchild_handle(); }

  VHierarchyNodeHandle  node_handle(VHierarchyNodeIndex _node_index);

private:
  
  VHierarchyNodeHandle compute_dependency(VHierarchyNodeIndex index0, 
					  VHierarchyNodeIndex index1);

};



//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_VDPROGMESH_VHIERARCHY_HH defined
//=============================================================================
