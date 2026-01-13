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
//   $Date: 2004/09/06 12:35:18 $
//                                                                            
//=============================================================================

#ifndef OPENMESH_ARRAY_ITEMS_HH
#define OPENMESH_ARRAY_ITEMS_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/Handles.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh {


//== CLASS DEFINITION =========================================================


/// Definition of mesh items for use in the ArrayKernelT
struct ArrayItems
{

  //------------------------------------------------------ internal vertex type

  /// The vertex item
  class Vertex
  {
    template <class AttribKernel, class Items> friend class ArrayKernelT;
    HalfedgeHandle  halfedge_handle_;
  };


  //---------------------------------------------------- internal halfedge type

#ifndef DOXY_IGNORE_THIS
  class Halfedge_without_prev
  {
    template <class AttribKernel, class Items> friend class ArrayKernelT;
    FaceHandle      face_handle_;
    VertexHandle    vertex_handle_;
    HalfedgeHandle  next_halfedge_handle_;
  };
#endif


#ifndef DOXY_IGNORE_THIS
  class Halfedge_with_prev : public Halfedge_without_prev
  {
    template <class AttribKernel, class Items> friend class ArrayKernelT;
    HalfedgeHandle  prev_halfedge_handle_;
  };
#endif

  

  //-------------------------------------------------------- internal edge type
#ifndef DOXY_IGNORE_THIS
  template <class _Halfedge>
  class Edge
  {
    template <class AttribKernel, class Items> friend class ArrayKernelT;
    _Halfedge  halfedges_[2]; 
  };
#endif



  //-------------------------------------------------------- internal face type

#ifndef DOXY_IGNORE_THIS
  template <bool _is_triangle>
  class Face
  {
    template <class AttribKernel, class Items> friend class ArrayKernelT;
    HalfedgeHandle  halfedge_handle_;

  public:
    typedef GenProg::Bool2Type<_is_triangle>  IsTriangle;
    static bool is_triangle() { return _is_triangle; }
  };
};
#endif

//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_ITEMS_HH defined
//=============================================================================
