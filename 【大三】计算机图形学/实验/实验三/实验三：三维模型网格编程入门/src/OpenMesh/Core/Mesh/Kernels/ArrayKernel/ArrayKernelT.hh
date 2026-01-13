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
//   $Revision: 1.6 $
//   $Date: 2005/08/11 09:40:06 $
//
//=============================================================================


//=============================================================================
//
//  CLASS ArrayKernelT
//
//=============================================================================


#ifndef OPENMESH_ARRAY_KERNEL_HH
#define OPENMESH_ARRAY_KERNEL_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <vector>



//== NAMESPACES ===============================================================


namespace OpenMesh {


//== CLASS DEFINITION =========================================================


/** \ingroup mesh_kernels_group

    Mesh kernel using arrays for mesh item storage.

    This mesh kernel uses the std::vector as container to store the
    mesh items. Therefore all handle types are internally represented
    by integers. To get the index from a handle use the handle's \c
    idx() method.

    \note For a description of the minimal kernel interface see
    OpenMesh::Mesh::BaseKernel.
    \note You do not have to use this class directly, use the predefined
    mesh-kernel combinations in \ref mesh_types_group.
    \see OpenMesh::Concepts::KernelT, \ref mesh_type
*/

template <class AttribKernel, class FinalMeshItems>
class ArrayKernelT : public AttribKernel
{
public:

  typedef ArrayKernelT<AttribKernel, FinalMeshItems>  This;
  typedef AttribKernel                                Base;

  // attributes
  typedef typename Base::HasPrevHalfedge              HasPrevHalfedge;

  // item types
  typedef typename FinalMeshItems::Vertex             Vertex;
  typedef typename FinalMeshItems::Halfedge           Halfedge;
  typedef typename FinalMeshItems::Edge               Edge;
  typedef typename FinalMeshItems::Face               Face;
  typedef typename FinalMeshItems::Point              Point;
  typedef typename FinalMeshItems::Scalar             Scalar;

  // handles
  typedef OpenMesh::VertexHandle       VertexHandle;
  typedef OpenMesh::HalfedgeHandle     HalfedgeHandle;
  typedef OpenMesh::EdgeHandle         EdgeHandle;
  typedef OpenMesh::FaceHandle         FaceHandle;



  // --- constructor/destructor ---


  ArrayKernelT() {}
  ~ArrayKernelT() { clear(); }


  void copy_topology_from(const ArrayKernelT& _other)
  {
    // copy raw data
    vertices_ = _other.vertices_;
    edges_    = _other.edges_;
    faces_    = _other.faces_;

    // resize properties
    resize(n_vertices(), n_edges(), n_faces());
  }


  // --- handle -> item ---

  VertexHandle handle(const Vertex& _v) const  {
    return VertexHandle(&_v - &vertices_.front());
  }

  HalfedgeHandle handle(const Halfedge& _he) const {
    unsigned int eh = ((char*)&_he - (char*)&edges_.front()) / sizeof(Edge);
    assert((&_he == &edges_[eh].halfedges_[0]) ||
	   (&_he == &edges_[eh].halfedges_[1]));
    return ((&_he == &edges_[eh].halfedges_[0]) ?
	    HalfedgeHandle(eh<<1) : HalfedgeHandle((eh<<1)+1));
  }

  EdgeHandle handle(const Edge& _e) const {
    return EdgeHandle(&_e - &edges_.front());
  }

  FaceHandle handle(const Face& _f) const {
    return FaceHandle(&_f - &faces_.front());
  }



#define SIGNED(x) signed( (x) )
  //checks handle validity - useful for debugging
  bool is_valid_handle(VertexHandle _vh) const
  {
    return 0 <= _vh.idx() && _vh.idx() < SIGNED(n_vertices());
  }

  bool is_valid_handle(HalfedgeHandle _heh) const
  {
    return 0 <= _heh.idx() && _heh.idx() < SIGNED(n_edges()*2);
  }

  bool is_valid_handle(EdgeHandle _eh) const
  {
    return 0 <= _eh.idx() && _eh.idx() < SIGNED(n_edges());
  }

  bool is_valid_handle(FaceHandle _fh) const
  {
    return 0 <= _fh.idx() && _fh.idx() < SIGNED(n_faces());
  }

  // --- item -> handle ---

  const Vertex& vertex(VertexHandle _vh) const
  {
    assert(is_valid_handle(_vh));
    return vertices_[_vh.idx()];
  }

  Vertex& vertex(VertexHandle _vh)
  {
    assert(is_valid_handle(_vh));
    return vertices_[_vh.idx()];
  }

  const Halfedge& halfedge(HalfedgeHandle _heh) const
  {
    assert(is_valid_handle(_heh));
    return edges_[_heh.idx() >> 1].halfedges_[_heh.idx() & 1];
  }

  Halfedge& halfedge(HalfedgeHandle _heh)
  {
    assert(is_valid_handle(_heh));
    return edges_[_heh.idx() >> 1].halfedges_[_heh.idx() & 1];
  }

  const Edge& edge(EdgeHandle _eh) const
  {
    assert(is_valid_handle(_eh));
    return edges_[_eh.idx()];
  }

  Edge& edge(EdgeHandle _eh) {
    assert(is_valid_handle(_eh));
    return edges_[_eh.idx()];
  }

  const Face& face(FaceHandle _fh) const {
    assert(is_valid_handle(_fh));
    return faces_[_fh.idx()];
  }

  Face& face(FaceHandle _fh) {
    assert(is_valid_handle(_fh));
    return faces_[_fh.idx()];
  }

#undef SIGNED



  // --- get i'th items ---

  VertexHandle vertex_handle(unsigned int _i) const {
    return (_i < n_vertices()) ? handle( vertices_[_i] ) : VertexHandle();
  }

  HalfedgeHandle halfedge_handle(unsigned int _i) const {
    return (_i < n_halfedges()) ? halfedge_handle(edge_handle(_i/2), _i%2) : HalfedgeHandle();
  }

  EdgeHandle edge_handle(unsigned int _i) const {
    return (_i < n_edges()) ? handle(edges_[_i]) : EdgeHandle();
  }

  FaceHandle face_handle(unsigned int _i) const {
    return (_i < n_faces()) ? handle(faces_[_i]) : FaceHandle();
  }


  // --- add new items ---

  void reserve( unsigned int _n_vertices,
                unsigned int _n_edges,
                unsigned int _n_faces )
  {
    vertices_.reserve(_n_vertices);
    edges_.reserve(_n_edges);
    faces_.reserve(_n_faces);

    AttribKernel::vprops_reserve(_n_vertices);
    AttribKernel::hprops_reserve(_n_edges*2);
    AttribKernel::eprops_reserve(_n_edges);
    AttribKernel::fprops_reserve(_n_faces);
  }


public:

  VertexHandle new_vertex()
  {
    vertices_.push_back(Vertex());
    vprops_resize(n_vertices());

    return handle(vertices_.back());
  }


  VertexHandle new_vertex(const Point& _p)
  {
    vertices_.push_back(Vertex());
    vprops_resize(n_vertices());

    VertexHandle vh(handle(vertices_.back()));
    set_point(vh, _p);
    return vh;
  }


  HalfedgeHandle new_edge(VertexHandle _start_vertex_handle,
                          VertexHandle _end_vertex_handle)
  {
    assert(_start_vertex_handle != _end_vertex_handle);
    edges_.push_back(Edge());
    eprops_resize(n_edges());
    hprops_resize(n_halfedges());

    EdgeHandle eh(handle(edges_.back()));
    HalfedgeHandle heh0(halfedge_handle(eh, 0));
    HalfedgeHandle heh1(halfedge_handle(eh, 1));
    set_vertex_handle(heh0, _end_vertex_handle);
    set_vertex_handle(heh1, _start_vertex_handle);
    return heh0;
  }


  FaceHandle new_face()
  {
    faces_.push_back(Face());
    fprops_resize(n_faces());
    return handle(faces_.back());
  }

  FaceHandle new_face(const Face& _f)
  {
    faces_.push_back(_f);
    fprops_resize(n_faces());
    return handle(faces_.back());
  }

public:



  // --- deletion ---

  void garbage_collection(bool _v=true, bool _e=true, bool _f=true);

  void clear() 
  {
    vertices_.clear();
    edges_.clear();
    faces_.clear();

    AttribKernel::vprops_resize(0);
    AttribKernel::eprops_resize(0);
    AttribKernel::hprops_resize(0);
    AttribKernel::fprops_resize(0);
  }

  void free_mem()
  {
    VertexContainer(vertices_).swap(vertices_);
    EdgeContainer(edges_).swap(edges_);
    FaceContainer(faces_).swap(faces_);

    Base::vprops_free_mem();
    Base::eprops_free_mem();
    Base::hprops_free_mem();
    Base::fprops_free_mem();
  }

  void resize( unsigned int _n_vertices,
	       unsigned int _n_edges,
	       unsigned int _n_faces )
  {
    vertices_.resize(_n_vertices);
    edges_.resize(_n_edges);
    faces_.resize(_n_faces);

    vprops_resize(n_vertices());
    hprops_resize(n_halfedges());
    eprops_resize(n_edges());
    fprops_resize(n_faces());
  }




  // --- number of items ---

  unsigned int n_vertices()  const { return vertices_.size(); }
  unsigned int n_halfedges() const { return 2*edges_.size(); }
  unsigned int n_edges()     const { return edges_.size(); }
  unsigned int n_faces()     const { return faces_.size(); }

  bool empty()           const { return vertices_.empty(); }
  bool vertices_empty()  const { return vertices_.empty(); }
  bool halfedges_empty() const { return edges_.empty(); }
  bool edges_empty()     const { return edges_.empty(); }
  bool faces_empty()     const { return faces_.empty(); }





  // --- vertex connectivity ---

  HalfedgeHandle halfedge_handle(VertexHandle _vh) const {
    return vertex(_vh).halfedge_handle_;
  }

  void set_halfedge_handle(VertexHandle _vh, HalfedgeHandle _heh)
  {
//     assert(is_valid_handle(_heh));
    vertex(_vh).halfedge_handle_ = _heh;
  }

  bool is_isolated(VertexHandle _vh) const
  {
    return !halfedge_handle(_vh).is_valid();
  }

  void set_isolated(VertexHandle _vh)
  {
    vertex(_vh).halfedge_handle_.invalidate();
  }


  // --- halfedge connectivity ---


  VertexHandle to_vertex_handle(HalfedgeHandle _heh) const {
    return halfedge(_heh).vertex_handle_;
  }

  VertexHandle from_vertex_handle(HalfedgeHandle _heh) const {
    return to_vertex_handle(opposite_halfedge_handle(_heh));
  }

  void set_vertex_handle(HalfedgeHandle _heh, VertexHandle _vh)
  {
    assert(is_valid_handle(_vh));
    halfedge(_heh).vertex_handle_ = _vh;
  }


  FaceHandle face_handle(HalfedgeHandle _heh) const {
    return halfedge(_heh).face_handle_;
  }

  void set_face_handle(HalfedgeHandle _heh, FaceHandle _fh)
  {
//     assert(is_valid_handle(_fh));
    halfedge(_heh).face_handle_ = _fh;
  }

  void set_boundary(HalfedgeHandle _heh)
  {
    halfedge(_heh).face_handle_.invalidate();
  }

  /// Is halfedge _heh a boundary halfedge (is its face handle invalid) ?
  bool is_boundary(HalfedgeHandle _heh) const
  {
    return !face_handle(_heh).is_valid();
  }

  HalfedgeHandle next_halfedge_handle(HalfedgeHandle _heh) const {
    return halfedge(_heh).next_halfedge_handle_;
  }

  void set_next_halfedge_handle(HalfedgeHandle _heh, HalfedgeHandle _nheh)
  {
    assert(is_valid_handle(_nheh));
//     assert(to_vertex_handle(_heh) == from_vertex_handle(_nheh));
    halfedge(_heh).next_halfedge_handle_ = _nheh;
    set_prev_halfedge_handle(_nheh, _heh);
  }


  void set_prev_halfedge_handle(HalfedgeHandle _heh, HalfedgeHandle _pheh)
  {
    assert(is_valid_handle(_pheh));
    set_prev_halfedge_handle(_heh, _pheh, HasPrevHalfedge());
  }

  void set_prev_halfedge_handle(HalfedgeHandle _heh, HalfedgeHandle _pheh,
        GenProg::True) {
    halfedge(_heh).prev_halfedge_handle_ = _pheh;
  }
  void set_prev_halfedge_handle(HalfedgeHandle _heh, HalfedgeHandle _pheh,
        GenProg::False) {}


  HalfedgeHandle prev_halfedge_handle(HalfedgeHandle _heh) const {
    return prev_halfedge_handle(_heh, HasPrevHalfedge() );
  }

  HalfedgeHandle prev_halfedge_handle(HalfedgeHandle _heh,
              GenProg::True) const {
    return halfedge(_heh).prev_halfedge_handle_;
  }

  HalfedgeHandle prev_halfedge_handle(HalfedgeHandle _heh,
              GenProg::False) const
  {
    if (is_boundary(_heh))
    {//iterating around the vertex should be faster than iterating the boundary
      HalfedgeHandle curr_heh(opposite_halfedge_handle(_heh));
      HalfedgeHandle next_heh(This::next_halfedge_handle(curr_heh));
      do
      {
        curr_heh = opposite_halfedge_handle(next_heh);
        next_heh = This::next_halfedge_handle(curr_heh);
      }
      while (next_heh != _heh);
      return curr_heh;
    }
    else
    {
      HalfedgeHandle  heh(_heh);
      HalfedgeHandle  next_heh(This::next_halfedge_handle(heh));
      while (next_heh != _heh) {
        heh = next_heh;
        next_heh = This::next_halfedge_handle(next_heh);
      }
      return heh;
    }
  }


  HalfedgeHandle opposite_halfedge_handle(HalfedgeHandle _heh) const {
    return HalfedgeHandle((_heh.idx() & 1) ? _heh.idx()-1 : _heh.idx()+1);
  }


  HalfedgeHandle ccw_rotated_halfedge_handle(HalfedgeHandle _heh) const {
    return opposite_halfedge_handle(prev_halfedge_handle(_heh));
  }


  HalfedgeHandle cw_rotated_halfedge_handle(HalfedgeHandle _heh) const {
    return next_halfedge_handle(opposite_halfedge_handle(_heh));
  }



  // --- edge connectivity ---


  HalfedgeHandle halfedge_handle(EdgeHandle _eh, unsigned int _i) const {
    assert(_i<=1);
    return HalfedgeHandle((_eh.idx() << 1) + _i);
  }


  EdgeHandle edge_handle(HalfedgeHandle _heh) const {
    return EdgeHandle(_heh.idx() >> 1);
  }



  // --- face connectivity ---


  HalfedgeHandle halfedge_handle(FaceHandle _fh) const {
    return face(_fh).halfedge_handle_;
  }

  void set_halfedge_handle(FaceHandle _fh, HalfedgeHandle _heh)
  {
//     assert(is_valid_handle(_heh));
    face(_fh).halfedge_handle_ = _heh;
  }

private:

  // iterators
  typedef std::vector<Vertex>                         VertexContainer;
  typedef std::vector<Edge>                           EdgeContainer;
  typedef std::vector<Face>                           FaceContainer;
  typedef typename VertexContainer::iterator          KernelVertexIter;
  typedef typename VertexContainer::const_iterator    KernelConstVertexIter;
  typedef typename EdgeContainer::iterator            KernelEdgeIter;
  typedef typename EdgeContainer::const_iterator      KernelConstEdgeIter;
  typedef typename FaceContainer::iterator            KernelFaceIter;
  typedef typename FaceContainer::const_iterator      KernelConstFaceIter;


  KernelVertexIter vertices_begin()             { return vertices_.begin(); }
  KernelConstVertexIter vertices_begin() const  { return vertices_.begin(); }
  KernelVertexIter vertices_end()               { return vertices_.end(); }
  KernelConstVertexIter vertices_end() const    { return vertices_.end(); }

  KernelEdgeIter edges_begin()                  { return edges_.begin(); }
  KernelConstEdgeIter edges_begin() const       { return edges_.begin(); }
  KernelEdgeIter edges_end()                    { return edges_.end(); }
  KernelConstEdgeIter edges_end() const         { return edges_.end(); }

  KernelFaceIter faces_begin()                  { return faces_.begin(); }
  KernelConstFaceIter faces_begin() const       { return faces_.begin(); }
  KernelFaceIter faces_end()                    { return faces_.end(); }
  KernelConstFaceIter faces_end() const         { return faces_.end(); }


private:

  VertexContainer    vertices_;
  EdgeContainer      edges_;
  FaceContainer      faces_;
};


//-----------------------------------------------------------------------------


template <class AttribKernel, class FinalMeshItems>
void
ArrayKernelT<AttribKernel, FinalMeshItems>::
garbage_collection(bool _v, bool _e, bool _f)
{
  int            i, i0, i1,
                 nV(n_vertices()),
                 nE(n_edges()),
                 nH(2*n_edges()),
                 nF(n_faces());

  std::vector<VertexHandle>    vh_map;
  std::vector<HalfedgeHandle>  hh_map;
  std::vector<FaceHandle>      fh_map;

  // setup handle mapping:
  vh_map.reserve(nV);
  for (i=0; i<nV; ++i) vh_map.push_back(VertexHandle(i));

  hh_map.reserve(nH);
  for (i=0; i<nH; ++i) hh_map.push_back(HalfedgeHandle(i));

  fh_map.reserve(nF);
  for (i=0; i<nF; ++i) fh_map.push_back(FaceHandle(i));


  // remove deleted vertices
  if (_v && n_vertices() > 0)
  {
    i0=0;  i1=nV-1;

    while (1)
    {
      // find 1st deleted and last un-deleted
      while (!AttribKernel::status(VertexHandle(i0)).deleted() && i0 < i1)  ++i0;
      while ( AttribKernel::status(VertexHandle(i1)).deleted() && i0 < i1)  --i1;
      if (i0 >= i1) break;

      // swap
      std::swap(vertices_[i0], vertices_[i1]);
      std::swap(vh_map[i0],  vh_map[i1]);
      AttribKernel::vprops_swap(i0, i1);
    };

    vertices_.resize(AttribKernel::status(VertexHandle(i0)).deleted() ? i0 : i0+1);
    vprops_resize(n_vertices());
  }


  // remove deleted edges
  if (_e && n_edges() > 0)
  {
    i0=0;  i1=nE-1;

    while (1)
    {
      // find 1st deleted and last un-deleted
      while (!AttribKernel::status(EdgeHandle(i0)).deleted() && i0 < i1)  ++i0;
      while ( AttribKernel::status(EdgeHandle(i1)).deleted() && i0 < i1)  --i1;
      if (i0 >= i1) break;

      // swap
      std::swap(edges_[i0], edges_[i1]);
      std::swap(hh_map[2*i0], hh_map[2*i1]);
      std::swap(hh_map[2*i0+1], hh_map[2*i1+1]);
      AttribKernel::eprops_swap(i0, i1);
      AttribKernel::hprops_swap(2*i0,   2*i1);
      AttribKernel::hprops_swap(2*i0+1, 2*i1+1);
    };

    edges_.resize(AttribKernel::status(EdgeHandle(i0)).deleted() ? i0 : i0+1);
    eprops_resize(n_edges());
    hprops_resize(n_halfedges());
  }


  // remove deleted faces
  if (_f && n_faces() > 0)
  {
    i0=0;  i1=nF-1;

    while (1)
    {
      // find 1st deleted and last un-deleted
      while (!AttribKernel::status(FaceHandle(i0)).deleted() && i0 < i1)  ++i0;
      while ( AttribKernel::status(FaceHandle(i1)).deleted() && i0 < i1)  --i1;
      if (i0 >= i1) break;

      // swap
      std::swap(faces_[i0], faces_[i1]);
      std::swap(fh_map[i0], fh_map[i1]);
      AttribKernel::fprops_swap(i0, i1);
    };

    faces_.resize(AttribKernel::status(FaceHandle(i0)).deleted() ? i0 : i0+1);
    fprops_resize(n_faces());
  }


  // update handles of vertices
  if (_e)
  {
    KernelVertexIter v_it(vertices_begin()), v_end(vertices_end());
    VertexHandle     vh;

    for (; v_it!=v_end; ++v_it)
    {
      vh = handle(*v_it);
      if (!AttribKernel::status(vh).deleted() && !is_isolated(vh))
        set_halfedge_handle(vh, hh_map[halfedge_handle(vh).idx()]);
    }
  }

  HalfedgeHandle hh;
  // update handles of halfedges
  for (KernelEdgeIter e_it(edges_begin()); e_it != edges_end(); ++e_it)
  {//in the first pass update the (half)edges vertices
    hh = halfedge_handle(handle(*e_it), 0);
    set_vertex_handle(hh, vh_map[to_vertex_handle(hh).idx()]);
    hh = halfedge_handle(handle(*e_it), 1);
    set_vertex_handle(hh, vh_map[to_vertex_handle(hh).idx()]);
  }
  for (KernelEdgeIter e_it(edges_begin()); e_it != edges_end(); ++e_it)
  {//in the second pass update the connectivity of the (half)edges
    hh = halfedge_handle(handle(*e_it), 0);
    set_next_halfedge_handle(hh, hh_map[next_halfedge_handle(hh).idx()]);
    if (!is_boundary(hh))
    {
      set_face_handle(hh, fh_map[face_handle(hh).idx()]);
    }
    hh = halfedge_handle(handle(*e_it), 1);
    set_next_halfedge_handle(hh, hh_map[next_halfedge_handle(hh).idx()]);
    if (!is_boundary(hh))
    {
      set_face_handle(hh, fh_map[face_handle(hh).idx()]);
    }
  }


  // update handles of faces
  if (_e)
  {
    KernelFaceIter  f_it(faces_begin()), f_end(faces_end());
    FaceHandle      fh;

    for (; f_it!=f_end; ++f_it)
    {
      fh = handle(*f_it);
      set_halfedge_handle(fh, hh_map[halfedge_handle(fh).idx()]);
    }
  }

}


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_ARRAY_KERNEL_HH defined
//=============================================================================
