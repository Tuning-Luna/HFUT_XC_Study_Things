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
//   $Date: 2005/08/11 09:40:48 $
//
//=============================================================================

/** \file DecimaterT.cc
 */


//=============================================================================
//
//  CLASS DecimaterT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_DECIMATER_DECIMATERT_CC


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Utils/Progress.hh>

#include <vector>
#if defined(OM_CC_MIPS)
#  include <float.h>
#else
#  include <cfloat>
#endif

#ifdef min
# undef min
#endif

#ifdef max
# undef max
#endif


//== NAMESPACE ===============================================================


namespace OpenMesh  {
namespace Decimater {


//== IMPLEMENTATION ==========================================================



template <class Mesh>
DecimaterT<Mesh>::
DecimaterT( Mesh& _mesh )
  : mesh_(_mesh),
    heap_(NULL),
    cmodule_(NULL),
    initialized_(false),
    emit_progress_(false)
{
  // default properties
  mesh_.request_vertex_status();
  mesh_.request_edge_status();
  mesh_.request_face_status();
  mesh_.request_face_normals();

  // private vertex properties
  mesh_.add_property( collapse_target_ );
  mesh_.add_property( priority_ );
  mesh_.add_property( heap_position_ );
}


//-----------------------------------------------------------------------------


template <class Mesh>
DecimaterT<Mesh>::
~DecimaterT()
{
  // default properties
  mesh_.release_vertex_status();
  mesh_.release_edge_status();
  mesh_.release_face_status();
  mesh_.release_face_normals();

  // private vertex properties
  mesh_.remove_property(collapse_target_);
  mesh_.remove_property(priority_);
  mesh_.remove_property(heap_position_);

  // remove modules
  typename ModuleList::iterator m_it, m_end = bmodules_.end();
  for( m_it=bmodules_.begin(); m_it!=m_end; ++m_it)
    delete *m_it;
  bmodules_.clear();
  if (cmodule_)
    delete cmodule_;
}


//-----------------------------------------------------------------------------


template <class Mesh>
void
DecimaterT<Mesh>::
info( std::ostream& _os )
{
  typename ModuleList::iterator m_it, m_end = bmodules_.end();

  _os << "binary modules: " << bmodules_.size() << std::endl;
  for( m_it=bmodules_.begin(); m_it!=m_end; ++m_it)
    _os << "  " << (*m_it)->name() << std::endl;

  _os << "priority module: "
      << (cmodule_ ? cmodule_->name().c_str() : "<None>") << std::endl;
  _os << "is initialized : " << (initialized_ ? "yes" : "no") << std::endl;
}


//-----------------------------------------------------------------------------


template <class Mesh>
bool
DecimaterT<Mesh>::
initialize()
{
  typename ModuleList::iterator m_it, m_end;


  if (!cmodule_) return false;

//   // find priority module
//   if (!cmodule_)
//   {
//     // test all modules
//     for (m_it=bmodules_.begin(), m_end=bmodules_.end(); m_it!=m_end; ++m_it)
//     {
//       if ( ! (*m_it)->is_binary() )
//       {
// 	if ( !cmodule_ ) // only one non-binary module allowed!
// 	  cmodule_ = *m_it;
// 	else
// 	{
// 	  cmodule_ = NULL;
// 	  return initialized_ = false;
// 	}
//       }
//     }

//     // no module found? look for quadric module
//     for (m_it=bmodules_.begin(), m_end=bmodules_.end(); m_it!=m_end; ++m_it)
//     {
//       if ((*m_it)->name() == "Quadric")
//       {
// 	cmodule_ = *m_it;
// 	break;
//       }
//     }

//     // need one priority module
//     if (!cmodule_) return initialized_ = false;

//     // remove prio module from list of binary modules
//     bmodules_.erase( std::find(bmodules_.begin(), bmodules_.end(), cmodule_ ) );
//   }



  // initialize modules
  for (m_it=bmodules_.begin(), m_end=bmodules_.end(); m_it!=m_end; ++m_it)
    (*m_it)->initialize();
  cmodule_->initialize();


  return initialized_ = true;
}


//-----------------------------------------------------------------------------

template <class Mesh>
bool
DecimaterT<Mesh>::
is_collapse_legal(const CollapseInfo& _ci)
{
  // locked ? deleted ?
  if (mesh_.status(_ci.v0).locked() ||
      mesh_.status(_ci.v0).deleted())
    return false;

  if (_ci.vl.is_valid() && _ci.vr.is_valid() &&
      mesh_.find_halfedge(_ci.vl, _ci.vr).is_valid() &&
      mesh_.valence(_ci.vl) == 3 && mesh_.valence(_ci.vr) == 3)
  {
    return false;
  }


  //--- feature tests ---

  if (mesh_.status(_ci.v0).feature() &&
      !mesh_.status(mesh_.edge_handle(_ci.v0v1)).feature())
    return false;

  // the other two edges removed by collapse must not be features
  if (_ci.v0vl.is_valid() &&
      mesh_.status(mesh_.edge_handle(_ci.v0vl)).feature())
    return false;
  if (_ci.vrv0.is_valid() &&
      mesh_.status(mesh_.edge_handle(_ci.vrv0)).feature())
    return false;
  


  //--- test one ring intersection ---

  typename Mesh::VertexVertexIter  vv_it;

  for (vv_it = mesh_.vv_iter(_ci.v0); vv_it; ++vv_it)
    mesh_.status(vv_it).set_tagged(false);

  for (vv_it = mesh_.vv_iter(_ci.v1); vv_it; ++vv_it)
    mesh_.status(vv_it).set_tagged(true);

  for (vv_it = mesh_.vv_iter(_ci.v0); vv_it; ++vv_it)
    if (mesh_.status(vv_it).tagged() &&
	vv_it.handle() != _ci.vl &&
	vv_it.handle() != _ci.vr)
      return false;

  // if both are invalid OR equal -> fail
  if (_ci.vl == _ci.vr) return false;


  //--- test boundary cases ---
  if (mesh_.is_boundary(_ci.v0))
  {
    if (!mesh_.is_boundary(_ci.v1))
    {// don't collapse a boundary vertex to an inner one
      return false;
    }
    else
    {// edge between two boundary vertices has to be a boundary edge
      if (!(mesh_.is_boundary(_ci.v0v1) || mesh_.is_boundary(_ci.v1v0)))
        return false;
    }
    // only one one ring intersection
    if (_ci.vl.is_valid() && _ci.vr.is_valid())
      return false;
  }

  // v0vl and v1vl must not both be boundary edges
  if (_ci.vl.is_valid() &&
      mesh_.is_boundary(_ci.vlv1) &&
      mesh_.is_boundary(_ci.v0v1))
    return false;

  // v0vr and v1vr must not be both boundary edges
  if (_ci.vr.is_valid() &&
      mesh_.is_boundary(_ci.vrv0) &&
      mesh_.is_boundary(_ci.v1vr))
    return false;

  // there have to be at least 2 incident faces at v0
  if (mesh_.cw_rotated_halfedge_handle(
				       mesh_.cw_rotated_halfedge_handle(_ci.v0v1)) == _ci.v0v1)
    return false;


  // collapse passed all tests -> ok
  return true;
}


//-----------------------------------------------------------------------------


template <class Mesh>
float
DecimaterT<Mesh>::
collapse_priority(const CollapseInfo& _ci)
{
  typename ModuleList::iterator m_it, m_end = bmodules_.end();

  for (m_it = bmodules_.begin(); m_it != m_end; ++m_it)
  {
    if ( (*m_it)->collapse_priority(_ci) < 0.0)
      return -1.0; // ILLEGAL_COLLAPSE
  }
  return cmodule_->collapse_priority(_ci);
}


//-----------------------------------------------------------------------------


template <class Mesh>
void
DecimaterT<Mesh>::
heap_vertex(VertexHandle _vh)
{
  float                           prio, best_prio(FLT_MAX);
  typename Mesh::HalfedgeHandle   heh, collapse_target;


  // find best target in one ring
  if (!mesh_.status(_vh).locked())
  {
    for (typename Mesh::VOHIter voh_it=mesh_.voh_iter(_vh); voh_it; ++voh_it)
    {
      heh = voh_it.handle();
      CollapseInfo  ci(mesh_, heh);

      if (is_collapse_legal(ci))
      {
	prio = collapse_priority(ci);
	if (prio >= 0.0 && prio < best_prio)
	{
	  best_prio       = prio;
	  collapse_target = heh;
	}
      }
    }
  }

  // target found -> put vertex on heap
  if (collapse_target.is_valid())
  {
    mesh_.property(collapse_target_, _vh) = collapse_target;
    mesh_.property(priority_, _vh)        = best_prio;

    if (heap_->is_stored(_vh))  heap_->update(_vh);
    else                        heap_->insert(_vh);
  }

  // not valid -> remove from heap
  else
  {
    if (heap_->is_stored(_vh))  heap_->remove(_vh);

    mesh_.property(collapse_target_, _vh) = collapse_target;
    mesh_.property(priority_, _vh)        = -1;
  }
}


//-----------------------------------------------------------------------------


template <class Mesh>
void
DecimaterT<Mesh>::
postprocess_collapse(CollapseInfo& _ci)
{
  typename ModuleList::iterator m_it, m_end = bmodules_.end();

  for (m_it = bmodules_.begin(); m_it != m_end; ++m_it)
    (*m_it)->postprocess_collapse(_ci);

  cmodule_->postprocess_collapse(_ci);
}


template <class Mesh>
void
DecimaterT<Mesh>::
preprocess_collapse(CollapseInfo& _ci)
{
  typename ModuleList::iterator m_it, m_end = bmodules_.end();

  for (m_it = bmodules_.begin(); m_it != m_end; ++m_it)
    (*m_it)->preprocess_collapse(_ci);

  cmodule_->preprocess_collapse(_ci);
}


//-----------------------------------------------------------------------------


template <class Mesh>
size_t
DecimaterT<Mesh>::
decimate_to( size_t _nv, size_t _nf )
{
  if ( !is_initialized() )
    return 0;
  
  if (_nv >= mesh_.n_vertices() || _nf >= mesh_.n_faces())
    return 0;
  
  
  // update progress
  if (emit_progress_)
    Progress().set_steps( std::min( (mesh_.n_vertices()-_nv),
				    (mesh_.n_faces()-_nf)/2 ),
			  "Decimating" );



  typename Mesh::VertexIter         v_it, v_end(mesh_.vertices_end());
  typename Mesh::VertexHandle       vp;
  typename Mesh::HalfedgeHandle     v0v1;
  typename Mesh::VertexFaceIter     vf_it;
  unsigned int                      nv = mesh_.n_vertices();
  unsigned int                      nf = mesh_.n_faces();
  unsigned int                      nc = 0;

  typename std::vector<typename Mesh::VertexHandle>::iterator vh_it, vh_end;




  // initialize heap
  HeapInterface  HI(mesh_, priority_, heap_position_);
  heap_ = std::auto_ptr<DeciHeap>(new DeciHeap(HI));
  heap_->reserve(mesh_.n_vertices());


  for (v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it)
  {
    heap_->reset_heap_position( v_it.handle() );
    if (!mesh_.status(v_it).deleted())
      heap_vertex( v_it.handle() );
  }


  // process heap
  while ((!heap_->empty()) && (_nv < nv) && (_nf < nf))
  {
    // get 1st heap entry
    vp   = heap_->front();
    v0v1 = mesh_.property(collapse_target_, vp);
    heap_->pop_front();


    // setup collapse info
    CollapseInfo ci(mesh_, v0v1);


    // check topological correctness AGAIN !
    if (!is_collapse_legal(ci))
      continue;


    // adjust complexity in advance (need boundary status)
    ++nc;
    --nv;
    if (mesh_.is_boundary(ci.v0v1) ||
	mesh_.is_boundary(ci.v1v0))
      --nf;
    else nf -= 2;


    // pre-processing
    preprocess_collapse(ci);


    // perform collapse
    mesh_.collapse(v0v1);


    // update progress
    if (emit_progress_)
      if (!Progress().step())
	break;


    // update triangle normals
    vf_it = mesh_.vf_iter(ci.v1);
    for (; vf_it; ++vf_it)
      if (!mesh_.status(vf_it).deleted())
	mesh_.set_normal(vf_it, mesh_.calc_face_normal(vf_it.handle()));


    // post-process collapse
    postprocess_collapse(ci);


    // update heap (former one ring of decimated vertex)
    for (vh_it = ci.one_ring.begin(), vh_end = ci.one_ring.end();
	 vh_it != vh_end; ++vh_it)
    {
      assert(!mesh_.status(*vh_it).deleted());
      heap_vertex(*vh_it);
    }
  }
			  

  // delete heap
  heap_.reset();


  // update progress
  if (emit_progress_)
    Progress().finish();


  // DON'T do garbage collection here! It's up to the application.
  return nc;
}


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================

