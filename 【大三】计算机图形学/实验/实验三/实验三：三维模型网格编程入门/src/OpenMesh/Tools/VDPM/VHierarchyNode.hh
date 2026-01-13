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

#ifndef OPENMESH_VDPROGMESH_VHIERARCHYNODE_HH
#define OPENMESH_VDPROGMESH_VHIERARCHYNODE_HH


//== INCLUDES =================================================================


#include <vector>
#include <list>
#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/Handles.hh>
#include <OpenMesh/Tools/VDPM/VHierarchyNodeIndex.hh>


//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== CLASS DEFINITION =========================================================

	      
/** Handle for vertex hierarchy nodes  
 */
struct VHierarchyNodeHandle : public BaseHandle
{
  explicit VHierarchyNodeHandle(int _idx=-1) : BaseHandle(_idx) {}
};


/// Invalid handle
static const VHierarchyNodeHandle InvalidVHierarchyNodeHandle;


/** Vertex hierarchy node
 *  \todo Complete documentation
 */
class VHierarchyNode
{
public:

  VHierarchyNode() { }

  /// Returns true, if node is root else false.
  bool is_root() const
  { return (parent_handle_.is_valid() == false) ? true : false; }

  /// Returns true, if node is leaf else false.
  bool is_leaf() const
  { return (lchild_handle_.is_valid() == false) ? true : false; }
  
  /// Returns parent handle.
  VHierarchyNodeHandle parent_handle() { return parent_handle_; }
  
  /// Returns handle to left child.
  VHierarchyNodeHandle lchild_handle() { return lchild_handle_; }

  /// Returns handle to right child.
  VHierarchyNodeHandle rchild_handle() 
  { return VHierarchyNodeHandle(lchild_handle_.idx()+1); }

  void set_parent_handle(VHierarchyNodeHandle _parent_handle)
  { parent_handle_ = _parent_handle; }

  void set_children_handle(VHierarchyNodeHandle _lchild_handle)
  { lchild_handle_ = _lchild_handle; }

  VertexHandle vertex_handle() const                  { return vh_; }
  float radius() const                                { return radius_; }
  const OpenMesh::Vec3f& normal() const               { return normal_; }
  float sin_square() const                            { return sin_square_; }
  float mue_square() const                            { return mue_square_; }
  float sigma_square() const                          { return sigma_square_; }

  void set_vertex_handle(OpenMesh::VertexHandle _vh)  { vh_     = _vh; }
  void set_radius(float _radius)                      { radius_ = _radius; }
  void set_normal(const OpenMesh::Vec3f &_normal)     { normal_ = _normal; }
  
  void set_sin_square(float _sin_square)      { sin_square_ = _sin_square; }
  void set_mue_square(float _mue_square)      { mue_square_ = _mue_square; }
  void set_sigma_square(float _sigma_square)  { sigma_square_ = _sigma_square; }
  
  void set_semi_angle(float _semi_angle) 
  { float f=sinf(_semi_angle); sin_square_ = f*f; }

  void set_mue(float _mue)                        { mue_square_ = _mue * _mue; }
  void set_sigma(float _sigma)              { sigma_square_ = _sigma * _sigma; }

  const VHierarchyNodeIndex& node_index() const         { return  node_index_; }
  const VHierarchyNodeIndex& fund_lcut_index() const 
  { return  fund_cut_node_index_[0]; }

  const VHierarchyNodeIndex& fund_rcut_index() const
  { return  fund_cut_node_index_[1]; }

  VHierarchyNodeIndex& node_index()
  { return  node_index_; }

  VHierarchyNodeIndex& fund_lcut_index()   { return  fund_cut_node_index_[0]; }
  VHierarchyNodeIndex& fund_rcut_index()   { return  fund_cut_node_index_[1]; }

  void set_index(const VHierarchyNodeIndex &_node_index)
  { node_index_ = _node_index; }

  void set_fund_lcut(const VHierarchyNodeIndex &_node_index)
  { fund_cut_node_index_[0] = _node_index; }

  void set_fund_rcut(const VHierarchyNodeIndex &_node_index)
  { fund_cut_node_index_[1] = _node_index; }

private:
  VertexHandle            vh_;
  float                   radius_;
  Vec3f                   normal_;
  float                   sin_square_;
  float                   mue_square_;
  float                   sigma_square_;

  VHierarchyNodeHandle    parent_handle_;
  VHierarchyNodeHandle    lchild_handle_;


  VHierarchyNodeIndex     node_index_;
  VHierarchyNodeIndex     fund_cut_node_index_[2];
};

/// Container for vertex hierarchy nodes
typedef std::vector<VHierarchyNode>         VHierarchyNodeContainer;

/// Container for vertex hierarchy node handles
typedef std::vector<VHierarchyNodeHandle>   VHierarchyNodeHandleContainer;

/// Container for vertex hierarchy node handles
typedef std::list<VHierarchyNodeHandle>     VHierarchyNodeHandleList;


//=============================================================================
} // namesapce VDPM
} // namespace OpenMesh
//=============================================================================
#endif //  OPENMESH_VDPROGMESH_VHIERARCHYNODE_HH defined
//=============================================================================
