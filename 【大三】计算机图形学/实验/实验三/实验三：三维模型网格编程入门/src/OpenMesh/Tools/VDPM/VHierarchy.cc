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

#include <OpenMesh/Tools/VDPM/VHierarchy.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== IMPLEMENTATION ========================================================== 


VHierarchy::
VHierarchy()
{
  clear();
}

void
VHierarchy::
set_num_roots(unsigned int _n_roots)
{
  n_roots_ = _n_roots;
  
  tree_id_bits_ = 0;
  while (n_roots_ > ((unsigned int) 0x00000001 << tree_id_bits_))
    ++tree_id_bits_;
}


VHierarchyNodeHandle 
VHierarchy::
add_node()
{
  return  add_node(VHierarchyNode());
}

VHierarchyNodeHandle 
VHierarchy::
add_node(const VHierarchyNode &_node)
{
  nodes_.push_back(_node);

  return  VHierarchyNodeHandle(nodes_.size() - 1);
}


void
VHierarchy::
make_children(VHierarchyNodeHandle &_parent_handle)
{
  VHierarchyNodeHandle lchild_handle = add_node();
  VHierarchyNodeHandle rchild_handle = add_node();    

  VHierarchyNode &parent = node(_parent_handle);
  VHierarchyNode &lchild = node(lchild_handle);
  VHierarchyNode &rchild = node(rchild_handle);

  parent.set_children_handle(lchild_handle);
  lchild.set_parent_handle(_parent_handle);
  rchild.set_parent_handle(_parent_handle);

  lchild.set_index(VHierarchyNodeIndex(parent.node_index().tree_id(tree_id_bits_), 2*parent.node_index().node_id(tree_id_bits_), tree_id_bits_));
  rchild.set_index(VHierarchyNodeIndex(parent.node_index().tree_id(tree_id_bits_), 2*parent.node_index().node_id(tree_id_bits_)+1, tree_id_bits_));
}

VHierarchyNodeHandle
VHierarchy::
node_handle(VHierarchyNodeIndex _node_index)
{
  if (_node_index.is_valid(tree_id_bits_) != true)
    return  InvalidVHierarchyNodeHandle;

  VHierarchyNodeHandle node_handle = root_handle(_node_index.tree_id(tree_id_bits_));
  unsigned int node_id = _node_index.node_id(tree_id_bits_);
  unsigned int flag = 0x80000000;

  while (!(node_id & flag))   flag >>= 1;
  flag >>= 1;

  while (flag > 0 && is_leaf_node(node_handle) != true)
  {
    if (node_id & flag)     // 1: rchild
    {
      node_handle = rchild_handle(node_handle);
    }
    else                    // 0: lchild
    {
      node_handle = lchild_handle(node_handle);
    }
    flag >>= 1;
  }

  return  node_handle;
}

bool
VHierarchy::
is_ancestor(VHierarchyNodeIndex _ancestor_index, VHierarchyNodeIndex _descendent_index)
{
  if (_ancestor_index.tree_id(tree_id_bits_) != _descendent_index.tree_id(tree_id_bits_))
    return  false;

  unsigned int ancestor_node_id = _ancestor_index.node_id(tree_id_bits_);
  unsigned int descendent_node_id = _descendent_index.node_id(tree_id_bits_);

  if (ancestor_node_id > descendent_node_id)
    return  false;

  while (descendent_node_id > 0)
  {
    if (ancestor_node_id == descendent_node_id)
      return  true;
    descendent_node_id >>= 1;       // descendent_node_id /= 2
  }

  return  false;
}

//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
