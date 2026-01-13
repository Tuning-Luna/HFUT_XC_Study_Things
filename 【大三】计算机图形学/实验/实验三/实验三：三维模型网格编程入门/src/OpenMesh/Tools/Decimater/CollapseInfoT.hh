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
//   $Date: 2004/09/06 12:36:44 $
//                                                                            
//=============================================================================

/** \file CollapseInfoT.hh
    Provides data class CollapseInfoT for storing all information
    about a halfedge collapse.
 */

//=============================================================================
//
//  STRUCT CollpaseInfoT
//
//=============================================================================

#ifndef OPENMESH_DECIMATER_COLLAPSEINFOT_HH
#define OPENMESH_DECIMATER_COLLAPSEINFOT_HH


//== INCLUDES =================================================================

#include <vector>


//== NAMESPACE ================================================================

namespace OpenMesh  { 
namespace Decimater {


//== CLASS DEFINITION =========================================================

/** Stores information about a halfedge collapse.

    The class stores information about a halfedge collapse. The most
    important information is \c v0v1, \c v1v0, \c v0, \c v1, \c vl,
    \c vr, which you can lookup in the following image:  
    \image html collapse_info.png
    \see ModProgMeshT::Info
 */
template <class Mesh>
struct CollapseInfoT
{
public:
  /** Initializing constructor.
   *
   *  Given a mesh and a halfedge handle of the halfedge to be collapsed
   *  all important information of a halfedge collapse will be stored.
   * \param _mesh Mesh source 
   * \param _heh Halfedge to collapse. The direction of the halfedge
   *        defines the direction of the collapse, i.e. the from-vertex
   *        will be removed and the to-vertex remains.
   */
   CollapseInfoT(Mesh& _mesh, typename Mesh::HalfedgeHandle _heh);

  Mesh&                          mesh;

  typename Mesh::HalfedgeHandle  v0v1; ///< Halfedge to be collapsed
  typename Mesh::HalfedgeHandle  v1v0; ///< Reverse halfedge
  typename Mesh::VertexHandle    v0;   ///< Vertex to be removed
  typename Mesh::VertexHandle    v1;   ///< Remaining vertex
  typename Mesh::Point           p0;   ///< Position of removed vertex
  typename Mesh::Point           p1;   ///< Positions of remaining vertex
  typename Mesh::FaceHandle      fl;   ///< Left face
  typename Mesh::FaceHandle      fr;   ///< Right face
  typename Mesh::VertexHandle    vl;   ///< Left vertex
  typename Mesh::VertexHandle    vr;   ///< Right vertex
  //@{ 
  /** Outer remaining halfedge of diamond spanned by \c v0, \c v1, 
   *  \c vl, and \c vr
   */
  typename Mesh::HalfedgeHandle  vlv1, v0vl, vrv0, v1vr;
  //@}
  /// The one-ring of vertex v0 (the one to be collapsed)
  std::vector<typename Mesh::VertexHandle>  one_ring;
};


//-----------------------------------------------------------------------------


// CollapseInfoT::CollapseInfoT( _mesh, _heh )
//
//   Local configuration of halfedge collapse to be stored in CollapseInfoT:
/* 
           vl
           * 
          / \
         /   \
        / fl  \
    v0 *------>* v1
        \ fr  /
         \   /
          \ /
           * 
           vr
*/
// Parameters:
//   _mesh               Reference to mesh
//   _heh                The halfedge (v0 -> v1) defining the collapse
//
template <class Mesh>
inline
CollapseInfoT<Mesh>::
CollapseInfoT(Mesh& _mesh, typename Mesh::HalfedgeHandle _heh) :

  mesh(_mesh),
  v0v1(_heh),
  v1v0(_mesh.opposite_halfedge_handle(v0v1)),
  v0(_mesh.to_vertex_handle(v1v0)),
  v1(_mesh.to_vertex_handle(v0v1)),
  p0(_mesh.point(v0)),
  p1(_mesh.point(v1)),
  fl(_mesh.face_handle(v0v1)),
  fr(_mesh.face_handle(v1v0))

{
  // get vl
  if (fl.is_valid())
  {
    vlv1 = mesh.next_halfedge_handle(v0v1);
    v0vl = mesh.next_halfedge_handle(vlv1);
    vl   = mesh.to_vertex_handle(vlv1);
    vlv1 = mesh.opposite_halfedge_handle(vlv1);
    v0vl = mesh.opposite_halfedge_handle(v0vl);
  }


  // get vr
  if (fr.is_valid())
  {
    vrv0 = mesh.next_halfedge_handle(v1v0);
    v1vr = mesh.next_halfedge_handle(vrv0);
    vr   = mesh.to_vertex_handle(vrv0);
    vrv0 = mesh.opposite_halfedge_handle(vrv0);
    v1vr = mesh.opposite_halfedge_handle(v1vr);
  }


  // store one-ring
  one_ring.clear();
  for (typename Mesh::VVIter vv_it=mesh.vv_iter(v0); vv_it; ++vv_it)
    one_ring.push_back(vv_it);
}  

//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENMESH_DECIMATER_COLLAPSEINFOT_HH defined
//=============================================================================

