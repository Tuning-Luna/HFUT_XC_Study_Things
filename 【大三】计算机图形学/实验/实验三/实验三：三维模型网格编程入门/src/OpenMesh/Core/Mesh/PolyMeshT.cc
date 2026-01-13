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
//   $Date: 2005/04/18 09:07:09 $
//
//=============================================================================


//=============================================================================
//
//  CLASS PolyMeshT - IMPLEMENTATION
//
//=============================================================================


#define OPENMESH_POLYMESH_C


//== INCLUDES =================================================================

#include <OpenMesh/Core/Mesh/PolyMeshT.hh>
#include <OpenMesh/Core/Math/LoopSchemeMaskT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <vector>


//== NAMESPACES ===============================================================


namespace OpenMesh {


//== IMPLEMENTATION ==========================================================


template <class Kernel>
const typename PolyMeshT<Kernel>::VertexHandle
PolyMeshT<Kernel>::InvalidVertexHandle;

template <class Kernel>
const typename PolyMeshT<Kernel>::HalfedgeHandle
PolyMeshT<Kernel>::InvalidHalfedgeHandle;

template <class Kernel>
const typename PolyMeshT<Kernel>::EdgeHandle
PolyMeshT<Kernel>::InvalidEdgeHandle;

template <class Kernel>
const typename PolyMeshT<Kernel>::FaceHandle
PolyMeshT<Kernel>::InvalidFaceHandle;



template <class Kernel_>
uint PolyMeshT<Kernel_>::find_feature_edges(Scalar _angle_tresh)
{
  assert(This::has_edge_status());//this function needs edge status property
  uint n_feature_edges = 0;
  for (EdgeIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
  {
    if (fabs(calc_dihedral_angle(e_it)) > _angle_tresh)
    {//note: could be optimized by comparing cos(dih_angle) vs. cos(_angle_tresh)
      status(e_it).set_feature(true);
      n_feature_edges++;
    }
    else
    {
      status(e_it).set_feature(false);
    }
  }
  return n_feature_edges;
}

//-----------------------------------------------------------------------------


template <class Kernel_>
typename PolyMeshT<Kernel_>::HalfedgeHandle
PolyMeshT<Kernel_>::
find_halfedge( VertexHandle _start_vertex_handle,
               VertexHandle _end_vertex_handle ) const
{
  assert(_start_vertex_handle.is_valid() && _end_vertex_handle.is_valid());

  for (ConstVertexVertexIter vvIt=cvv_iter(_start_vertex_handle); vvIt; ++vvIt)
    if (vvIt.handle() == _end_vertex_handle)
      return vvIt.current_halfedge_handle();

  return InvalidHalfedgeHandle;
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
adjust_outgoing_halfedge(VertexHandle _vh)
{
  for (ConstVertexOHalfedgeIter vh_it=cvoh_iter(_vh); vh_it; ++vh_it)
  {
    if (is_boundary(vh_it.handle()))
    {
      set_halfedge_handle(_vh, vh_it.handle());
      break;
    }
  }
}


//-----------------------------------------------------------------------------


namespace add_face_ns
{
  template <class _Handle>
  struct NextCacheEntryT : public std::pair<_Handle, _Handle>
  {
    typedef std::pair<_Handle, _Handle> Base;

    NextCacheEntryT(_Handle _heh0, _Handle _heh1)
      : Base(_heh0, _heh1)
    {
      assert(_heh0.is_valid());
      assert(_heh1.is_valid());
    }
  };
};


template <class Kernel>
typename PolyMeshT<Kernel>::FaceHandle
PolyMeshT<Kernel>::
add_face(const std::vector<VertexHandle>& _vertex_handles)
{
  VertexHandle                   vh;
  unsigned int                   i, ii, n(_vertex_handles.size()), id;
  std::vector<HalfedgeHandle>    halfedge_handles(n);
  std::vector<bool>              is_new(n), needs_adjust(n, false);
  HalfedgeHandle                 inner_next, inner_prev,
                                 outer_next, outer_prev,
                                 boundary_next, boundary_prev,
                                 patch_start, patch_end;

  // cache for set_next_halfedge and vertex' set_halfedge
  typedef add_face_ns::NextCacheEntryT<HalfedgeHandle> NextCacheEntry;
  typedef std::vector<NextCacheEntry>                  NextCache;

  NextCache    next_cache;
  next_cache.reserve(3*n);


  // don't allow degenerated faces
  assert (n > 2);


  // test for topological errors
  for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
  {
    if ( !is_boundary(_vertex_handles[i]) )
    {
      omerr() << "PolyMeshT::add_face: complex vertex\n";
      return InvalidFaceHandle;
    }

    halfedge_handles[i] = find_halfedge(_vertex_handles[i],
                                        _vertex_handles[ii]);
    is_new[i] = !halfedge_handles[i].is_valid();

    if (!is_new[i] && !is_boundary(halfedge_handles[i]))
    {
      omerr() << "PolyMeshT::add_face: complex edge\n";
      return InvalidFaceHandle;
    }
  }


  // re-link patches if necessary
  for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
  {
    if (!is_new[i] && !is_new[ii])
    {
      inner_prev = halfedge_handles[i];
      inner_next = halfedge_handles[ii];

      if (next_halfedge_handle(inner_prev) != inner_next)
      {
        // here comes the ugly part... we have to relink a whole patch

        // search a free gap
        // free gap will be between boundary_prev and boundary_next
        outer_prev = opposite_halfedge_handle(inner_next);
        outer_next = opposite_halfedge_handle(inner_prev);
        boundary_prev = outer_prev;
        do
          boundary_prev =
            opposite_halfedge_handle(next_halfedge_handle(boundary_prev));
        while (!is_boundary(boundary_prev) || boundary_prev==inner_prev);
        boundary_next = next_halfedge_handle(boundary_prev);
        assert(is_boundary(boundary_prev));
        assert(is_boundary(boundary_next));


        // ok ?
        if (boundary_next == inner_next)
        {
          omerr() << "PolyMeshT::add_face: patch re-linking failed\n";
          return InvalidFaceHandle;
        }

        // other halfedges' handles
        patch_start = next_halfedge_handle(inner_prev);
        patch_end   = prev_halfedge_handle(inner_next);

        // relink
        next_cache.push_back(NextCacheEntry(boundary_prev, patch_start));
        next_cache.push_back(NextCacheEntry(patch_end, boundary_next));
        next_cache.push_back(NextCacheEntry(inner_prev, inner_next));
      }
    }
  }



  // create missing edges
  for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
    if (is_new[i])
      halfedge_handles[i] = new_edge(_vertex_handles[i], _vertex_handles[ii]);



  // create the face
  FaceHandle fh(This::new_face());
  set_halfedge_handle(fh, halfedge_handles[n-1]);



  // setup halfedges
  for (i=0, ii=1; i<n; ++i, ++ii, ii%=n)
  {
    vh         = _vertex_handles[ii];
    inner_prev = halfedge_handles[i];
    inner_next = halfedge_handles[ii];

    id = 0;
    if (is_new[i])  id |= 1;
    if (is_new[ii]) id |= 2;

    if (id)
    {
      outer_prev = opposite_halfedge_handle(inner_next);
      outer_next = opposite_halfedge_handle(inner_prev);

      // set outer links
      switch (id)
      {
        case 1: // prev is new, next is old
          boundary_prev = prev_halfedge_handle(inner_next);
          next_cache.push_back(NextCacheEntry(boundary_prev, outer_next));
          set_halfedge_handle(vh, outer_next);
          break;

        case 2: // next is new, prev is old
          boundary_next = next_halfedge_handle(inner_prev);
          next_cache.push_back(NextCacheEntry(outer_prev, boundary_next));
          set_halfedge_handle(vh, boundary_next);
          break;

        case 3: // both are new
          if (!halfedge_handle(vh).is_valid())
          {
            set_halfedge_handle(vh, outer_next);
            next_cache.push_back(NextCacheEntry(outer_prev, outer_next));
          }
          else
          {
            boundary_next = halfedge_handle(vh);
            boundary_prev = prev_halfedge_handle(boundary_next);
            next_cache.push_back(NextCacheEntry(boundary_prev, outer_next));
            next_cache.push_back(NextCacheEntry(outer_prev, boundary_next));
          }
          break;
      }

      // set inner link
      next_cache.push_back(NextCacheEntry(inner_prev, inner_next));
    }
    else needs_adjust[ii] = (halfedge_handle(vh) == inner_next);


    // set face handle
    set_face_handle(halfedge_handles[i], fh);
  }



  // process next halfedge cache
  typename NextCache::const_iterator
    ncIt(next_cache.begin()), ncEnd(next_cache.end());
  for (; ncIt != ncEnd; ++ncIt)
    set_next_halfedge_handle(ncIt->first, ncIt->second);



  // adjust vertices' halfedge handle
  for (i=0; i<n; ++i)
    if (needs_adjust[i])
      adjust_outgoing_halfedge(_vertex_handles[i]);



  return fh;
}


//-----------------------------------------------------------------------------


#if OM_OUT_OF_CLASS_TEMPLATE
template <typename Kernel>
template <typename OtherMesh>
PolyMeshT<Kernel>&
PolyMeshT<Kernel>::
assign(const OtherMesh& _rhs)
#  include "PolyMeshT_assign.hh"
#endif


//-----------------------------------------------------------------------------


template <class Kernel>
typename PolyMeshT<Kernel>::Normal
PolyMeshT<Kernel>::
calc_face_normal(FaceHandle _fh) const
{
  assert(halfedge_handle(_fh).is_valid());
  ConstFaceVertexIter fv_it(cfv_iter(_fh));

  const Point& p0(point(fv_it));  ++fv_it;
  const Point& p1(point(fv_it));  ++fv_it;
  const Point& p2(point(fv_it));

  return calc_face_normal(p0, p1, p2);
}


//-----------------------------------------------------------------------------


template <class Kernel>
typename PolyMeshT<Kernel>::Normal
PolyMeshT<Kernel>::
calc_face_normal(const Point& _p0,
		 const Point& _p1,
		 const Point& _p2) const
{
#if 1
  // The OpenSG <Vector>::operator -= () does not support the type Point
  // as rhs. Therefore use vector_cast at this point!!!
  // Note! OpenSG distinguishes between Normal and Point!!!
  Normal p1p0(_p0);  p1p0 -= vector_cast<Normal>(_p1);
  Normal p1p2(_p2);  p1p2 -= vector_cast<Normal>(_p1);

  Normal n    = cross(p1p2, p1p0);
  Scalar norm = n.length();

  // The expression ((n *= (1.0/norm)),n) is used because the OpenSG
  // vector class does not return self after component-wise
  // self-multiplication with a scalar!!!
  return (norm != Scalar(0)) ? ((n *= (Scalar(1)/norm)),n) : Normal(0,0,0);
#else
  Point p1p0 = _p0;  p1p0 -= _p1;
  Point p1p2 = _p2;  p1p2 -= _p1;

  Normal n = vector_cast<Normal>(cross(p1p2, p1p0));
  Scalar norm = n.length();

  return (norm != 0.0) ? n *= (1.0/norm) : Normal(0,0,0);
#endif
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
update_normals(VertexNormalMode _mode)
{
  if (This::has_face_normals())    update_face_normals();
  if (This::has_vertex_normals())  update_vertex_normals(_mode);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
update_face_normals()
{
  FaceIter f_it(faces_begin()), f_end(faces_end());

  for (; f_it != f_end; ++f_it)
    set_normal(f_it.handle(), calc_face_normal(f_it.handle()));
}


//-----------------------------------------------------------------------------


template <class Kernel>
typename PolyMeshT<Kernel>::Normal
PolyMeshT<Kernel>::
calc_vertex_normal(VertexHandle _vh) const
{
  Normal n;
  calc_vertex_normal_fast(_vh,n);
  return n;
}


//-----------------------------------------------------------------------------


template <class Kernel_>
void PolyMeshT<Kernel_>::
calc_vertex_normal_fast(VertexHandle _vh, Normal& _n) const
{
  _n[0]=_n[1]=_n[2]=Scalar(0.0);
  for (ConstVertexFaceIter vf_it=cvf_iter(_vh); vf_it; ++vf_it)
    _n += normal(vf_it.handle());
  Scalar norm = _n.length();
  if (norm != 0.0f) _n *= (1.0f/norm);
}


//-----------------------------------------------------------------------------


template <class Kernel_>
void PolyMeshT<Kernel_>::
calc_vertex_normal_correct(VertexHandle _vh, Normal& _n) const
{
  _n[0]=_n[1]=_n[2]=Scalar(0.0);
  ConstVertexIHalfedgeIter cvih_it = cvih_iter(_vh);
  if (!cvih_it)
  {//don't crash on isolated vertices
    return;
  }
  Normal in_he_vec;
  calc_edge_vector(cvih_it, in_he_vec);
  for ( ; cvih_it; ++cvih_it)
  {//calculates the sector normal defined by cvih_it and adds it to _n
    if (is_boundary(cvih_it))
    {
      continue;
    }
    HalfedgeHandle out_heh(next_halfedge_handle(cvih_it));
    Normal out_he_vec;
    calc_edge_vector(out_heh, out_he_vec);
    _n += cross(in_he_vec, out_he_vec);//sector area is taken into account
    in_he_vec = out_he_vec;
    in_he_vec *= -1;//change the orientation
  }
}


//-----------------------------------------------------------------------------


template <class Kernel_>
void PolyMeshT<Kernel_>::
calc_vertex_normal_loop(VertexHandle _vh, Normal& _n) const
{
  static const LoopSchemeMaskDouble& loop_scheme_mask__ =
                  LoopSchemeMaskDoubleSingleton::Instance();

  Normal t_v(0.0,0.0,0.0), t_w(0.0,0.0,0.0);
  unsigned int vh_val = valence(_vh);
  unsigned int i = 0;
  for (ConstVertexOHalfedgeIter cvoh_it = cvoh_iter(_vh); cvoh_it; ++cvoh_it, ++i)
  {
    VertexHandle r1_v(to_vertex_handle(cvoh_it));
    t_v += (Normal)(point(r1_v) * loop_scheme_mask__.tang0_weight(vh_val, i));
    t_w += (Normal)(point(r1_v) * loop_scheme_mask__.tang1_weight(vh_val, i));
  }
  _n = cross(t_w, t_v);//hack: should be cross(t_v, t_w), but then the normals are reversed?
}


//-----------------------------------------------------------------------------


template <class Kernel_>
void PolyMeshT<Kernel_>::
calc_vertex_normal_angle_weighted(VertexHandle _vh, Normal& _n) const
{
  _n[0]=_n[1]=_n[2]=Scalar(0.0);

  ConstVertexVertexIter  v2_it(cvv_iter(_vh));

  if (v2_it)
  {
    ConstVertexVertexIter  v1_it(v2_it); ++v1_it;
    const Point&           p0 = point(_vh);
    Normal                 d1, d2;
    Normal                 n;
    Scalar                 cosine, angle, norm;
    const Scalar           mone(-1.0), one(1.0);
    FaceHandle             fh;

    for (; v2_it; ++v2_it, ++v1_it)
    {
      if ((fh=face_handle(v1_it.current_halfedge_handle())).is_valid())
      {
	const Point& p1 = point(v1_it);
	const Point& p2 = point(v2_it);

	// this casts suck, but OpenSG compatibility requires it
	(d1 = (Normal)p1) -= (Normal)p0;
	(d2 = (Normal)p2) -= (Normal)p0;

	d1.normalize();
	d2.normalize();

	cosine = dot(d1,d2) / sqrt(dot(d1,d1)*dot(d2,d2)); // saves one sqrt
	if      (cosine < mone) cosine = mone; 
	else if (cosine >  one) cosine =  one;
	angle = acos(cosine);

	n = normal(fh);
	n *= angle;
	_n += n;
      }
    }

    norm = _n.length();
    if (norm != 0.0f) _n *= (1.0f/norm);
  }
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
update_vertex_normals(VertexNormalMode _mode)
{
  VertexIter  v_it(vertices_begin()), v_end(vertices_end());
  Normal      n;

  switch (_mode)
  {
    case FAST:
      for (; v_it!=v_end; ++v_it)
      {	calc_vertex_normal_fast(v_it, n); set_normal(v_it.handle(), n); }
      break;

    case CORRECT:
      for (; v_it!=v_end; ++v_it)
      {	calc_vertex_normal_correct(v_it, n); set_normal(v_it.handle(), n); }
      break;

    case LOOP:
      for (; v_it!=v_end; ++v_it)
      {	calc_vertex_normal_loop(v_it, n); set_normal(v_it.handle(), n); }
      break;

    case ANGLE_WEIGHTED:
      for (; v_it!=v_end; ++v_it)
      {	calc_vertex_normal_angle_weighted(v_it, n); set_normal(v_it.handle(), n); }
      break;
  }
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
delete_vertex(VertexHandle _vh, bool _delete_isolated_vertices)
{
  // store incident faces
  std::vector<FaceHandle> face_handles;
  face_handles.reserve(8);
  for (VFIter vf_it(vf_iter(_vh)); vf_it; ++vf_it)
    face_handles.push_back(vf_it.handle());


  // delete collected faces
  typename std::vector<FaceHandle>::iterator
    fh_it(face_handles.begin()),
    fh_end(face_handles.end());

  for (; fh_it!=fh_end; ++fh_it)
    delete_face(*fh_it, _delete_isolated_vertices);

  status(_vh).set_deleted(true);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
delete_edge(EdgeHandle _eh, bool _delete_isolated_vertices)
{
  FaceHandle fh0(face_handle(halfedge_handle(_eh, 0)));
  FaceHandle fh1(face_handle(halfedge_handle(_eh, 1)));

  if (fh0.is_valid())  delete_face(fh0, _delete_isolated_vertices);
  if (fh1.is_valid())  delete_face(fh1, _delete_isolated_vertices);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
delete_face(FaceHandle _fh, bool _delete_isolated_vertices)
{
  assert(_fh.is_valid() && !status(_fh).deleted());

  // mark face deleted
  status(_fh).set_deleted(true);


  // this vector will hold all boundary edges of face _fh
  // these edges will be deleted
  std::vector<EdgeHandle> deleted_edges;
  deleted_edges.reserve(3);


  // this vector will hold all vertices of face _fh
  // for updating their outgoing halfedge
  std::vector<VertexHandle>  vhandles;
  vhandles.reserve(3);


  // for all halfedges of face _fh do:
  //   1) invalidate face handle.
  //   2) collect all boundary halfedges, set them deleted
  //   3) store vertex handles
  HalfedgeHandle hh;
  for (FaceHalfedgeIter fh_it(fh_iter(_fh)); fh_it; ++fh_it)
  {
    hh = fh_it.handle();

    set_boundary(hh);//set_face_handle(hh, InvalidFaceHandle);

    if (is_boundary(opposite_halfedge_handle(hh)))
        deleted_edges.push_back(edge_handle(hh));

    vhandles.push_back(to_vertex_handle(hh));
  }


  // delete all collected (half)edges
  // delete isolated vertices (if _delete_isolated_vertices is true)
  if (!deleted_edges.empty())
  {
    typename std::vector<EdgeHandle>::iterator
      del_it(deleted_edges.begin()),
      del_end(deleted_edges.end());

    HalfedgeHandle h0, h1, next0, next1, prev0, prev1;
    VertexHandle   v0, v1;

    for (; del_it!=del_end; ++del_it)
    {
      h0    = halfedge_handle(*del_it, 0);
      v0    = to_vertex_handle(h0);
      next0 = next_halfedge_handle(h0);
      prev0 = prev_halfedge_handle(h0);

      h1    = halfedge_handle(*del_it, 1);
      v1    = to_vertex_handle(h1);
      next1 = next_halfedge_handle(h1);
      prev1 = prev_halfedge_handle(h1);

      // adjust next and prev handles
      set_next_halfedge_handle(prev0, next1);
      set_next_halfedge_handle(prev1, next0);

      // mark edge deleted
      status(*del_it).set_deleted(true);

      // update v0
      if (halfedge_handle(v0) == h1)
      {
        // isolated ?
        if (next0 == h1)
        {
          if (_delete_isolated_vertices)
            status(v0).set_deleted(true);
          set_isolated(v0);
        }
        else set_halfedge_handle(v0, next0);
      }

      // update v1
      if (halfedge_handle(v1) == h0)
      {
        // isolated ?
        if (next1 == h0)
        {
          if (_delete_isolated_vertices)
            status(v1).set_deleted(true);
          set_isolated(v1);
        }
        else  set_halfedge_handle(v1, next1);
      }
    }
  }


  // update outgoing halfedge handles of remaining vertices
  typename std::vector<VertexHandle>::iterator
    v_it(vhandles.begin()),
    v_end(vhandles.end());

  for (; v_it!=v_end; ++v_it)
    adjust_outgoing_halfedge(*v_it);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
triangulate(FaceHandle _fh)
{
  /*
    Split an arbitrary face into triangles by connecting
    each vertex of fh after its second to vh.

    - fh will remain valid (it will become one of the
      triangles)
    - the halfedge handles of the new triangles will
      point to the old halfedges
  */

  HalfedgeHandle base_heh(halfedge_handle(_fh));
  VertexHandle start_vh = from_vertex_handle(base_heh);
  HalfedgeHandle next_heh(next_halfedge_handle(base_heh));

  while (to_vertex_handle(next_halfedge_handle(next_heh)) != start_vh)
  {
    HalfedgeHandle next_next_heh(next_halfedge_handle(next_heh));

    FaceHandle new_fh = This::new_face();
    set_halfedge_handle(new_fh, base_heh);

    HalfedgeHandle new_heh = new_edge(to_vertex_handle(next_heh), start_vh);

    set_next_halfedge_handle(base_heh, next_heh);
    set_next_halfedge_handle(next_heh, new_heh);
    set_next_halfedge_handle(new_heh, base_heh);

    set_face_handle(base_heh, new_fh);
    set_face_handle(next_heh, new_fh);
    set_face_handle(new_heh,  new_fh);

    base_heh = opposite_halfedge_handle(new_heh);
    next_heh = next_next_heh;
  }
  set_halfedge_handle(_fh, base_heh);  //the last face takes the handle _fh

  set_next_halfedge_handle(base_heh, next_heh);
  set_next_halfedge_handle(next_halfedge_handle(next_heh), base_heh);

  set_face_handle(base_heh, _fh);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::
triangulate()
{
  /* The iterators will stay valid, even though new faces are added,
     because they are now implemented index-based instead of
     pointer-based.
  */
  FaceIter f_it(faces_begin()), f_end(faces_end());
  for (; f_it!=f_end; ++f_it)
    triangulate(f_it);
}


//-----------------------------------------------------------------------------


template <class Kernel>
void
PolyMeshT<Kernel>::split(FaceHandle fh, VertexHandle vh)
{
  /*
    Split an arbitrary face into triangles by connecting
    each vertex of fh to vh.

    - fh will remain valid (it will become one of the
      triangles)
    - the halfedge handles of the new triangles will
      point to the old halfeges
  */

  HalfedgeHandle hend = halfedge_handle(fh);
  HalfedgeHandle hh   = next_halfedge_handle(hend);

  HalfedgeHandle hold = new_edge(to_vertex_handle(hend), vh);

  set_next_halfedge_handle(hend, hold);
  set_face_handle(hold, fh);

  hold = opposite_halfedge_handle(hold);

  while (hh != hend) {

    HalfedgeHandle hnext = next_halfedge_handle(hh);

    FaceHandle fnew = This::new_face();
    set_halfedge_handle(fnew, hh);

    HalfedgeHandle hnew = new_edge(to_vertex_handle(hh), vh);

    set_next_halfedge_handle(hnew, hold);
    set_next_halfedge_handle(hold, hh);
    set_next_halfedge_handle(hh, hnew);

    set_face_handle(hnew, fnew);
    set_face_handle(hold, fnew);
    set_face_handle(hh,   fnew);

    hold = opposite_halfedge_handle(hnew);

    hh = hnext;
  }

  set_next_halfedge_handle(hold, hend);
  set_next_halfedge_handle(next_halfedge_handle(hend), hold);

  set_face_handle(hold, fh);

  set_halfedge_handle(vh, hold);
}


//-----------------------------------------------------------------------------


template <class Kernel>
unsigned int
PolyMeshT<Kernel>::
valence(VertexHandle _vh) const
{
  unsigned int count(0);

  for (ConstVertexVertexIter vv_it=cvv_iter(_vh); vv_it; ++vv_it)
    ++count;

  return count;
}


//-----------------------------------------------------------------------------


template <class Kernel>
unsigned int
PolyMeshT<Kernel>::
valence(FaceHandle _fh) const
{
  unsigned int count(0);

  for (ConstFaceVertexIter fv_it=cfv_iter(_fh); fv_it; ++fv_it)
    ++count;

  return count;
}


//=============================================================================
} // namespace OpenMesh
//=============================================================================
