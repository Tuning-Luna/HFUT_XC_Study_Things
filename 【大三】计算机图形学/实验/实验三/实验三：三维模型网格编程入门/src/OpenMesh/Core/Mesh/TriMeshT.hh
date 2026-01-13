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
//   $Revision: 1.4 $
//   $Date: 2005/08/11 13:06:12 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  CLASS TriMeshT
//
//=============================================================================


#ifndef OPENMESH_TRIMESH_HH
#define OPENMESH_TRIMESH_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Mesh/PolyMeshT.hh>
#include <vector>


//== NAMESPACES ===============================================================


namespace OpenMesh {


//== CLASS DEFINITION =========================================================


/** \class TriMeshT TriMeshT.hh <OpenMesh/Mesh/TriMeshT.hh>

    Base type for a triangle mesh.
    
    Base type for a triangle mesh, parameterized by a mesh kernel. The
    mesh inherits all methods from the kernel class and the
    more general polygonal mesh PolyMeshT. Therefore it provides
    the same types for items, handles, iterators and so on.

    \param Kernel: template argument for the mesh kernel
    \note You should use the predefined mesh-kernel combinations in
    \ref mesh_types_group
    \see \ref mesh_type 
    \see OpenMesh::PolyMeshT
*/

template <class Kernel>
class TriMeshT : public PolyMeshT<Kernel>
{

public:


  // self
  typedef TriMeshT<Kernel>                    This;
  typedef PolyMeshT<Kernel>                   PolyMesh;

  //@{
  /// Determine whether this is a PolyMeshT or TriMeshT
  enum { IsPolyMesh = 0 };
  enum { IsTriMesh  = 1 };
  static bool is_polymesh() { return false; }
  static bool is_trimesh()  { return  true; }
  //@}


  //--- items ---

  typedef typename PolyMesh::Scalar             Scalar;
  typedef typename PolyMesh::Point              Point;
  typedef typename PolyMesh::Normal             Normal;
  typedef typename PolyMesh::Color              Color;
  typedef typename PolyMesh::TexCoord1D         TexCoord1D;
  typedef typename PolyMesh::TexCoord2D         TexCoord2D;
  typedef typename PolyMesh::TexCoord3D         TexCoord3D;
  typedef typename PolyMesh::Vertex             Vertex;
  typedef typename PolyMesh::Halfedge           Halfedge;
  typedef typename PolyMesh::Edge               Edge;
  typedef typename PolyMesh::Face               Face;
  

  //--- handles ---

  typedef typename PolyMesh::VertexHandle       VertexHandle;
  typedef typename PolyMesh::HalfedgeHandle     HalfedgeHandle;
  typedef typename PolyMesh::EdgeHandle         EdgeHandle;
  typedef typename PolyMesh::FaceHandle         FaceHandle;


  //--- iterators ---

  typedef typename PolyMesh::VertexIter         VertexIter;
  typedef typename PolyMesh::ConstVertexIter    ConstVertexIter;
  typedef typename PolyMesh::EdgeIter           EdgeIter;
  typedef typename PolyMesh::ConstEdgeIter      ConstEdgeIter;
  typedef typename PolyMesh::FaceIter           FaceIter;
  typedef typename PolyMesh::ConstFaceIter      ConstFaceIter;



  //--- circulators ---

  typedef typename PolyMesh::VertexVertexIter         VertexVertexIter;
  typedef typename PolyMesh::VertexOHalfedgeIter      VertexOHalfedgeIter;
  typedef typename PolyMesh::VertexIHalfedgeIter      VertexIHalfedgeIter;
  typedef typename PolyMesh::VertexEdgeIter           VertexEdgeIter;
  typedef typename PolyMesh::VertexFaceIter           VertexFaceIter;
  typedef typename PolyMesh::FaceVertexIter           FaceVertexIter;
  typedef typename PolyMesh::FaceHalfedgeIter         FaceHalfedgeIter;
  typedef typename PolyMesh::FaceEdgeIter             FaceEdgeIter;
  typedef typename PolyMesh::FaceFaceIter             FaceFaceIter;
  typedef typename PolyMesh::ConstVertexVertexIter    ConstVertexVertexIter;
  typedef typename PolyMesh::ConstVertexOHalfedgeIter ConstVertexOHalfedgeIter;
  typedef typename PolyMesh::ConstVertexIHalfedgeIter ConstVertexIHalfedgeIter;
  typedef typename PolyMesh::ConstVertexEdgeIter      ConstVertexEdgeIter;
  typedef typename PolyMesh::ConstVertexFaceIter      ConstVertexFaceIter;
  typedef typename PolyMesh::ConstFaceVertexIter      ConstFaceVertexIter;
  typedef typename PolyMesh::ConstFaceHalfedgeIter    ConstFaceHalfedgeIter;
  typedef typename PolyMesh::ConstFaceEdgeIter        ConstFaceEdgeIter;
  typedef typename PolyMesh::ConstFaceFaceIter        ConstFaceFaceIter;






  // --- constructor/destructor

  /// Default constructor
  TriMeshT() : PolyMesh() {}

  /// Destructor
  ~TriMeshT() {}

  /** Assignment operator. 
      \sa PolyMeshT::operator=()
  */
  TriMeshT& operator=(const TriMeshT& _rhs)
  { if (this!=&_rhs) PolyMesh::operator=(_rhs); return *this; }





  // --- creation ---

  /** \name Addding items to a mesh
  */
  //@{

  /** Override OpenMesh::Mesh::PolyMeshT::add_face(). Faces that aren't
      triangles will be triangulated and added. In this case an
      invalid face handle will be returned.
  */
  FaceHandle add_face(const std::vector<VertexHandle>& _vhandles);

  /** Add face wrapper that accepts three vertex handles. Provided for
      convenience.
  */
  FaceHandle add_face(VertexHandle _v0, VertexHandle _v1, VertexHandle _v2) {
    static std::vector<VertexHandle> vhandles(3);
    vhandles[0] = _v0; vhandles[1] = _v1; vhandles[2] = _v2;
    return PolyMesh::add_face(vhandles);
  }

  /** Inherited assignment method.
      \see OpenMesh::PolyMeshT::assign()
  */
  template <class OtherMesh>
  TriMeshT& assign(const OtherMesh& _rhs)
  #if OM_OUT_OF_CLASS_TEMPLATE || defined(DOXYGEN)
     ;
  #else
  #  include "PolyMeshT_assign.hh"
  #endif

  //@}


  /** Returns the opposite vertex to the halfedge _heh in the face
      referenced by _heh returns InvalidVertexHandle if the _heh is
      boundary
  */
  VertexHandle opposite_vh(HalfedgeHandle _heh) const
  {
    if (is_boundary(_heh))
    {
      return This::InvalidVertexHandle;
    }
    return to_vertex_handle(next_halfedge_handle(_heh));
  }

  /** Returns the opposite vertex to the opposite halfedge of _heh in
      the face referenced by it returns InvalidVertexHandle if the
      opposite halfedge is boundary
  */
  VertexHandle opposite_he_opposite_vh(HalfedgeHandle _heh) const
  {
    return opposite_vh(opposite_halfedge_handle(_heh));
  }


  //--- halfedge collapse / vertex split ---


  /** \name Topology modifying operators
  */
  //@{

  /** Halfedge collapse: collapse the from-vertex of halfedge _heh
      into its to-vertex.

      \attention Needs vertex/edge/face status attribute in order to
      delete the items that degenerate.

      \note This function does not perform a garbage collection. It
      only marks degenerate items as deleted.

      \attention A halfedge collapse may lead to topological inconsistencies.
      Therefore you should check this using is_collapse_ok().
  */
  void collapse(HalfedgeHandle _heh);



  /** Returns whether collapsing halfedge _heh is ok or would lead to
      topological inconsistencies.
      \attention This method need the Attributes::Status attribute and
      changes the \em tagged bit.
  */
  bool is_collapse_ok(HalfedgeHandle _heh);


  /// Vertex Split: inverse operation to collapse().
  HalfedgeHandle vertex_split( Point        _v0_point,
			       VertexHandle _v1, 
			       VertexHandle _vl, 
			       VertexHandle _vr )
  { return vertex_split(add_vertex(_v0_point), _v1, _vl, _vr); }


  /// Vertex Split: inverse operation to collapse().
  HalfedgeHandle vertex_split( VertexHandle v0,
			       VertexHandle v1, 
			       VertexHandle vl, 
			       VertexHandle vr );


  /// Check whether flipping _eh is topologically correct.
  bool is_flip_ok(EdgeHandle _eh) const;

  /** Flip edge _eh. 
      Check for topological correctness first using is_flip_ok(). */
  void flip(EdgeHandle _eh);

  /// Edge split (= 2-to-4 split)
  void split(EdgeHandle _eh, VertexHandle _vh);
  /// Edge split (= 2-to-4 split)
  void split(EdgeHandle _eh, const Point& _p) { 
    split(_eh, add_vertex(_p)); 
  }

  /// Face split (= 1-to-3 split, calls corresponding PolyMeshT function).
  void split(FaceHandle _fh, VertexHandle _vh) {
    PolyMeshT<Kernel>::split(_fh, _vh);
  }
  /// Face split (= 1-to-3 split, calls corresponding PolyMeshT function).
  void split(FaceHandle _fh, const Point& _p) {
    PolyMeshT<Kernel>::split(_fh, _p);
  }

  //@}

  
  /// Helper for halfedge collapse
  void remove_edge(HalfedgeHandle _hh);

  /// Helper for halfedge collapse
  void remove_loop(HalfedgeHandle _hh);

  /// Helper for vertex split
  HalfedgeHandle insert_loop(HalfedgeHandle _hh);

  /// Helper for vertex split
  HalfedgeHandle insert_edge(VertexHandle   _vh,
			     HalfedgeHandle _h0, 
			     HalfedgeHandle _h1);
};


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_TRIMESH_C)
#define OPENMESH_TRIMESH_TEMPLATES
#include "TriMeshT.cc"
#endif
//=============================================================================
#endif // OPENMESH_TRIMESH_HH defined
//=============================================================================
