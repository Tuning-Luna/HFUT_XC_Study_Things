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
//   $Revision: 1.5 $
//   $Date: 2005/08/11 09:37:54 $
//
//=============================================================================


//=============================================================================
//
//  CLASS TriMeshT - IMPLEMENTATION
//
//=============================================================================


#define OPENMESH_TRIMESH_C


//== INCLUDES =================================================================


#include <OpenMesh/Core/Mesh/TriMeshT.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <vector>


//== NAMESPACES ==============================================================


namespace OpenMesh {


//== IMPLEMENTATION ==========================================================


template <class Kernel>
typename TriMeshT<Kernel>::FaceHandle
TriMeshT<Kernel>::
add_face(const std::vector<VertexHandle>& _vertex_handles)
{
  unsigned int n_vertices(_vertex_handles.size());

  // need at least 3 vertices
  if (n_vertices < 3) return This::InvalidFaceHandle;

  /// face is triangle -> ok
  if (_vertex_handles.size() == 3)
    return PolyMesh::add_face(_vertex_handles);

  /// face is not a triangle -> triangulate
  else
  {
    omlog() << "triangulating " << n_vertices << "_gon\n";

    std::vector<VertexHandle>  vhandles(3);
    vhandles[0] = _vertex_handles[0];

    FaceHandle fh;
    unsigned int i(1);
    --n_vertices;

    while (i < n_vertices)
    {
      vhandles[1] = _vertex_handles[i];
      vhandles[2] = _vertex_handles[++i];
      fh = PolyMesh::add_face(vhandles);
    }

    return fh;
  }
}


//-----------------------------------------------------------------------------


template <class Kernel>
bool
TriMeshT<Kernel>::
is_collapse_ok(HalfedgeHandle v0v1)
{
  HalfedgeHandle  v1v0(opposite_halfedge_handle(v0v1));
  VertexHandle    v0(to_vertex_handle(v1v0));
  VertexHandle    v1(to_vertex_handle(v0v1));



  // are vertices already deleted ?
  if (status(v0).deleted() || status(v1).deleted())
    return false;


  VertexHandle    vl, vr;
  HalfedgeHandle  h1, h2;


  // the edges v1-vl and vl-v0 must not be both boundary edges
  if (!is_boundary(v0v1))
  {
    vl = to_vertex_handle(next_halfedge_handle(v0v1));

    h1 = next_halfedge_handle(v0v1);
    h2 = next_halfedge_handle(h1);
    if (is_boundary(opposite_halfedge_handle(h1)) &&
	is_boundary(opposite_halfedge_handle(h2)))
      return false;
  }


  // the edges v0-vr and vr-v1 must not be both boundary edges
  if (!is_boundary(v1v0))
  {
    vr = to_vertex_handle(next_halfedge_handle(v1v0));

    h1 = next_halfedge_handle(v1v0);
    h2 = next_halfedge_handle(h1);
    if (is_boundary(opposite_halfedge_handle(h1)) &&
	is_boundary(opposite_halfedge_handle(h2)))
      return false;
  }


  // if vl and vr are equal or both invalid -> fail
  if (vl == vr) return false;


  VertexVertexIter  vv_it;


  // test intersection of the one-rings of v0 and v1
  for (vv_it = vv_iter(v0); vv_it; ++vv_it)
    status(vv_it).set_tagged(false);

  for (vv_it = vv_iter(v1); vv_it; ++vv_it)
    status(vv_it).set_tagged(true);

  for (vv_it = vv_iter(v0); vv_it; ++vv_it)
    if (status(vv_it).tagged() && vv_it.handle() != vl && vv_it.handle() != vr)
      return false;



  // edge between two boundary vertices should be a boundary edge
  if ( is_boundary(v0) && is_boundary(v1) &&
       !is_boundary(v0v1) && !is_boundary(v1v0))
    return false;



  // passed all tests
  return true;
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
TriMeshT<Kernel>::
collapse(HalfedgeHandle _hh)
{
  HalfedgeHandle h0 = _hh;
  HalfedgeHandle h1 = prev_halfedge_handle(h0);
  HalfedgeHandle o0 = opposite_halfedge_handle(h0);
  HalfedgeHandle o1 = next_halfedge_handle(o0);

  // remove edge
  remove_edge(h0);

  // remove loops
  if (next_halfedge_handle(next_halfedge_handle(h1)) == h1)
    remove_loop(h1);
  if (next_halfedge_handle(next_halfedge_handle(o1)) == o1)
    remove_loop(o1);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
TriMeshT<Kernel>::
remove_edge(HalfedgeHandle _hh)
{
  HalfedgeHandle  h  = _hh;
  HalfedgeHandle  hn = next_halfedge_handle(h);
  HalfedgeHandle  hp = prev_halfedge_handle(h);

  HalfedgeHandle  o  = opposite_halfedge_handle(h);
  HalfedgeHandle  on = next_halfedge_handle(o);
  HalfedgeHandle  op = prev_halfedge_handle(o);

  FaceHandle      fh = face_handle(h);
  FaceHandle      fo = face_handle(o);

  VertexHandle    vh = to_vertex_handle(h);
  VertexHandle    vo = to_vertex_handle(o);



  // halfedge -> vertex
  for (VertexIHalfedgeIter vih_it(vih_iter(vo)); vih_it; ++vih_it)
    set_vertex_handle(vih_it.handle(), vh);


  // halfedge -> halfedge
  set_next_halfedge_handle(hp, hn);
  set_next_halfedge_handle(op, on);


  // face -> halfedge
  if (fh.is_valid())  set_halfedge_handle(fh, hn);
  if (fo.is_valid())  set_halfedge_handle(fo, on);


  // vertex -> halfedge
  if (halfedge_handle(vh) == o)  set_halfedge_handle(vh, hn);
  adjust_outgoing_halfedge(vh);
  set_isolated(vo);


  // delete stuff
  status(edge_handle(h)).set_deleted(true);
  status(vo).set_deleted(true);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
TriMeshT<Kernel>::
remove_loop(HalfedgeHandle _hh)
{
  HalfedgeHandle  h0 = _hh;
  HalfedgeHandle  h1 = next_halfedge_handle(h0);

  HalfedgeHandle  o0 = opposite_halfedge_handle(h0);
  HalfedgeHandle  o1 = opposite_halfedge_handle(h1);

  VertexHandle    v0 = to_vertex_handle(h0);
  VertexHandle    v1 = to_vertex_handle(h1);

  FaceHandle      fh = face_handle(h0);
  FaceHandle      fo = face_handle(o0);



  // is it a loop ?
  assert ((next_halfedge_handle(h1) == h0) && (h1 != o0));


  // halfedge -> halfedge
  set_next_halfedge_handle(h1, next_halfedge_handle(o0));
  set_next_halfedge_handle(prev_halfedge_handle(o0), h1);


  // halfedge -> face
  set_face_handle(h1, fo);


  // vertex -> halfedge
  set_halfedge_handle(v0, h1);  adjust_outgoing_halfedge(v0);
  set_halfedge_handle(v1, o1);  adjust_outgoing_halfedge(v1);


  // face -> halfedge
  if (fo.is_valid() && halfedge_handle(fo) == o0)
    set_halfedge_handle(fo, h1);
  //set_halfedge_handle(fh, InvalidHalfedgeHandle);//Do we need that - it will be marked as deleted below?

  // delete stuff
  if (fh.is_valid())  status(fh).set_deleted(true);
  status(edge_handle(h0)).set_deleted(true);
}


//-----------------------------------------------------------------------------


template <class Kernel>
typename TriMeshT<Kernel>::HalfedgeHandle
TriMeshT<Kernel>::
vertex_split( VertexHandle  v0,
        VertexHandle  v1,
        VertexHandle  vl,
        VertexHandle  vr )
{
  HalfedgeHandle v1vl, vlv1, vrv1, v0v1;


  // build loop from halfedge v1->vl
  if (vl.is_valid())
  {
    v1vl = find_halfedge(v1, vl);
    assert(v1vl.is_valid());
    vlv1 = insert_loop(v1vl);
  }


  // build loop from halfedge vr->v1
  if (vr.is_valid())
  {
    vrv1 = find_halfedge(vr, v1);
    assert(vrv1.is_valid());
    insert_loop(vrv1);
  }


  // handle boundary cases
  if (!vl.is_valid())
    vlv1 = prev_halfedge_handle(halfedge_handle(v1));
  if (!vr.is_valid())
    vrv1 = prev_halfedge_handle(halfedge_handle(v1));


  // split vertex v1 into edge v0v1
  v0v1 = insert_edge(v0, vlv1, vrv1);


  return v0v1;
}


//-----------------------------------------------------------------------------


template <class Kernel>
typename TriMeshT<Kernel>::HalfedgeHandle
TriMeshT<Kernel>::
insert_loop(HalfedgeHandle _hh)
{
  HalfedgeHandle  h0(_hh);
  HalfedgeHandle  o0(opposite_halfedge_handle(h0));

  VertexHandle    v0(to_vertex_handle(o0));
  VertexHandle    v1(to_vertex_handle(h0));

  HalfedgeHandle  h1 = new_edge(v1, v0);
  HalfedgeHandle  o1 = opposite_halfedge_handle(h1);

  FaceHandle      f0 = face_handle(h0);
  FaceHandle      f1 = This::new_face();



  // halfedge -> halfedge
  set_next_halfedge_handle(prev_halfedge_handle(h0), o1);
  set_next_halfedge_handle(o1, next_halfedge_handle(h0));
  set_next_halfedge_handle(h1, h0);
  set_next_halfedge_handle(h0, h1);


  // halfedge -> face
  set_face_handle(o1, f0);
  set_face_handle(h0, f1);
  set_face_handle(h1, f1);


  // face -> halfedge
  set_halfedge_handle(f1, h0);
  if (f0.is_valid())
    set_halfedge_handle(f0, o1);


  // vertex -> halfedge
  adjust_outgoing_halfedge(v0);
  adjust_outgoing_halfedge(v1);


  return h1;
}


//-----------------------------------------------------------------------------


template <class Kernel>
typename TriMeshT<Kernel>::HalfedgeHandle
TriMeshT<Kernel>::
insert_edge(VertexHandle   _vh,
      HalfedgeHandle _h0,
      HalfedgeHandle _h1)
{
  assert(_h0.is_valid() && _h1.is_valid());

  VertexHandle  v0 = _vh;
  VertexHandle  v1 = to_vertex_handle(_h0);

  assert( v1 == to_vertex_handle(_h1));

  HalfedgeHandle v0v1 = new_edge(v0, v1);
  HalfedgeHandle v1v0 = opposite_halfedge_handle(v0v1);



  // vertex -> halfedge
  set_halfedge_handle(v0, v0v1);
  set_halfedge_handle(v1, v1v0);


  // halfedge -> halfedge
  set_next_halfedge_handle(v0v1, next_halfedge_handle(_h0));
  set_next_halfedge_handle(_h0, v0v1);
  set_next_halfedge_handle(v1v0, next_halfedge_handle(_h1));
  set_next_halfedge_handle(_h1, v1v0);


  // halfedge -> vertex
  for (VertexIHalfedgeIter vih_it(vih_iter(v0)); vih_it; ++vih_it)
    set_vertex_handle(vih_it.handle(), v0);


  // halfedge -> face
  set_face_handle(v0v1, face_handle(_h0));
  set_face_handle(v1v0, face_handle(_h1));


  // face -> halfedge
  if (face_handle(v0v1).is_valid())
    set_halfedge_handle(face_handle(v0v1), v0v1);
  if (face_handle(v1v0).is_valid())
    set_halfedge_handle(face_handle(v1v0), v1v0);


  // vertex -> halfedge
  adjust_outgoing_halfedge(v0);
  adjust_outgoing_halfedge(v1);


  return v0v1;
}


//-----------------------------------------------------------------------------


template <class Impl>
bool
TriMeshT<Impl>::
is_flip_ok(EdgeHandle _eh) const
{
  // boundary edges cannot be flipped
  if (is_boundary(_eh)) return false;


  HalfedgeHandle hh = halfedge_handle(_eh, 0);
  HalfedgeHandle oh = halfedge_handle(_eh, 1);


  // check if the flipped edge is already present
  // in the mesh

  VertexHandle ah = to_vertex_handle(next_halfedge_handle(hh));
  VertexHandle bh = to_vertex_handle(next_halfedge_handle(oh));

  if (ah == bh)   // this is generally a bad sign !!!
    return false;

  for (ConstVertexVertexIter vvi(*this, ah); vvi; ++vvi)
    if (vvi.handle() == bh)
      return false;

  return true;
}


//-----------------------------------------------------------------------------


template <class Impl>
void
TriMeshT<Impl>::
flip(EdgeHandle _eh)
{
  // CAUTION : Flipping a halfedge may result in
  // a non-manifold mesh, hence check for yourself
  // whether this operation is allowed or not!
  assert(is_flip_ok(_eh));//let's make it sure it is actually checked
  assert(!is_boundary(_eh));

  HalfedgeHandle a0 = halfedge_handle(_eh, 0);
  HalfedgeHandle b0 = halfedge_handle(_eh, 1);

  HalfedgeHandle a1 = next_halfedge_handle(a0);
  HalfedgeHandle a2 = next_halfedge_handle(a1);

  HalfedgeHandle b1 = next_halfedge_handle(b0);
  HalfedgeHandle b2 = next_halfedge_handle(b1);

  VertexHandle   va0 = to_vertex_handle(a0);
  VertexHandle   va1 = to_vertex_handle(a1);

  VertexHandle   vb0 = to_vertex_handle(b0);
  VertexHandle   vb1 = to_vertex_handle(b1);

  FaceHandle     fa  = face_handle(a0);
  FaceHandle     fb  = face_handle(b0);

  set_vertex_handle(a0, va1);
  set_vertex_handle(b0, vb1);

  set_next_halfedge_handle(a0, a2);
  set_next_halfedge_handle(a2, b1);
  set_next_halfedge_handle(b1, a0);

  set_next_halfedge_handle(b0, b2);
  set_next_halfedge_handle(b2, a1);
  set_next_halfedge_handle(a1, b0);

  set_face_handle(a1, fb);
  set_face_handle(b1, fa);

  set_halfedge_handle(fa, a0);
  set_halfedge_handle(fb, b0);

  if (halfedge_handle(va0) == b0)
    set_halfedge_handle(va0, a1);
  if (halfedge_handle(vb0) == a0)
    set_halfedge_handle(vb0, b1);
}


//-----------------------------------------------------------------------------


template <class Impl>
void
TriMeshT<Impl>::
split(EdgeHandle _eh, VertexHandle _vh)
{
  HalfedgeHandle h0 = halfedge_handle(_eh, 0);
  HalfedgeHandle o0 = halfedge_handle(_eh, 1);

  VertexHandle   v2 = to_vertex_handle(o0);

  HalfedgeHandle e1 = new_edge(_vh, v2);
  HalfedgeHandle t1 = opposite_halfedge_handle(e1);

  FaceHandle     f0 = face_handle(h0);
  FaceHandle     f3 = face_handle(o0);

  set_halfedge_handle(_vh, h0);
  set_vertex_handle(o0, _vh);

  if (!is_boundary(h0))
  {
    HalfedgeHandle h1 = next_halfedge_handle(h0);
    HalfedgeHandle h2 = next_halfedge_handle(h1);

    VertexHandle v1 = to_vertex_handle(h1);

    HalfedgeHandle e0 = new_edge(_vh, v1);
    HalfedgeHandle t0 = opposite_halfedge_handle(e0);

    FaceHandle f1 = This::new_face();
    set_halfedge_handle(f0, h0);
    set_halfedge_handle(f1, h2);

    set_face_handle(h1, f0);
    set_face_handle(t0, f0);
    set_face_handle(h0, f0);

    set_face_handle(h2, f1);
    set_face_handle(t1, f1);
    set_face_handle(e0, f1);

    set_next_halfedge_handle(h0, h1);
    set_next_halfedge_handle(h1, t0);
    set_next_halfedge_handle(t0, h0);

    set_next_halfedge_handle(e0, h2);
    set_next_halfedge_handle(h2, t1);
    set_next_halfedge_handle(t1, e0);
  }
  else
  {
    set_next_halfedge_handle(prev_halfedge_handle(h0), t1);
    set_next_halfedge_handle(t1, h0);
    // halfedge handle of _vh already is h0
  }


  if (!is_boundary(o0))
  {
    HalfedgeHandle o1 = next_halfedge_handle(o0);
    HalfedgeHandle o2 = next_halfedge_handle(o1);

    VertexHandle v3 = to_vertex_handle(o1);

    HalfedgeHandle e2 = new_edge(_vh, v3);
    HalfedgeHandle t2 = opposite_halfedge_handle(e2);

    FaceHandle f2 = This::new_face();
    set_halfedge_handle(f2, o1);
    set_halfedge_handle(f3, o0);

    set_face_handle(o1, f2);
    set_face_handle(t2, f2);
    set_face_handle(e1, f2);

    set_face_handle(o2, f3);
    set_face_handle(o0, f3);
    set_face_handle(e2, f3);

    set_next_halfedge_handle(e1, o1);
    set_next_halfedge_handle(o1, t2);
    set_next_halfedge_handle(t2, e1);

    set_next_halfedge_handle(o0, e2);
    set_next_halfedge_handle(e2, o2);
    set_next_halfedge_handle(o2, o0);
  }
  else
  {
    set_next_halfedge_handle(e1, next_halfedge_handle(o0));
    set_next_halfedge_handle(o0, e1);
    set_halfedge_handle(_vh, e1);
  }

  if (halfedge_handle(v2) == h0)
    set_halfedge_handle(v2, t1);
}

//-----------------------------------------------------------------------------


#if OM_OUT_OF_CLASS_TEMPLATE
template <typename Kernel_>
template <typename OtherMesh>
TriMeshT<Kernel_>&
TriMeshT<Kernel_>::
assign(const OtherMesh& _rhs)
#  include "PolyMeshT_assign.hh"
#endif

//=============================================================================
} // namespace OpenMesh
//=============================================================================
