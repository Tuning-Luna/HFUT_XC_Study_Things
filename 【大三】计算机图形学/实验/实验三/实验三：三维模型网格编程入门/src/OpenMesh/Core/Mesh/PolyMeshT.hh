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
//   $Revision: 1.3 $
//   $Date: 2005/04/18 09:07:09 $
//
//=============================================================================


//=============================================================================
//
//  CLASS PolyMeshT
//
//=============================================================================


#ifndef OPENMESH_POLYMESHT_HH
#define OPENMESH_POLYMESHT_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Mesh/BaseMesh.hh>
#include <OpenMesh/Core/Mesh/Iterators/IteratorsT.hh>
#include <OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh>
#include <OpenMesh/Core/Attributes/Attributes.hh>
#include <OpenMesh/Core/Math/MathDefs.hh>
#include <vector>


//== NAMESPACES ===============================================================


namespace OpenMesh {


//== CLASS DEFINITION =========================================================


/** \class PolyMeshT PolyMeshT.hh <OpenMesh/Mesh/PolyMeshT.hh>

    Base type for a polygonal mesh.

    This is the base class for a polygonal mesh. It is parameterized
    by a mesh kernel that is given as a template argument. This class
    inherits all methods from its mesh kernel.

    \param Kernel: template argument for the mesh kernel
    \note You should use the predefined mesh-kernel combinations in
    \ref mesh_types_group
    \see \ref mesh_type
*/

template <class Kernel>
class PolyMeshT : public Kernel, public BaseMesh
{
public:

  /// Self type. Used to specify iterators/circulators.
  typedef PolyMeshT<Kernel>   This;

  //@{
  /// Determine whether this is a PolyMeshT or TriMeshT
  enum { IsPolyMesh = 1 };
  enum { IsTriMesh  = 0 };
  static bool is_polymesh() { return true;  }
  static bool is_trimesh()  { return false; }
  //@}


  //--- item types ---

  /// \name Mesh Items
  //@{
  /// Scalar type
  typedef typename Kernel::Scalar    Scalar;
  /// Coordinate type
  typedef typename Kernel::Point     Point;

  /// Normal type
  typedef typename Kernel::Normal    Normal;

  /// Color type
  typedef typename Kernel::Color     Color;

  /// TexCoord1D type
  typedef typename Kernel::TexCoord1D  TexCoord1D;
  /// TexCoord2D type
  typedef typename Kernel::TexCoord2D  TexCoord2D;
  /// TexCoord3D type
  typedef typename Kernel::TexCoord3D  TexCoord3D;

  /// Vertex type
  typedef typename Kernel::Vertex    Vertex;
  /// Halfedge type
  typedef typename Kernel::Halfedge  Halfedge;
  /// Edge type
  typedef typename Kernel::Edge      Edge;
  /// Face type
  typedef typename Kernel::Face      Face;
  //@}



  //--- handle types ---

  /// \name Mesh Handles
  //@{
  /// Handle for referencing the corresponding item
  typedef typename Kernel::VertexHandle       VertexHandle;
  typedef typename Kernel::HalfedgeHandle     HalfedgeHandle;
  typedef typename Kernel::EdgeHandle         EdgeHandle;
  typedef typename Kernel::FaceHandle         FaceHandle;

  /// Invalid handle
  static const VertexHandle    InvalidVertexHandle;
  /// Invalid handle
  static const HalfedgeHandle  InvalidHalfedgeHandle;
  /// Invalid handle
  static const EdgeHandle      InvalidEdgeHandle;
  /// Invalid handle
  static const FaceHandle      InvalidFaceHandle;
  //@}



  //--- iterators ---

  /** \name Mesh Iterators
      Refer to OpenMesh::Mesh::Iterators or \ref mesh_iterators for
      documentation.
  */
  //@{
  /// Linear iterator
  typedef Iterators::VertexIterT<This>                VertexIter;
  typedef Iterators::HalfedgeIterT<This>              HalfedgeIter;
  typedef Iterators::EdgeIterT<This>                  EdgeIter;
  typedef Iterators::FaceIterT<This>                  FaceIter;

  typedef Iterators::ConstVertexIterT<This>           ConstVertexIter;
  typedef Iterators::ConstHalfedgeIterT<This>         ConstHalfedgeIter;
  typedef Iterators::ConstEdgeIterT<This>             ConstEdgeIter;
  typedef Iterators::ConstFaceIterT<This>             ConstFaceIter;
  //@}



  //--- circulators ---

  /** \name Mesh Circulators
      Refer to OpenMesh::Mesh::Iterators or \ref mesh_iterators
      for documentation.
  */
  //@{
  /// Circulator
  typedef Iterators::VertexVertexIterT<This>          VertexVertexIter;
  typedef Iterators::VertexOHalfedgeIterT<This>       VertexOHalfedgeIter;
  typedef Iterators::VertexIHalfedgeIterT<This>       VertexIHalfedgeIter;
  typedef Iterators::VertexEdgeIterT<This>            VertexEdgeIter;
  typedef Iterators::VertexFaceIterT<This>            VertexFaceIter;
  typedef Iterators::FaceVertexIterT<This>            FaceVertexIter;
  typedef Iterators::FaceHalfedgeIterT<This>          FaceHalfedgeIter;
  typedef Iterators::FaceEdgeIterT<This>              FaceEdgeIter;
  typedef Iterators::FaceFaceIterT<This>              FaceFaceIter;

  typedef Iterators::ConstVertexVertexIterT<This>     ConstVertexVertexIter;
  typedef Iterators::ConstVertexOHalfedgeIterT<This>  ConstVertexOHalfedgeIter;
  typedef Iterators::ConstVertexIHalfedgeIterT<This>  ConstVertexIHalfedgeIter;
  typedef Iterators::ConstVertexEdgeIterT<This>       ConstVertexEdgeIter;
  typedef Iterators::ConstVertexFaceIterT<This>       ConstVertexFaceIter;
  typedef Iterators::ConstFaceVertexIterT<This>       ConstFaceVertexIter;
  typedef Iterators::ConstFaceHalfedgeIterT<This>     ConstFaceHalfedgeIter;
  typedef Iterators::ConstFaceEdgeIterT<This>         ConstFaceEdgeIter;
  typedef Iterators::ConstFaceFaceIterT<This>         ConstFaceFaceIter;
  //@}




  // --- shortcuts

  /** \name Typedef Shortcuts
      Provided for convenience only
  */
  //@{
  /// Alias typedef
  typedef VertexHandle    VHandle;
  typedef HalfedgeHandle  HHandle;
  typedef EdgeHandle      EHandle;
  typedef FaceHandle      FHandle;

  typedef VertexIter    VIter;
  typedef HalfedgeIter  HIter;
  typedef EdgeIter      EIter;
  typedef FaceIter      FIter;

  typedef ConstVertexIter    CVIter;
  typedef ConstHalfedgeIter  CHIter;
  typedef ConstEdgeIter      CEIter;
  typedef ConstFaceIter      CFIter;

  typedef VertexVertexIter      VVIter;
  typedef VertexOHalfedgeIter   VOHIter;
  typedef VertexIHalfedgeIter   VIHIter;
  typedef VertexEdgeIter        VEIter;
  typedef VertexFaceIter        VFIter;
  typedef FaceVertexIter        FVIter;
  typedef FaceHalfedgeIter      FHIter;
  typedef FaceEdgeIter          FEIter;
  typedef FaceFaceIter          FFIter;

  typedef ConstVertexVertexIter      CVVIter;
  typedef ConstVertexOHalfedgeIter   CVOHIter;
  typedef ConstVertexIHalfedgeIter   CVIHIter;
  typedef ConstVertexEdgeIter        CVEIter;
  typedef ConstVertexFaceIter        CVFIter;
  typedef ConstFaceVertexIter        CFVIter;
  typedef ConstFaceHalfedgeIter      CFHIter;
  typedef ConstFaceEdgeIter          CFEIter;
  typedef ConstFaceFaceIter          CFFIter;
  //@}



  // --- constructor/destructor

  /// Default constructor
  PolyMeshT()  {}

  /// Destructor
  ~PolyMeshT() {}

  /** Assignment operator.
      This function call is passed on to the correspoding mesh kernel.
      Use this function to assign two meshes of \b equal type.
      If the mesh types vary, use PolyMeshT::assign() instead.
  */
  PolyMeshT& operator=(const PolyMeshT& _rhs) {
    if (this!=&_rhs) Kernel::operator=(_rhs); return *this;
  }





  // --- creation ---

  /** \name Adding items to a mesh
  */
  //@{

  /// Add a new vertex with coordinate \c _p
  VertexHandle add_vertex(const Point& _p) {
    return new_vertex(_p);
  }

  /// Add and connect a new face
  FaceHandle add_face(const std::vector<VertexHandle>& _vhandles);

  /** Assignment from another mesh of \em another type.
      This method will use \c add_vertex() and \c add_face()
      in order to copy _rhs to itself. This will not be very
      efficient, but because different mesh types can have
      different item types it's the only solution.

      \note All that's copied is connectivity and vertex positions.
      All other information (like e.g. attributes or additional
      elements from traits classes) is not copied.

      \note If the two mesh types are equal (PolyMeshT<Kernel>==OtherMesh)
      you should use PolyMeshT::operator=() instead, because its more
      efficient and \em all information is copied.
  */
  template <class OtherMesh>
  PolyMeshT& assign(const OtherMesh& _rhs)
#if OM_OUT_OF_CLASS_TEMPLATE || defined(DOXYGEN)
     ;
#else
#  include "PolyMeshT_assign.hh"
#endif

  //@}





  // --- deletion ---

  /// \name Deleting mesh items
  //@{

  /** Mark vertex and all incident edges and faces deleted.
      Items marked deleted will be removed by garbageCollection().
      \attention Needs the Attributes::Status attribute for vertices,
      edges and faces.
  */
  void delete_vertex(VertexHandle _vh, bool _delete_isolated_vertices = true);

  /** Mark edge (two opposite halfedges) and incident faces deleted.
      Resulting isolated vertices are marked deleted if
      _delete_isolated_vertices is true. Items marked deleted will be
      removed by garbageCollection().

      \attention Needs the Attributes::Status attribute for vertices,
      edges and faces.
  */
  void delete_edge(EdgeHandle _eh, bool _delete_isolated_vertices=true);

  /** Delete face _fh and resulting degenerated empty halfedges as
      well.  Resultling isolated vertices will be deleted if
      _delete_isolated_vertices is true.

      \attention All item will only be marked to be deleted. They will
      actually be removed by calling garbage_collection().

      \attention Needs the Attributes::Status attribute for vertices,
      edges and faces.
  */
  void delete_face(FaceHandle _fh, bool _delete_isolated_vertices=true);

  //@}




  // --- iterators begin() and end() ---


  /** \name Begin and end iterators
  */
  //@{

  /// Begin iterator for vertices
  VertexIter vertices_begin()
  { return VertexIter(*this, VertexHandle(0)); }
  /// Const begin iterator for vertices
  ConstVertexIter vertices_begin() const
  { return ConstVertexIter(*this, VertexHandle(0)); }
  /// End iterator for vertices
  VertexIter vertices_end()
  { return VertexIter(*this, VertexHandle(This::n_vertices())); }
  /// Const end iterator for vertices
  ConstVertexIter vertices_end() const
  { return ConstVertexIter(*this, VertexHandle(This::n_vertices())); }

  /// Begin iterator for halfedges
  HalfedgeIter halfedges_begin()
  { return HalfedgeIter(*this, HalfedgeHandle(0)); }
  /// Const begin iterator for halfedges
  ConstHalfedgeIter halfedges_begin() const
  { return ConstHalfedgeIter(*this, HalfedgeHandle(0)); }
  /// End iterator for halfedges
  HalfedgeIter halfedges_end()
  { return HalfedgeIter(*this, HalfedgeHandle(This::n_halfedges())); }
  /// Const end iterator for halfedges
  ConstHalfedgeIter halfedges_end() const
  { return ConstHalfedgeIter(*this, HalfedgeHandle(This::n_halfedges())); }

  /// Begin iterator for edges
  EdgeIter edges_begin()
  { return EdgeIter(*this, EdgeHandle(0)); }
  /// Const begin iterator for edges
  ConstEdgeIter edges_begin() const
  { return ConstEdgeIter(*this, EdgeHandle(0)); }
  /// End iterator for edges
  EdgeIter edges_end()
  { return EdgeIter(*this, EdgeHandle(This::n_edges())); }
  /// Const end iterator for edges
  ConstEdgeIter edges_end() const
  { return ConstEdgeIter(*this, EdgeHandle(This::n_edges())); }

  /// Begin iterator for faces
  FaceIter faces_begin()
  { return FaceIter(*this, FaceHandle(0)); }
  /// Const begin iterator for faces
  ConstFaceIter faces_begin() const
  { return ConstFaceIter(*this, FaceHandle(0)); }
  /// End iterator for faces
  FaceIter faces_end()
  { return FaceIter(*this, FaceHandle(This::n_faces())); }
  /// Const end iterator for faces
  ConstFaceIter faces_end() const
  { return ConstFaceIter(*this, FaceHandle(This::n_faces())); }

  //@}



  /** \name Begin for skipping iterators
  */
  //@{

  /// Begin iterator for vertices
  VertexIter vertices_sbegin()
  { return VertexIter(*this, VertexHandle(0), true); }
  /// Const begin iterator for vertices
  ConstVertexIter vertices_sbegin() const
  { return ConstVertexIter(*this, VertexHandle(0), true); }

  /// Begin iterator for halfedges
  HalfedgeIter halfedges_sbegin()
  { return HalfedgeIter(*this, HalfedgeHandle(0), true); }
  /// Const begin iterator for halfedges
  ConstHalfedgeIter halfedges_sbegin() const
  { return ConstHalfedgeIter(*this, HalfedgeHandle(0), true); }

  /// Begin iterator for edges
  EdgeIter edges_sbegin()
  { return EdgeIter(*this, EdgeHandle(0), true); }
  /// Const begin iterator for edges
  ConstEdgeIter edges_sbegin() const
  { return ConstEdgeIter(*this, EdgeHandle(0), true); }

  /// Begin iterator for faces
  FaceIter faces_sbegin()
  { return FaceIter(*this, FaceHandle(0), true); }
  /// Const begin iterator for faces
  ConstFaceIter faces_sbegin() const
  { return ConstFaceIter(*this, FaceHandle(0), true); }

  //@}




  //--- circulators ---

  /** \name Vertex and Face circulators
  */
  //@{

  /// vertex - vertex circulator
  VertexVertexIter vv_iter(VertexHandle _vh) {
    return VertexVertexIter(*this, _vh); }
  /// vertex - incoming halfedge circulator
  VertexIHalfedgeIter vih_iter(VertexHandle _vh) {
    return VertexIHalfedgeIter(*this, _vh); }
  /// vertex - outgoing halfedge circulator
  VertexOHalfedgeIter voh_iter(VertexHandle _vh) {
    return VertexOHalfedgeIter(*this, _vh); }
  /// vertex - edge circulator
  VertexEdgeIter ve_iter(VertexHandle _vh) {
    return VertexEdgeIter(*this, _vh); }
  /// vertex - face circulator
  VertexFaceIter vf_iter(VertexHandle _vh) {
    return VertexFaceIter(*this, _vh); }

  /// const vertex circulator
  ConstVertexVertexIter cvv_iter(VertexHandle _vh) const {
    return ConstVertexVertexIter(*this, _vh); }
  /// const vertex - incoming halfedge circulator
  ConstVertexIHalfedgeIter cvih_iter(VertexHandle _vh) const {
    return ConstVertexIHalfedgeIter(*this, _vh); }
  /// const vertex - outgoing halfedge circulator
  ConstVertexOHalfedgeIter cvoh_iter(VertexHandle _vh) const {
    return ConstVertexOHalfedgeIter(*this, _vh); }
  /// const vertex - edge circulator
  ConstVertexEdgeIter cve_iter(VertexHandle _vh) const {
    return ConstVertexEdgeIter(*this, _vh); }
  /// const vertex - face circulator
  ConstVertexFaceIter cvf_iter(VertexHandle _vh) const {
    return ConstVertexFaceIter(*this, _vh); }

  /// face - vertex circulator
  FaceVertexIter fv_iter(FaceHandle _fh) {
    return FaceVertexIter(*this, _fh); }
  /// face - halfedge circulator
  FaceHalfedgeIter fh_iter(FaceHandle _fh) {
    return FaceHalfedgeIter(*this, _fh); }
  /// face - edge circulator
  FaceEdgeIter fe_iter(FaceHandle _fh) {
    return FaceEdgeIter(*this, _fh); }
  /// face - face circulator
  FaceFaceIter ff_iter(FaceHandle _fh) {
    return FaceFaceIter(*this, _fh); }

  /// const face - vertex circulator
  ConstFaceVertexIter cfv_iter(FaceHandle _fh) const {
    return ConstFaceVertexIter(*this, _fh); }
  /// const face - halfedge circulator
  ConstFaceHalfedgeIter cfh_iter(FaceHandle _fh) const {
    return ConstFaceHalfedgeIter(*this, _fh); }
  /// const face - edge circulator
  ConstFaceEdgeIter cfe_iter(FaceHandle _fh) const {
    return ConstFaceEdgeIter(*this, _fh); }
  /// const face - face circulator
  ConstFaceFaceIter cff_iter(FaceHandle _fh) const {
    return ConstFaceFaceIter(*this, _fh); }
  //@}




  // --- boundary / manifold checks ---

  /** \name Boundary and manifold tests
  */
  //@{
  bool is_boundary(HalfedgeHandle _heh) const
  {
    return Kernel::is_boundary(_heh);
  }

  /** Is the edge _eh a boundary edge, i.e. is one of its halfedges
      a boundary halfege ? */
  bool is_boundary(EdgeHandle _eh) const {
    return (is_boundary(halfedge_handle(_eh, 0)) ||
            is_boundary(halfedge_handle(_eh, 1)));
  }
  /// Is vertex _vh a boundary vertex ?
  bool is_boundary(VertexHandle _vh) const {
    HalfedgeHandle heh(halfedge_handle(_vh));
    return (!(heh.is_valid() && face_handle(heh).is_valid()));
  }

  /** Is face _fh at boundary, i.e. is one of its edges (or vertices)
   *   a boundary edge?
   *  \param _fh Check this face
   *  \param _check_vertex If \c true, check the corner vertices of
   *         the face, too.
   */
  bool is_boundary(FaceHandle _fh, bool _check_vertex=false) const
  {
     for (ConstFaceEdgeIter cfeit = cfe_iter( _fh ); cfeit; ++cfeit)
        if (is_boundary( cfeit.handle() ) )
           return true;

     if (_check_vertex)
     {
        for (ConstFaceVertexIter cfvit = cfv_iter( _fh ); cfvit; ++cfvit)
           if (is_boundary( cfvit.handle() ) )
              return true;
     }
     return false;
  }

  /// Is (the mesh at) vertex _vh  two-manifold ?
  bool is_manifold(VertexHandle _vh) const
  {
    /* The vertex is non-manifold if more than one gap exists, i.e.
       more than one outgoing boundary halfedge. If (at least) one
       boundary halfedge exists, the vertex' halfedge must be a
       boundary halfedge. If iterating around the vertex finds another
       boundary halfedge, the vertex is non-manifold. */

    ConstVertexOHalfedgeIter vh_it(*this, _vh);
    if (vh_it)
      for (++vh_it; vh_it; ++vh_it)
          if (is_boundary(vh_it.handle()))
            return false;
    return true;
  }

  //@}




  // --- normal vectors ---

  /** \name Normal vector computation
  */
  //@{

  /// Different methods for calculation of the normal at a vertex
  enum VertexNormalMode 
  {
    FAST, ///< the default one, needs the Attributes::Normal attribute for faces also
    CORRECT, ///< works properly for non-triangular meshes, does not need any attributes
    ANGLE_WEIGHTED, ///< computes vertex normals as angle weighted averages of face normals, therefore needs face normals, too
    LOOP ///< calculates Loop surface normals, does not need any attributes
  };


  /** Calls update_face_normals() and update_vertex_normals() if
      these normals (i.e. the properties) exist */
  void update_normals(VertexNormalMode _mode=FAST);

  /// Update normal for face _fh
  void update_normal(FaceHandle _fh)
  {
    set_normal(_fh, calc_face_normal(_fh));
  }

  /** Update normal vectors for all faces.
      \attention Needs the Attributes::Normal attribute for faces.
  */
  void update_face_normals();

  /** Calculate normal vector for face _fh. */
  Normal calc_face_normal(FaceHandle _fh) const;

  /** Calculate normal vector for face (_p0, _p1, _p2). */
  Normal calc_face_normal(const Point& _p0, 
			  const Point& _p1,
			  const Point& _p2) const;

  /// Update normal for vertex _vh
  void update_normal(VertexHandle _vh)
  {
    set_normal(_vh, calc_vertex_normal(_vh));
  }


  /** Update normal vectors for all vertices. \attention Needs the
      Attributes::Normal attribute for faces and vertices.
  */
  void update_vertex_normals(VertexNormalMode _mode=FAST);


  /** Calculate normal vector for vertex _vh by averaging normals
      of adjacent faces. Face normals have to be computed first.
      \attention Needs the Attributes::Normal attribute for faces.
  */
  Normal calc_vertex_normal(VertexHandle _vh) const;


  void calc_vertex_normal_fast(VertexHandle _vh, Normal& _n) const;
  void calc_vertex_normal_correct(VertexHandle _vh, Normal& _n) const;
  void calc_vertex_normal_loop(VertexHandle _vh, Normal& _n) const;
  void calc_vertex_normal_angle_weighted(VertexHandle _vh, Normal& _n) const;


  //@}

  // --- Geometry API - still in development ---

  /** Calculates the edge vector as the vector defined by
      the halfedge with id #0 (see below)
  */
  void calc_edge_vector(EdgeHandle _eh, Normal& _edge_vec) const
  {
    calc_edge_vector(halfedge_handle(_eh,0), _edge_vec);
  }

  /** Calculates the edge vector as the difference of the
      the points defined by to_vertex_handle() and from_vertex_handle()
  */
  void calc_edge_vector(HalfedgeHandle _heh, Normal& _edge_vec) const
  {
    _edge_vec = (Normal)point(to_vertex_handle(_heh));
    _edge_vec -= (Normal)point(from_vertex_handle(_heh));
  }

  /** Calculates the length of the edge _eh
  */
  Scalar calc_edge_length(EdgeHandle _eh) const
  {
    return calc_edge_length(halfedge_handle(_eh,0));
  }

  /** Calculates the length of the edge _heh
  */
  Scalar calc_edge_length(HalfedgeHandle _heh) const
  {
    return (Scalar)sqrt(calc_edge_sqr_length(_heh));
  }

  Scalar calc_edge_sqr_length(EdgeHandle _eh) const
  {
    return calc_edge_sqr_length(halfedge_handle(_eh,0));
  }

  Scalar calc_edge_sqr_length(HalfedgeHandle _heh) const
  {
    Normal edge_vec;
    calc_edge_vector(_heh, edge_vec);
    return edge_vec.sqrnorm();
  }

  /** defines a consistent representation of a sector geometry:
      the halfedge _in_heh defines the sector orientation
      the vertex pointed by _in_heh defines the sector center
      _vec0 and _vec1 are resp. the first and the second vectors defining the sector
  */
  void calc_sector_vectors(HalfedgeHandle _in_heh, Normal& _vec0, Normal& _vec1) const
  {
    calc_edge_vector(next_halfedge_handle(_in_heh), _vec0);//p2 - p1
    calc_edge_vector(opposite_halfedge_handle(_in_heh), _vec1);//p0 - p1
  }

  /** calculates the sector angle
      NOTE: only boundary concave sectors are treated correctly
  */
  Scalar calc_sector_angle(HalfedgeHandle _in_heh) const
  {
    Normal v0, v1;
    calc_sector_vectors(_in_heh, v0, v1);
    Scalar denom = v0.norm()*v1.norm();
    if (is_zero(denom))
    {
      return 0;
    }
    Scalar cos_a = v0*v1/denom;
    if (is_boundary(_in_heh))
    {//determine if the boundary sector is concave or convex
      FaceHandle fh(face_handle(opposite_halfedge_handle(_in_heh)));
      Normal f_n(calc_face_normal(fh));//this normal is (for convex fh) OK
      Scalar sign_a = dot(cross(v0, v1), f_n);
      return angle(cos_a, sign_a);
    }
    else
    {
      return acos(sane_aarg(cos_a));
    }
  }

  /** calculate the cos and the sin of angle <(_in_heh,next_halfedge(_in_heh))
  */
  /*
  void calc_sector_angle_cos_sin(HalfedgeHandle _in_heh, Scalar& _cos_a, Scalar& _sin_a) const
  {
    Normal in_vec, out_vec;
    calc_edge_vector(_in_heh, in_vec);
    calc_edge_vector(next_halfedge_handle(_in_heh), out_vec);
    Scalar denom = in_vec.norm()*out_vec.norm();
    if (is_zero(denom))
    {
      _cos_a = 1;
      _sin_a = 0;
    }
    else
    {
      _cos_a = dot(in_vec, out_vec)/denom;
      _sin_a = cross(in_vec, out_vec).norm()/denom;
    }
  }
  */
  /** calculates the normal (non-normalized) of the face sector defined by
      the angle <(_in_heh,next_halfedge(_in_heh))
  */
  void calc_sector_normal(HalfedgeHandle _in_heh, Normal& _sector_normal) const
  {
    Normal vec0, vec1;
    calc_sector_vectors(_in_heh, vec0, vec1);
    _sector_normal = cross(vec0, vec1);//(p2-p1)^(p0-p1)
  }

  /** calculates the area of the face sector defined by
      the angle <(_in_heh,next_halfedge(_in_heh))
      NOTE: special cases (e.g. concave sectors) are not handled correctly
  */
  Scalar calc_sector_area(HalfedgeHandle _in_heh) const
  {
    Normal sector_normal;
    calc_sector_normal(_in_heh, sector_normal);
    return sector_normal.norm()/2;
  }

  /** calculates the dihedral angle on the halfedge _heh
  */
  Scalar calc_dihedral_angle(HalfedgeHandle _heh) const
  {
    if (is_boundary(edge_handle(_heh)))
    {//the dihedral angle at a boundary edge is 0
      return 0;
    }
    Normal n0, n1, he;
    calc_sector_normal(_heh, n0);
    calc_sector_normal(opposite_halfedge_handle(_heh), n1);
    calc_edge_vector(_heh, he);
    Scalar denom = n0.norm()*n1.norm();
    if (is_zero(denom))
    {
      return 0;
    }
    Scalar da_cos = dot(n0, n1)/denom;
    //should be normalized, but we need only the sign
    Scalar da_sin_sign = cross(n0, n1)*he;
    return angle(da_cos, da_sin_sign);
  }

  /** calculates the dihedral angle on the edge _eh
  */
  Scalar calc_dihedral_angle(EdgeHandle _eh) const
  {
    return calc_dihedral_angle(halfedge_handle(_eh,0));
  }

  /** tags an edge as a feature if its dihedral angle is larger than _angle_tresh
      returns the number of the found feature edges, requires edge_status property*/
  unsigned int find_feature_edges(Scalar _angle_tresh = deg_to_rad(44.0));
  // --- misc ---

  /// Find halfedge from _vh0 to _vh1. Returns invalid handle if not found.
  HalfedgeHandle find_halfedge(VertexHandle _start_vertex_handle,
                               VertexHandle _end_vertex_habdle) const;

  /** Adjust outgoing halfedge handle for vertices, so that it is a
      boundary halfedge whenever possible. */
  void adjust_outgoing_halfedge(VertexHandle _vh);

  /// Face split (= 1-to-n split)
  void split(FaceHandle _fh, VertexHandle _vh);
  /// Face split (= 1-to-n split)
  void split(FaceHandle _fh, const Point& _p) { split(_fh, add_vertex(_p)); }

  /// triangulate the face _fh
  void triangulate(FaceHandle _fh);

  /// triangulate the entire mesh
  void triangulate();


  /// Vertex valence
  unsigned int valence(VertexHandle _vh) const;

  /// Face valence
  unsigned int valence(FaceHandle _fh) const;


  /** \name Generic handle derefertiation.
      Calls the respective vertex(), halfedge(), edge(), face()
      method of the mesh kernel.
  */
  //@{
  /// Get item from handle
  const Vertex&    deref(VertexHandle _h)   const { return vertex(_h); }
  Vertex&          deref(VertexHandle _h)         { return vertex(_h); }
  const Halfedge&  deref(HalfedgeHandle _h) const { return halfedge(_h); }
  Halfedge&        deref(HalfedgeHandle _h)       { return halfedge(_h); }
  const Edge&      deref(EdgeHandle _h)     const { return edge(_h); }
  Edge&            deref(EdgeHandle _h)           { return edge(_h); }
  const Face&      deref(FaceHandle _h)     const { return face(_h); }
  Face&            deref(FaceHandle _h)           { return face(_h); }
  //@}
};



//=============================================================================
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_POLYMESH_C)
#  define OPENMESH_POLYMESH_TEMPLATES
#  include "PolyMeshT.cc"
#endif
//=============================================================================
#endif // OPENMESH_POLYMESHT_HH defined
//=============================================================================
