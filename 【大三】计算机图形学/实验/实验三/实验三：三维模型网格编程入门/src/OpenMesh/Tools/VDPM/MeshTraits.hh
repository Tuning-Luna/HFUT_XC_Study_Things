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
//  CLASS VDPMTraits
//
//=============================================================================


#ifndef OPENMESH_VDPM_TRAITS_HH
#define OPENMESH_VDPM_TRAITS_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Tools/VDPM/VHierarchy.hh>

//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {

//== CLASS DEFINITION =========================================================

	      
/** \class MeshTraits MeshTraits.hh <OpenMesh/Tools/VDPM/MeshTraits.hh>

    Mesh traits for View Dependent Progressive Meshes  
*/

struct MeshTraits : public DefaultTraits
{
  VertexTraits
  {
  public:

    VHierarchyNodeHandle vhierarchy_node_handle()
    {
      return node_handle_; 
    }

    void set_vhierarchy_node_handle(VHierarchyNodeHandle _node_handle)
    {
      node_handle_ = _node_handle; 
    }
    
    bool is_ancestor(const VHierarchyNodeIndex &_other)
    {
      return false; 
    }

  private:

    VHierarchyNodeHandle  node_handle_;
   
  };
  
  VertexAttributes(OpenMesh::Attributes::Status |
		   OpenMesh::Attributes::Normal);
  HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
  EdgeAttributes(OpenMesh::Attributes::Status);
  FaceAttributes(OpenMesh::Attributes::Status |
		 OpenMesh::Attributes::Normal);
};


//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_VDPM_TRAITS_HH defined
//=============================================================================

