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
//   $Revision: 1.2 $
//   $Date: 2005/04/18 09:07:12 $
//                                                                            
//=============================================================================

/** \file RulesT.cc
    
 */

//=============================================================================
//
//  Rules - IMPLEMENTATION
//
//=============================================================================


#define OPENMESH_SUBDIVIDER_ADAPTIVE_RULEST_CC


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "RulesT.hh"
// --------------------
#if defined(OM_CC_MIPS)
#  include <math.h>
#else
#  include <cmath>
#endif

#if defined(OM_CC_MSVC)
#  pragma warning(disable:4244)
#endif

//== NAMESPACE ================================================================

namespace OpenMesh   { // BEGIN_NS_OPENMESH
namespace Subdivider { // BEGIN_NS_DECIMATER
namespace Adaptive   { // BEGIN_NS_UNIFORM


//== IMPLEMENTATION ========================================================== 

#define MOBJ Inherited::mesh_.deref
#define FH face_handle
#define VH vertex_handle
#define EH edge_handle
#define HEH halfedge_handle
#define NHEH next_halfedge_handle
#define PHEH prev_halfedge_handle
#define OHEH opposite_halfedge_handle
#define TVH  to_vertex_handle
#define FVH  from_vertex_handle

// ------------------------------------------------------------------ Tvv3 ----


template<class M>
void
Tvv3<M>::raise(typename M::FaceHandle& _fh, state_t _target_state) 
{
  if (MOBJ(_fh).state() < _target_state) 
  {
    update(_fh, _target_state);

    typename M::VertexVertexIter          vv_it;
    typename M::FaceVertexIter            fv_it;
    typename M::VertexHandle              vh;
    typename M::Point                     position(0.0, 0.0, 0.0);
    typename M::Point                     face_position;
    const typename M::Point               zero_point(0.0, 0.0, 0.0);
    std::vector<typename M::VertexHandle> vertex_vector;
    int                                      valence(0);

    // raise all adjacent vertices to level x-1
    for (fv_it = Inherited::mesh_.fv_iter(_fh); fv_it; ++fv_it) {

      vertex_vector.push_back(fv_it.handle());
    }

    while(!vertex_vector.empty()) {

      vh = vertex_vector.back();
      vertex_vector.pop_back();

      if (_target_state > 1)
	Inherited::prev_rule()->raise(vh, _target_state - 1);
    }

    face_position = MOBJ(_fh).position(_target_state - 1);
    
    typename M::EdgeHandle               eh;
    std::vector<typename M::EdgeHandle>  edge_vector;

    // interior face
    if (!Inherited::mesh_.is_boundary(_fh) || MOBJ(_fh).final()) { 

      // insert new vertex
      vh = Inherited::mesh_.new_vertex();

      Inherited::mesh_.split(_fh, vh);

      // calculate display position for new vertex
      for (vv_it = Inherited::mesh_.vv_iter(vh); vv_it; ++vv_it) 
      {
        position += Inherited::mesh_.point(vv_it.handle());
        ++valence;
      }

      position /= valence;

      // set attributes for new vertex
      Inherited::mesh_.set_point(vh, position);
      MOBJ(vh).set_position(_target_state, zero_point);
      MOBJ(vh).set_state(_target_state);
      MOBJ(vh).set_not_final();

      typename M::VertexOHalfedgeIter      voh_it;
      // check for edge flipping
      for (voh_it = Inherited::mesh_.voh_iter(vh); voh_it; ++voh_it) {
      
	if (Inherited::mesh_.FH(voh_it.handle()).is_valid()) {

	  MOBJ(Inherited::mesh_.FH(voh_it.handle())).set_state(_target_state);
	  MOBJ(Inherited::mesh_.FH(voh_it.handle())).set_not_final();
	  MOBJ(Inherited::mesh_.FH(voh_it.handle())).set_position(_target_state - 1, face_position);
      

	  for (state_t j = 0; j < _target_state; ++j) {
	    MOBJ(Inherited::mesh_.FH(voh_it.handle())).set_position(j, MOBJ(_fh).position(j));
	  }
      
	  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))).is_valid()) {

	    if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle())))).state() == _target_state) {

	      if (Inherited::mesh_.is_flip_ok(Inherited::mesh_.EH(Inherited::mesh_.NHEH(voh_it.handle())))) {

		edge_vector.push_back(Inherited::mesh_.EH(Inherited::mesh_.NHEH(voh_it.handle())));
	      }
	    }
	  }
	}
      }
    }
    
    // boundary face
    else { 

      typename M::VertexHandle vh1 = Inherited::mesh_.new_vertex(),
	                          vh2 = Inherited::mesh_.new_vertex();
      
      typename M::HalfedgeHandle hh2 = Inherited::mesh_.HEH(_fh),
                                    hh1, hh3;
      
      while (!Inherited::mesh_.is_boundary(Inherited::mesh_.OHEH(hh2)))
	hh2 = Inherited::mesh_.NHEH(hh2);
      
      eh = Inherited::mesh_.EH(hh2);
      
      hh2 = Inherited::mesh_.NHEH(hh2);
      hh1 = Inherited::mesh_.NHEH(hh2);
      
      assert(Inherited::mesh_.is_boundary(eh));

      Inherited::mesh_.split(eh, vh1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh2));

      assert(Inherited::mesh_.is_boundary(eh));

      Inherited::mesh_.split(eh, vh2);

      hh3 = Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(hh1)));

      typename M::VertexHandle   vh0(Inherited::mesh_.TVH(hh1)),
	                            vh3(Inherited::mesh_.FVH(hh2));

      // set display position and attributes for new vertices
      Inherited::mesh_.set_point(vh1, (Inherited::mesh_.point(vh0) * 2.0 + Inherited::mesh_.point(vh3)) / 3.0);

      MOBJ(vh1).set_position(_target_state, zero_point);
      MOBJ(vh1).set_state(_target_state);
      MOBJ(vh1).set_not_final();

      MOBJ(vh0).set_position(_target_state, MOBJ(vh0).position(_target_state - 1) * 3.0);
      MOBJ(vh0).set_state(_target_state);
      MOBJ(vh0).set_not_final();

      // set display position and attributes for old vertices
      Inherited::mesh_.set_point(vh2, (Inherited::mesh_.point(vh3) * 2.0 + Inherited::mesh_.point(vh0)) / 3.0);
      MOBJ(vh2).set_position(_target_state, zero_point);
      MOBJ(vh2).set_state(_target_state);
      MOBJ(vh2).set_not_final();

      MOBJ(vh3).set_position(_target_state, MOBJ(vh3).position(_target_state - 1) * 3.0);
      MOBJ(vh3).set_state(_target_state);
      MOBJ(vh3).set_not_final();

      // init 3 faces
      MOBJ(Inherited::mesh_.FH(hh1)).set_state(_target_state);
      MOBJ(Inherited::mesh_.FH(hh1)).set_not_final();
      MOBJ(Inherited::mesh_.FH(hh1)).set_position(_target_state - 1, face_position);
      
      MOBJ(Inherited::mesh_.FH(hh2)).set_state(_target_state);
      MOBJ(Inherited::mesh_.FH(hh2)).set_not_final();
      MOBJ(Inherited::mesh_.FH(hh2)).set_position(_target_state - 1, face_position);

      MOBJ(Inherited::mesh_.FH(hh3)).set_state(_target_state);
      MOBJ(Inherited::mesh_.FH(hh3)).set_final();
      MOBJ(Inherited::mesh_.FH(hh3)).set_position(_target_state - 1, face_position);
      

      for (state_t j = 0; j < _target_state; ++j) {
	MOBJ(Inherited::mesh_.FH(hh1)).set_position(j, MOBJ(_fh).position(j));
      }
      
      for (state_t j = 0; j < _target_state; ++j) {

	MOBJ(Inherited::mesh_.FH(hh2)).set_position(j, MOBJ(_fh).position(j));
      }
      
      for (state_t j = 0; j < _target_state; ++j) {

	MOBJ(Inherited::mesh_.FH(hh3)).set_position(j, MOBJ(_fh).position(j));
      }
      
      // check for edge flipping
      if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh1)).is_valid()) {

	if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh1))).state() == _target_state) {

	  if (Inherited::mesh_.is_flip_ok(Inherited::mesh_.EH(hh1))) {

	    edge_vector.push_back(Inherited::mesh_.EH(hh1));
	  }
	}
      }

      if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh2)).is_valid()) {

	if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh2))).state() == _target_state) {

	  if (Inherited::mesh_.is_flip_ok(Inherited::mesh_.EH(hh2))) {

	    edge_vector.push_back(Inherited::mesh_.EH(hh2));
	  }
	}
      }
    }
  
    // flip edges
    while (!edge_vector.empty()) {

      eh = edge_vector.back();
      edge_vector.pop_back();
      
      assert(Inherited::mesh_.is_flip_ok(eh));

      Inherited::mesh_.flip(eh);

      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(eh, 0))).set_final();
      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(eh, 1))).set_final();

      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(eh, 0))).set_state(_target_state);
      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(eh, 1))).set_state(_target_state);

      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(eh, 0))).set_position(_target_state, face_position);
      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(eh, 1))).set_position(_target_state, face_position);
    }
  }
}


template<class M>
void Tvv3<M>::raise(typename M::VertexHandle& _vh, state_t _target_state) {

  if (MOBJ(_vh).state() < _target_state) {

    update(_vh, _target_state);

    // multiply old position by 3
    MOBJ(_vh).set_position(_target_state, MOBJ(_vh).position(_target_state - 1) * 3.0);

    MOBJ(_vh).inc_state();

    assert(MOBJ(_vh).state() == _target_state);
  }
}


// ------------------------------------------------------------------ Tvv4 ----


template<class M>
void
Tvv4<M>::raise(typename M::FaceHandle& _fh, state_t _target_state) 
{

  if (MOBJ(_fh).state() < _target_state) {

    update(_fh, _target_state);

    typename M::FaceVertexIter              fv_it;
    typename M::VertexHandle                temp_vh;
    typename M::Point                       face_position;
    const typename M::Point                 zero_point(0.0, 0.0, 0.0);
    std::vector<typename M::VertexHandle>   vertex_vector;
    std::vector<typename M::HalfedgeHandle> halfedge_vector;

    // raise all adjacent vertices to level x-1
    for (fv_it = Inherited::mesh_.fv_iter(_fh); fv_it; ++fv_it) {

      vertex_vector.push_back(fv_it.handle());
    }

    while(!vertex_vector.empty()) {

      temp_vh = vertex_vector.back();
      vertex_vector.pop_back();

      if (_target_state > 1) {

	Inherited::prev_rule()->raise(temp_vh, _target_state - 1);
      }
    }

    face_position = MOBJ(_fh).position(_target_state - 1);

    typename M::HalfedgeHandle hh[3];
    typename M::VertexHandle   vh[3];
    typename M::VertexHandle   new_vh[3];
    typename M::FaceHandle     fh[4];
    typename M::EdgeHandle     eh;
    typename M::HalfedgeHandle temp_hh;

    // normal (final) face
    if (MOBJ(_fh).final()) {

      // define three halfedge handles around the face
      hh[0] = Inherited::mesh_.HEH(_fh);
      hh[1] = Inherited::mesh_.NHEH(hh[0]);
      hh[2] = Inherited::mesh_.NHEH(hh[1]);

      assert(hh[0] == Inherited::mesh_.NHEH(hh[2]));

      vh[0] = Inherited::mesh_.TVH(hh[0]);
      vh[1] = Inherited::mesh_.TVH(hh[1]);
      vh[2] = Inherited::mesh_.TVH(hh[2]);
      
      new_vh[0] = Inherited::mesh_.add_vertex(zero_point);
      new_vh[1] = Inherited::mesh_.add_vertex(zero_point);
      new_vh[2] = Inherited::mesh_.add_vertex(zero_point);

      // split three edges
      split_edge(hh[0], new_vh[0], _target_state);
      eh = Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh[2]));
      split_edge(hh[1], new_vh[1], _target_state);
      split_edge(hh[2], new_vh[2], _target_state);

      assert(Inherited::mesh_.FVH(hh[2]) == vh[1]);
      assert(Inherited::mesh_.FVH(hh[1]) == vh[0]);
      assert(Inherited::mesh_.FVH(hh[0]) == vh[2]);

      if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[0])).is_valid()) {

	temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[0])));
	if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
	  halfedge_vector.push_back(temp_hh);
      }

      if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[1])).is_valid()) {

	temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[1])));
	if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
	  halfedge_vector.push_back(temp_hh);
      }

      if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[2])).is_valid()) {

	temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[2])));
	if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
	  halfedge_vector.push_back(temp_hh);
      }
    }
    
    // splitted face, check for type
    else {

      // define red halfedge handle
      typename M::HalfedgeHandle red_hh(MOBJ(_fh).red_halfedge());

      if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh))).is_valid() 
	  && Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)))).is_valid() 
	  && MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh)))).red_halfedge() == red_hh 
	  && MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh))))).red_halfedge() == red_hh) {

	// three times divided face
	vh[0] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)))));
	vh[1] = Inherited::mesh_.TVH(red_hh);
	vh[2] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh))));
      
	new_vh[0] = Inherited::mesh_.FVH(red_hh);
	new_vh[1] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)));
	new_vh[2] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(red_hh));

	hh[0] = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh)));
	hh[1] = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh))));
	hh[2] = Inherited::mesh_.NHEH(red_hh);

	eh = Inherited::mesh_.EH(red_hh);
      }
      
      else {

	if ((Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh))).is_valid() && 
             MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh)))).red_halfedge() 
             == red_hh )
	    || (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)))).is_valid() 
	    && MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh))))).red_halfedge() == red_hh))
        {

	  // double divided face
	  if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh)))).red_halfedge() == red_hh) {

	    // first case
	    vh[0] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)));
	    vh[1] = Inherited::mesh_.TVH(red_hh);
	    vh[2] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh))));
      
	    new_vh[0] = Inherited::mesh_.FVH(red_hh);
	    new_vh[1] = Inherited::mesh_.add_vertex(zero_point);
	    new_vh[2] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(red_hh));

	    hh[0] = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(red_hh)));
	    hh[1] = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(red_hh));
	    hh[2] = Inherited::mesh_.NHEH(red_hh);

	    // split one edge
	    eh = Inherited::mesh_.EH(red_hh);

	    split_edge(hh[1], new_vh[1], _target_state);

	    assert(Inherited::mesh_.FVH(hh[2]) == vh[1]);
	    assert(Inherited::mesh_.FVH(hh[1]) == vh[0]);
	    assert(Inherited::mesh_.FVH(hh[0]) == vh[2]);
	    
	    if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[1])).is_valid()) {

	      temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[1])));
	      if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
		halfedge_vector.push_back(temp_hh);
	    }
	  }
	
	
	  else {

	    // second case
	    vh[0] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)))));
	    vh[1] = Inherited::mesh_.TVH(red_hh);
	    vh[2] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(red_hh));
      
	    new_vh[0] = Inherited::mesh_.FVH(red_hh);
	    new_vh[1] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)));
	    new_vh[2] = Inherited::mesh_.add_vertex(zero_point);

	    hh[0] = Inherited::mesh_.PHEH(red_hh);
	    hh[1] = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh))));
	    hh[2] = Inherited::mesh_.NHEH(red_hh);

	    // split one edge
	    eh = Inherited::mesh_.EH(red_hh);

	    split_edge(hh[2], new_vh[2], _target_state);

	    assert(Inherited::mesh_.FVH(hh[2]) == vh[1]);
	    assert(Inherited::mesh_.FVH(hh[1]) == vh[0]);
	    assert(Inherited::mesh_.FVH(hh[0]) == vh[2]);

	    if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[2])).is_valid()) {
	    
	      temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[2])));
	      if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
		halfedge_vector.push_back(temp_hh);
	    }
	  }
	}

	else {
	  
	  // one time divided face
	  vh[0] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(red_hh)));
	  vh[1] = Inherited::mesh_.TVH(red_hh);
	  vh[2] = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(red_hh));
      
	  new_vh[0] = Inherited::mesh_.FVH(red_hh);
	  new_vh[1] = Inherited::mesh_.add_vertex(zero_point);
	  new_vh[2] = Inherited::mesh_.add_vertex(zero_point);

	  hh[0] = Inherited::mesh_.PHEH(red_hh);
	  hh[1] = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(red_hh));
	  hh[2] = Inherited::mesh_.NHEH(red_hh);

	  // split two edges
	  eh = Inherited::mesh_.EH(red_hh);

	  split_edge(hh[1], new_vh[1], _target_state);
	  split_edge(hh[2], new_vh[2], _target_state);

	  assert(Inherited::mesh_.FVH(hh[2]) == vh[1]);
	  assert(Inherited::mesh_.FVH(hh[1]) == vh[0]);
	  assert(Inherited::mesh_.FVH(hh[0]) == vh[2]);

	  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[1])).is_valid()) {

	    temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[1])));
	    if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
	      halfedge_vector.push_back(temp_hh);
	  }

	  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(hh[2])).is_valid()) {

	    temp_hh = Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(hh[2])));
	    if (MOBJ(Inherited::mesh_.FH(temp_hh)).red_halfedge() != temp_hh)
	      halfedge_vector.push_back(temp_hh);
	  }
      	}
      }
    }

    // continue here for all cases
	
    // flip edge
    if (Inherited::mesh_.is_flip_ok(eh)) {
      
      Inherited::mesh_.flip(eh);
    }
    
    // search new faces
    fh[0] = Inherited::mesh_.FH(hh[0]);
    fh[1] = Inherited::mesh_.FH(hh[1]);
    fh[2] = Inherited::mesh_.FH(hh[2]);
    fh[3] = Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(hh[0])));

    assert(_fh == fh[0] || _fh == fh[1] || _fh == fh[2] || _fh == fh[3]); 

    // init new faces
    for (int i = 0; i <= 3; ++i) {

      MOBJ(fh[i]).set_state(_target_state);
      MOBJ(fh[i]).set_final();
      MOBJ(fh[i]).set_position(_target_state, face_position);
      MOBJ(fh[i]).set_red_halfedge(Inherited::Mesh::InvalidHalfedgeHandle);
    }

    // init new vertices and edges
    for (int i = 0; i <= 2; ++i) {
	
      MOBJ(new_vh[i]).set_position(_target_state, zero_point);
      MOBJ(new_vh[i]).set_state(_target_state);
      MOBJ(new_vh[i]).set_not_final();

      Inherited::mesh_.set_point(new_vh[i], (Inherited::mesh_.point(vh[i]) + Inherited::mesh_.point(vh[(i + 2) % 3])) * 0.5);

      MOBJ(Inherited::mesh_.EH(hh[i])).set_state(_target_state);
      MOBJ(Inherited::mesh_.EH(hh[i])).set_position(_target_state, zero_point);
      MOBJ(Inherited::mesh_.EH(hh[i])).set_final();

      MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh[i]))).set_state(_target_state);
      MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh[i]))).set_position(_target_state, zero_point);
      MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh[i]))).set_final();

      MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh[i]))).set_state(_target_state);
      MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh[i]))).set_position(_target_state, zero_point);
      MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh[i]))).set_final();
    }

    // check, if opposite triangle needs splitting
    while (!halfedge_vector.empty()) {

      temp_hh = halfedge_vector.back();
      halfedge_vector.pop_back();

      check_edge(temp_hh, _target_state);
    }

    assert(MOBJ(fh[0]).state() == _target_state);
    assert(MOBJ(fh[1]).state() == _target_state);
    assert(MOBJ(fh[2]).state() == _target_state);
    assert(MOBJ(fh[3]).state() == _target_state);
  }
}


template<class M>
void
Tvv4<M>::raise(typename M::VertexHandle& _vh, state_t _target_state)
{
 
  if (MOBJ(_vh).state() < _target_state)
  {

    update(_vh, _target_state);

    // multiply old position by 4
    MOBJ(_vh).set_position(_target_state, MOBJ(_vh).position(_target_state - 1) * 4.0);

    MOBJ(_vh).inc_state();
  }
}


template<class M>
void
Tvv4<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) 
{ 
  if (MOBJ(_eh).state() < _target_state) 
  {
    update(_eh, _target_state);

    typename M::FaceHandle fh(Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 0)));

    if (!fh.is_valid())
      fh=Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 1));

    raise(fh, _target_state);

    assert(MOBJ(_eh).state() == _target_state);
  }
}

#ifndef DOXY_IGNORE_THIS

template<class M>
void
Tvv4<M>::split_edge(typename M::HalfedgeHandle &_hh, 
                       typename M::VertexHandle   &_vh, 
                       state_t _target_state) 
{
  typename M::HalfedgeHandle temp_hh;

  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh)).is_valid()) 
  {
    if (!MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).final()) 
    {    
      if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).red_halfedge().is_valid()) 
      {        
	temp_hh = MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).red_halfedge();
      }
      else 
      {
	// two cases for divided, but not visited face
	if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(_hh))))).state() 
            == MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).state()) 
        {
	  temp_hh = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(_hh));
	}

	else if (MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(_hh))))).state() 
                 == MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).state())
        {
	  temp_hh = Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(_hh)); 
	}
      }
    }
    else
      temp_hh = Inherited::Mesh::InvalidHalfedgeHandle;
  }
  else
    temp_hh = Inherited::Mesh::InvalidHalfedgeHandle;
	 
  // split edge
  Inherited::mesh_.split(Inherited::mesh_.EH(_hh), _vh);
	  
  if (Inherited::mesh_.FVH(_hh) == _vh) 
  {	    
    MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(_hh))))).set_state(MOBJ(Inherited::mesh_.EH(_hh)).state());
    _hh = Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(_hh)));
  }

  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh)).is_valid()) {
	  
    MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(_hh)))).set_not_final();
    MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).set_state(_target_state-1);
    MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(_hh)))))).set_state(_target_state-1);

    MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).set_not_final();
    MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(_hh)))))).set_not_final();

    MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(_hh)))).set_state(_target_state);

    if (temp_hh.is_valid()) {
	
      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))).set_red_halfedge(temp_hh);
      MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(_hh)))))).set_red_halfedge(temp_hh);
    } 
    else {

      typename M::FaceHandle 
        fh1(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh))),
        fh2(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(_hh))))));

      MOBJ(fh1).set_red_halfedge(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(_hh))));
      MOBJ(fh2).set_red_halfedge(Inherited::mesh_.OHEH(Inherited::mesh_.PHEH(Inherited::mesh_.OHEH(_hh))));

      const typename M::Point zero_point(0.0, 0.0, 0.0);

      MOBJ(fh1).set_position(_target_state - 1, zero_point);
      MOBJ(fh2).set_position(_target_state - 1, zero_point);
    }
  }

  // init edges
  MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(_hh))))).set_state(_target_state - 1);
  MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(_hh))))).set_final();

  MOBJ(Inherited::mesh_.EH(_hh)).set_state(_target_state - 1);
  MOBJ(Inherited::mesh_.EH(_hh)).set_final();
}  


template<class M>
void Tvv4<M>::check_edge(const typename M::HalfedgeHandle& _hh, 
                         state_t _target_state) 
{
  typename M::FaceHandle fh1(Inherited::mesh_.FH(_hh)),
    fh2(Inherited::mesh_.FH(Inherited::mesh_.OHEH(_hh)));

  assert(fh1.is_valid());
  assert(fh2.is_valid());

  typename M::HalfedgeHandle red_hh(MOBJ(fh1).red_halfedge());
  
  if (!MOBJ(fh1).final()) {

    assert (MOBJ(fh1).final() == MOBJ(fh2).final());
    assert (!MOBJ(fh1).final());
    assert (MOBJ(fh1).red_halfedge() == MOBJ(fh2).red_halfedge());

    const typename M::Point zero_point(0.0, 0.0, 0.0);

    MOBJ(fh1).set_position(_target_state - 1, zero_point);
    MOBJ(fh2).set_position(_target_state - 1, zero_point);

    assert(red_hh.is_valid());

    if (!red_hh.is_valid()) {

      MOBJ(fh1).set_state(_target_state - 1);
      MOBJ(fh2).set_state(_target_state - 1);

      MOBJ(fh1).set_red_halfedge(_hh);
      MOBJ(fh2).set_red_halfedge(_hh);

      MOBJ(Inherited::mesh_.EH(_hh)).set_not_final();
      MOBJ(Inherited::mesh_.EH(_hh)).set_state(_target_state - 1);
    }

    else {

      MOBJ(Inherited::mesh_.EH(_hh)).set_not_final();
      MOBJ(Inherited::mesh_.EH(_hh)).set_state(_target_state - 1);

      raise(fh1, _target_state);

      assert(MOBJ(fh1).state() == _target_state);
    }
  }
}


// -------------------------------------------------------------------- VF ----


template<class M>
void VF<M>::raise(typename M::FaceHandle& _fh, state_t _target_state) 
{
  if (MOBJ(_fh).state() < _target_state) {

    update(_fh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::FaceVertexIter            fv_it;
    typename M::VertexHandle              vh;
    std::vector<typename M::VertexHandle> vertex_vector;

    if (_target_state > 1) {

      for (fv_it = Inherited::mesh_.fv_iter(_fh); fv_it; ++fv_it) {

	vertex_vector.push_back(fv_it.handle());
      }

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (fv_it = Inherited::mesh_.fv_iter(_fh); fv_it; ++fv_it) {

      ++valence;
      position += fv_it->position(_target_state - 1);
    }

    position /= valence;

    // boundary rule
    if (Inherited::number() == Inherited::subdiv_rule()->Inherited::number() + 1 && 
        Inherited::mesh_.is_boundary(_fh)                  && 
        !MOBJ(_fh).final())
      position *= 0.5;

    MOBJ(_fh).set_position(_target_state, position);
    MOBJ(_fh).inc_state();

    assert(_target_state == MOBJ(_fh).state());
  }
}


// -------------------------------------------------------------------- FF ----


template<class M>
void FF<M>::raise(typename M::FaceHandle& _fh, state_t _target_state) {

  if (MOBJ(_fh).state() < _target_state) {

    update(_fh, _target_state);

    // raise all neighbour faces to level x-1
    typename M::FaceFaceIter              ff_it;
    typename M::FaceHandle                fh;
    std::vector<typename M::FaceHandle>   face_vector;

    if (_target_state > 1) {

      for (ff_it = Inherited::mesh_.ff_iter(_fh); ff_it; ++ff_it) {

	face_vector.push_back(ff_it.handle());
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	Inherited::prev_rule()->raise(fh, _target_state - 1);
      }

      for (ff_it = Inherited::mesh_.ff_iter(_fh); ff_it; ++ff_it) {

	face_vector.push_back(ff_it.handle());
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	while (MOBJ(fh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(fh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (ff_it = Inherited::mesh_.ff_iter(_fh); ff_it; ++ff_it) {

      ++valence;

      position += ff_it->position(_target_state - 1);
    }

    position /= valence;

    MOBJ(_fh).set_position(_target_state, position);
    MOBJ(_fh).inc_state();
  }
}


// ------------------------------------------------------------------- FFc ----


template<class M>
void FFc<M>::raise(typename M::FaceHandle& _fh, state_t _target_state) 
{
  if (MOBJ(_fh).state() < _target_state) {

    update(_fh, _target_state);

    // raise all neighbour faces to level x-1
    typename M::FaceFaceIter              ff_it(Inherited::mesh_.ff_iter(_fh));
    typename M::FaceHandle                fh;
    std::vector<typename M::FaceHandle>   face_vector;

    if (_target_state > 1) 
    {
      for (; ff_it; ++ff_it) 
	face_vector.push_back(ff_it.handle());

      while (!face_vector.empty()) 
      {
	fh = face_vector.back();
	face_vector.pop_back();
	Inherited::prev_rule()->raise(fh, _target_state - 1);
      }

      for (ff_it = Inherited::mesh_.ff_iter(_fh); ff_it; ++ff_it) 
	face_vector.push_back(ff_it.handle());

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	while (MOBJ(fh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(fh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (ff_it = Inherited::mesh_.ff_iter(_fh); ff_it; ++ff_it) 
    {
      ++valence;
      position += ff_it->position(_target_state - 1);
    }

    position /= valence;

    // choose coefficient c
    typename M::Scalar c = Inherited::coeff();

    position *= (1.0 - c);
    position += MOBJ(_fh).position(_target_state - 1) * c;

    MOBJ(_fh).set_position(_target_state, position);
    MOBJ(_fh).inc_state();
  }
}


// -------------------------------------------------------------------- FV ----


template<class M>
void FV<M>::raise(typename M::VertexHandle& _vh, state_t _target_state) 
{

  if (MOBJ(_vh).state() < _target_state) {

    update(_vh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexFaceIter            vf_it(Inherited::mesh_.vf_iter(_vh));
    typename M::FaceHandle                fh;
    std::vector<typename M::FaceHandle>   face_vector;

    if (_target_state > 1) {

      for (; vf_it; ++vf_it) {

	face_vector.push_back(vf_it.handle());
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	Inherited::prev_rule()->raise(fh, _target_state - 1);
      }

      for (vf_it = Inherited::mesh_.vf_iter(_vh); vf_it; ++vf_it) {

	face_vector.push_back(vf_it.handle());
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	while (MOBJ(fh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(fh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (vf_it = Inherited::mesh_.vf_iter(_vh); vf_it; ++vf_it) {

      ++valence;
      position += vf_it->position(_target_state - 1);
    }

    position /= valence;

    MOBJ(_vh).set_position(_target_state, position);
    MOBJ(_vh).inc_state();

    if (Inherited::number() == Inherited::n_rules() - 1) {

      Inherited::mesh_.set_point(_vh, position);
      MOBJ(_vh).set_final();
    }
  }
}


// ------------------------------------------------------------------- FVc ----


template<class M>
void FVc<M>::raise(typename M::VertexHandle& _vh, state_t _target_state) 
{
  if (MOBJ(_vh).state() < _target_state) {

    update(_vh, _target_state);

    typename M::VertexOHalfedgeIter       voh_it;
    typename M::FaceHandle                fh;
    std::vector<typename M::FaceHandle>   face_vector;
    int                                      valence(0);

    face_vector.clear();

    // raise all neighbour faces to level x-1
    if (_target_state > 1) {

      for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) {

	if (Inherited::mesh_.FH(voh_it.handle()).is_valid()) {

	  face_vector.push_back(Inherited::mesh_.FH(voh_it.handle()));

	  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))).is_valid()) {

	    face_vector.push_back(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))));
	  }
	}
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	Inherited::prev_rule()->raise(fh, _target_state - 1);
      }

      for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) {

	if (Inherited::mesh_.FH(voh_it.handle()).is_valid()) {

	  face_vector.push_back(Inherited::mesh_.FH(voh_it.handle()));

	  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))).is_valid()) {

	    face_vector.push_back(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))));
	  }
	}
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	while (MOBJ(fh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(fh, _target_state - 1);
      }

      for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) {

	if (Inherited::mesh_.FH(voh_it.handle()).is_valid()) {

	  face_vector.push_back(Inherited::mesh_.FH(voh_it.handle()));

	  if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))).is_valid()) {

	    face_vector.push_back(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))));
	  }
	}
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	while (MOBJ(fh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(fh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point               position(0.0, 0.0, 0.0);
    typename M::Scalar              c;
#if 0
    const typename M::Scalar        _2pi(2.0*M_PI);
    const typename M::Scalar        _2over3(2.0/3.0);

    for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) 
    {     
      ++valence;
    }

    // choose coefficient c
    c = _2over3 * ( cos( _2pi / valence) + 1.0);
#else
    valence = Inherited::mesh_.valence(_vh);
    c       = coeff(valence);
#endif


    for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) {

      fh = Inherited::mesh_.FH(voh_it.handle());
      if (fh.is_valid())
	Inherited::prev_rule()->raise(fh, _target_state - 1);

      fh = Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle())));
      if (fh.is_valid())
	Inherited::prev_rule()->raise(fh, _target_state - 1);

      if (Inherited::mesh_.FH(voh_it.handle()).is_valid()) {

	if (Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle()))).is_valid()) {

	  position += MOBJ(Inherited::mesh_.FH(voh_it.handle())).position(_target_state - 1) * c;

	  position += MOBJ(Inherited::mesh_.FH(Inherited::mesh_.OHEH(Inherited::mesh_.NHEH(voh_it.handle())))).position(_target_state - 1) * (1.0 - c);
	}
	else {

	  position += MOBJ(Inherited::mesh_.FH(voh_it.handle())).position(_target_state - 1);
	}
      }

      else {

	--valence;
      }
    } 
    
    position /= valence;

    MOBJ(_vh).set_position(_target_state, position);
    MOBJ(_vh).inc_state();

    assert(MOBJ(_vh).state() == _target_state);
    
    // check if last rule
    if (Inherited::number() == Inherited::n_rules() - 1) {

      Inherited::mesh_.set_point(_vh, position);
      MOBJ(_vh).set_final();
    }
  }
}

template<class M>
std::vector<double> FVc<M>::coeffs_;

template <class M>
void FVc<M>::init_coeffs(size_t _max_valence)
{
  if ( coeffs_.size() == _max_valence+1 )
    return;

  if ( coeffs_.size() < _max_valence+1 )
  {
    const double _2pi(2.0*M_PI);
    const double _2over3(2.0/3.0);

    if (coeffs_.empty())
      coeffs_.push_back(0.0); // dummy for valence 0
      
    for(size_t v=coeffs_.size(); v <= _max_valence; ++v)
      coeffs_.push_back(_2over3 * ( cos( _2pi / v) + 1.0));    
  }
}


// -------------------------------------------------------------------- VV ----


template<class M>
void VV<M>::raise(typename M::VertexHandle& _vh, state_t _target_state) 
{
  if (MOBJ(_vh).state() < _target_state) 
  {
    update(_vh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexVertexIter              vv_it(Inherited::mesh_.vv_iter(_vh));
    typename M::VertexHandle                  vh;
    std::vector<typename M::VertexHandle>     vertex_vector;

    if (_target_state > 1) {

      for (; vv_it; ++vv_it) {

	vertex_vector.push_back(vv_it.handle());
      }

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }

      for (; vv_it; ++vv_it) {

	vertex_vector.push_back(vv_it.handle());
      }

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (vv_it = Inherited::mesh_.vv_iter(_vh); vv_it; ++vv_it) {

      ++valence;

      position += vv_it->position(_target_state - 1);
    }

    position /= valence;

    MOBJ(_vh).set_position(_target_state, position);
    MOBJ(_vh).inc_state();

    // check if last rule
    if (Inherited::number() == Inherited::n_rules() - 1) {

      Inherited::mesh_.set_point(_vh, position);
      MOBJ(_vh).set_final();
    }
  }
}


// ------------------------------------------------------------------- VVc ----


template<class M>
void VVc<M>::raise(typename M::VertexHandle& _vh, state_t _target_state)
{
  if (MOBJ(_vh).state() < _target_state) {

    update(_vh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexVertexIter              vv_it(Inherited::mesh_.vv_iter(_vh));
    typename M::VertexHandle                  vh;
    std::vector<typename M::VertexHandle>     vertex_vector;

    if (_target_state > 1) {

      for (; vv_it; ++vv_it) {

	vertex_vector.push_back(vv_it.handle());
      }

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }

      for (; vv_it; ++vv_it) {

	vertex_vector.push_back(vv_it.handle());
      }

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);
    typename M::Scalar c;

    for (vv_it = Inherited::mesh_.vv_iter(_vh); vv_it; ++vv_it) 
    {
      ++valence;
      position += vv_it->position(_target_state - 1);
    }

    position /= valence;

    // choose coefficcient c
    c = Inherited::coeff();

    position *= (1.0 - c);
    position += MOBJ(_vh).position(_target_state - 1) * c;

    MOBJ(_vh).set_position(_target_state, position);
    MOBJ(_vh).inc_state();

    if (Inherited::number() == Inherited::n_rules() - 1) 
    {
      Inherited::mesh_.set_point(_vh, position);
      MOBJ(_vh).set_final();
    }
  }
}


// -------------------------------------------------------------------- VE ----


template<class M>
void VE<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) 
{
  if (MOBJ(_eh).state() < _target_state) {

    update(_eh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexHandle          vh;
    typename M::HalfedgeHandle        hh1(Inherited::mesh_.HEH(_eh, 0)),
                                         hh2(Inherited::mesh_.HEH(_eh, 1));

    if (_target_state > 1) {

      vh = Inherited::mesh_.TVH(hh1);
      
      Inherited::prev_rule()->raise(vh, _target_state - 1);

      vh = Inherited::mesh_.TVH(hh2);
      
      Inherited::prev_rule()->raise(vh, _target_state - 1);
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    valence = 2;
    position += MOBJ(Inherited::mesh_.TVH(hh1)).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.TVH(hh2)).position(_target_state - 1);

    position /= valence;

    MOBJ(_eh).set_position(_target_state, position);
    MOBJ(_eh).inc_state();
  }
}


// ------------------------------------------------------------------- VdE ----


template<class M>
void VdE<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) 
{
  if (MOBJ(_eh).state() < _target_state) 
  {
    update(_eh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexHandle             vh;
    typename M::HalfedgeHandle           hh1(Inherited::mesh_.HEH(_eh, 0)),
                                            hh2(Inherited::mesh_.HEH(_eh, 1));
    std::vector<typename M::VertexHandle> vertex_vector;
    typename M::FaceHandle                fh1, fh2;

    if (_target_state > 1) {

      fh1 = Inherited::mesh_.FH(hh1);
      fh2 = Inherited::mesh_.FH(hh2);

      if (fh1.is_valid()) {

	Inherited::prev_rule()->raise(fh1, _target_state - 1);

	vh = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh1));
	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }
			   
      if (fh2.is_valid()) {

	Inherited::prev_rule()->raise(fh2, _target_state - 1);

	vh = Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh2));
	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }
			   
      vh = Inherited::mesh_.TVH(hh1);
      Inherited::prev_rule()->raise(vh, _target_state - 1);

      vh = Inherited::mesh_.TVH(hh2);
      Inherited::prev_rule()->raise(vh, _target_state - 1);
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    valence = 2;
    position += MOBJ(Inherited::mesh_.TVH(hh1)).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.TVH(hh2)).position(_target_state - 1);

    if (fh1.is_valid()) {

      position += MOBJ(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh1))).position(_target_state - 1);
      ++valence;
    }

    if (fh2.is_valid()) {
      
      position += MOBJ(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh2))).position(_target_state - 1);
      ++valence;
    }

    if (Inherited::number() == Inherited::subdiv_rule()->Inherited::number() + 1) 
      valence = 4;

    position /= valence;

    MOBJ(_eh).set_position(_target_state, position);
    MOBJ(_eh).inc_state();
  }
}


// ------------------------------------------------------------------ VdEc ----


template<class M>
void
VdEc<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) 
{
  if (MOBJ(_eh).state() < _target_state) 
  {
    update(_eh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexHandle             vh;
    typename M::HalfedgeHandle           hh1(Inherited::mesh_.HEH(_eh, 0)),
                                            hh2(Inherited::mesh_.HEH(_eh, 1));
    std::vector<typename M::VertexHandle> vertex_vector;
    typename M::FaceHandle                fh1, fh2;

    if (_target_state > 1) {

      fh1 = Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 0));
      fh2 = Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 1));

      Inherited::prev_rule()->raise(fh1, _target_state - 1);
      Inherited::prev_rule()->raise(fh2, _target_state - 1);

      vertex_vector.push_back(Inherited::mesh_.TVH(hh1));
      vertex_vector.push_back(Inherited::mesh_.TVH(hh2));

      vertex_vector.push_back(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh1)));
      vertex_vector.push_back(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh2)));

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }

      vertex_vector.push_back(Inherited::mesh_.TVH(hh1));
      vertex_vector.push_back(Inherited::mesh_.TVH(hh2));

      vertex_vector.push_back(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh1)));
      vertex_vector.push_back(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh2)));

      while (!vertex_vector.empty()) {

	vh = vertex_vector.back();
	vertex_vector.pop_back();

	Inherited::prev_rule()->raise(vh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);
    typename M::Scalar c;

    // choose coefficient c
    c = Inherited::coeff();

    valence = 4;
    position += MOBJ(Inherited::mesh_.TVH(hh1)).position(_target_state - 1) * c;
    position += MOBJ(Inherited::mesh_.TVH(hh2)).position(_target_state - 1) * c;
    position += MOBJ(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh1))).position(_target_state - 1) * (0.5 - c);
    position += MOBJ(Inherited::mesh_.TVH(Inherited::mesh_.NHEH(hh2))).position(_target_state - 1) * (0.5 - c);

    position /= valence;

    MOBJ(_eh).set_position(_target_state, position);
    MOBJ(_eh).inc_state();
  }
}


// -------------------------------------------------------------------- EV ----


template<class M>
void EV<M>::raise(typename M::VertexHandle& _vh, state_t _target_state) 
{
  if (MOBJ(_vh).state() < _target_state) {

    update(_vh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexEdgeIter            ve_it(Inherited::mesh_.ve_iter(_vh));
    typename M::EdgeHandle                eh;
    std::vector<typename M::EdgeHandle>   edge_vector;

    if (_target_state > 1) {

      for (; ve_it; ++ve_it) {

	edge_vector.push_back(ve_it.handle());
      }

      while (!edge_vector.empty()) {

	eh = edge_vector.back();
	edge_vector.pop_back();

	Inherited::prev_rule()->raise(eh, _target_state - 1);
      }

      for (ve_it = Inherited::mesh_.ve_iter(_vh); ve_it; ++ve_it) {

	edge_vector.push_back(ve_it.handle());
      }

      while (!edge_vector.empty()) {

	eh = edge_vector.back();
	edge_vector.pop_back();

	while (MOBJ(eh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(eh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (ve_it = Inherited::mesh_.ve_iter(_vh); ve_it; ++ve_it) {

      if (ve_it->final()) {

	++valence;

	position += ve_it->position(_target_state - 1);
      }
    }

    position /= valence;

    MOBJ(_vh).set_position(_target_state, position);
    MOBJ(_vh).inc_state();

    // check if last rule
    if (Inherited::number() == Inherited::n_rules() - 1) {

      Inherited::mesh_.set_point(_vh, position);
      MOBJ(_vh).set_final();
    }
  }
}


// ------------------------------------------------------------------- EVc ----

template<class M>
std::vector<double> EVc<M>::coeffs_;

template<class M>
void EVc<M>::raise(typename M::VertexHandle& _vh, state_t _target_state) 
{
  if (MOBJ(_vh).state() < _target_state) 
  {
    update(_vh, _target_state);

    // raise all neighbour vertices to level x-1
    typename M::VertexOHalfedgeIter       voh_it;
    typename M::EdgeHandle                eh;
    typename M::FaceHandle                fh;
    std::vector<typename M::EdgeHandle>   edge_vector;
    std::vector<typename M::FaceHandle>   face_vector;

    if (_target_state > 1) {

      for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) {

	face_vector.push_back(Inherited::mesh_.FH(voh_it.handle()));
      }

      while (!face_vector.empty()) {

	fh = face_vector.back();
	face_vector.pop_back();

	if (fh.is_valid())
	  Inherited::prev_rule()->raise(fh, _target_state - 1);
      }

      for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) {

	edge_vector.push_back(Inherited::mesh_.EH(voh_it.handle()));

	edge_vector.push_back(Inherited::mesh_.EH(Inherited::mesh_.NHEH(voh_it.handle())));
      }

      while (!edge_vector.empty()) {

	eh = edge_vector.back();
	edge_vector.pop_back();

	while (MOBJ(eh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(eh, _target_state - 1);
      }
    }


    // calculate new position
    typename M::Point        position(0.0, 0.0, 0.0);
    typename M::Scalar       c;
    typename M::Point        zero_point(0.0, 0.0, 0.0);
    int                         valence(0);

    valence = Inherited::mesh_.valence(_vh);
    c       = coeff( valence );

    for (voh_it = Inherited::mesh_.voh_iter(_vh); voh_it; ++voh_it) 
    {
      if (MOBJ(Inherited::mesh_.EH(voh_it.handle())).final()) 
      {
	position += MOBJ(Inherited::mesh_.EH(voh_it.handle())).position(_target_state-1)*c;

        if ( Inherited::mesh_.FH(voh_it.handle()).is_valid() && 
             MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(voh_it.handle()))).final() && 
             MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(voh_it.handle()))).position(_target_state - 1) != zero_point) 
        {
	  position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(voh_it.handle()))).position(_target_state-1) * (1.0-c);
	}
	else {
	  position += MOBJ(Inherited::mesh_.EH(voh_it.handle())).position(_target_state - 1) * (1.0 - c);
	}
      }
      else {
	--valence;
      }
    }

    position /= valence;

    MOBJ(_vh).set_position(_target_state, position);
    MOBJ(_vh).inc_state();

    // check if last rule
    if (Inherited::number() == Inherited::n_rules() - 1) {

      Inherited::mesh_.set_point(_vh, position);
      MOBJ(_vh).set_final();
    }
  }
}


template <class M>
void 
EVc<M>::init_coeffs(size_t _max_valence)
{
  if ( coeffs_.size() == _max_valence+1 ) // equal? do nothing
    return;

  if (coeffs_.size() < _max_valence+1) // less than? add additional valences
  {
    const double _2pi = 2.0*M_PI;
    double c;
  
    if (coeffs_.empty())
      coeffs_.push_back(0.0); // dummy for invalid valences 0,1,2

    for(size_t v=coeffs_.size(); v <= _max_valence; ++v)
    {
      // ( 3/2 + cos ( 2 PI / valence ) )² / 2 - 1
      c = 1.5 + cos( _2pi / v );
      c = c * c * 0.5 - 1.0;
      coeffs_.push_back(c);
    }
  }
}


// -------------------------------------------------------------------- EF ----

template<class M>
void
EF<M>::raise(typename M::FaceHandle& _fh, state_t _target_state) {

  if (MOBJ(_fh).state() < _target_state) {

    update(_fh, _target_state);

    // raise all neighbour edges to level x-1
    typename M::FaceEdgeIter              fe_it(Inherited::mesh_.fe_iter(_fh));
    typename M::EdgeHandle                eh;
    std::vector<typename M::EdgeHandle>   edge_vector;

    if (_target_state > 1) {

      for (; fe_it; ++fe_it) {

	edge_vector.push_back(fe_it.handle());
      }

      while (!edge_vector.empty()) {

	eh = edge_vector.back();
	edge_vector.pop_back();

	Inherited::prev_rule()->raise(eh, _target_state - 1);
      }

      for (fe_it = Inherited::mesh_.fe_iter(_fh); fe_it; ++fe_it) {

	edge_vector.push_back(fe_it.handle());
      }

      while (!edge_vector.empty()) {

	eh = edge_vector.back();
	edge_vector.pop_back();

	while (MOBJ(eh).state() < _target_state - 1)
	  Inherited::prev_rule()->raise(eh, _target_state - 1);
      }
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(0);

    for (fe_it = Inherited::mesh_.fe_iter(_fh); fe_it; ++fe_it) {

      if (fe_it->final()) {

	++valence;

	position += fe_it->position(_target_state - 1);
      }
    }

    assert (valence == 3);

    position /= valence;

    MOBJ(_fh).set_position(_target_state, position);
    MOBJ(_fh).inc_state();
  }
}


// -------------------------------------------------------------------- FE ----


template<class M>
void
FE<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) {

  if (MOBJ(_eh).state() < _target_state) {

    update(_eh, _target_state);

    // raise all neighbour faces to level x-1
    typename M::FaceHandle                fh;

    if (_target_state > 1) {

      fh = Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 0));
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      fh = Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 1));
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      fh = Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 0));
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      fh = Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 1));
      Inherited::prev_rule()->raise(fh, _target_state - 1);
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(2);

    position += MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 0))).position(_target_state - 1);
    
    position += MOBJ(Inherited::mesh_.FH(Inherited::mesh_.HEH(_eh, 1))).position(_target_state - 1);
    
    position /= valence;

    MOBJ(_eh).set_position(_target_state, position);
    MOBJ(_eh).inc_state();
  }
}


// ------------------------------------------------------------------- EdE ----


template<class M>
void
EdE<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) {

  if (MOBJ(_eh).state() < _target_state) {

    update(_eh, _target_state);

    // raise all neighbour faces and edges to level x-1
    typename M::HalfedgeHandle            hh1, hh2;
    typename M::FaceHandle                fh;
    typename M::EdgeHandle                eh;

    hh1 = Inherited::mesh_.HEH(_eh, 0);
    hh2 = Inherited::mesh_.HEH(_eh, 1);

    if (_target_state > 1) {

      fh = Inherited::mesh_.FH(hh1);
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      fh = Inherited::mesh_.FH(hh2);
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh1));
      Inherited::prev_rule()->raise(eh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh1));
      Inherited::prev_rule()->raise(eh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh2));
      Inherited::prev_rule()->raise(eh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh2));
      Inherited::prev_rule()->raise(eh, _target_state - 1);
    }

    // calculate new position
    typename M::Point position(0.0, 0.0, 0.0);
    int                  valence(4);

    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh1))).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh1))).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh2))).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh2))).position(_target_state - 1);

    position /= valence;

    MOBJ(_eh).set_position(_target_state, position);
    MOBJ(_eh).inc_state();
  }
}


// ------------------------------------------------------------------ EdEc ----


template<class M>
void
EdEc<M>::raise(typename M::EdgeHandle& _eh, state_t _target_state) 
{
  if (MOBJ(_eh).state() < _target_state) {

    update(_eh, _target_state);

    // raise all neighbour faces and edges to level x-1
    typename M::HalfedgeHandle            hh1, hh2;
    typename M::FaceHandle                fh;
    typename M::EdgeHandle                eh;

    hh1 = Inherited::mesh_.HEH(_eh, 0);
    hh2 = Inherited::mesh_.HEH(_eh, 1);

    if (_target_state > 1) {

      fh = Inherited::mesh_.FH(hh1);
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      fh = Inherited::mesh_.FH(hh2);
      Inherited::prev_rule()->raise(fh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh1));
      Inherited::prev_rule()->raise(eh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh1));
      Inherited::prev_rule()->raise(eh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh2));
      Inherited::prev_rule()->raise(eh, _target_state - 1);

      eh = Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh2));
      Inherited::prev_rule()->raise(eh, _target_state - 1);
    }

    // calculate new position
    typename M::Point  position(0.0, 0.0, 0.0);
    int                valence(4);
    typename M::Scalar c;

    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh1))).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh1))).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.NHEH(hh2))).position(_target_state - 1);
    position += MOBJ(Inherited::mesh_.EH(Inherited::mesh_.PHEH(hh2))).position(_target_state - 1);

    position /= valence;

    // choose coefficient c
    c = Inherited::coeff();

    position *= (1.0 - c);

    position += MOBJ(_eh).position(_target_state - 1) * c;

    MOBJ(_eh).set_position(_target_state, position);
    MOBJ(_eh).inc_state();
  }
}

#endif // DOXY_IGNORE_THIS

#undef FH 
#undef VH 
#undef EH 
#undef HEH
#undef M
#undef MOBJ

//=============================================================================
} // END_NS_ADAPTIVE
} // END_NS_SUBDIVIDER
} // END_NS_OPENMESH
//=============================================================================
