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
//   $Date: 2005/08/11 09:42:00 $
//                                                                            
//=============================================================================


#define OPENMESH_MESHCHECKER_C


//== INCLUDES =================================================================


#include <OpenMesh/Tools/Utils/MeshCheckerT.hh>


//== NAMESPACES ============================================================== 


namespace OpenMesh {
namespace Utils {

//== IMPLEMENTATION ========================================================== 


template <class Mesh>
bool 
MeshCheckerT<Mesh>::
check(unsigned int _targets, std::ostream& _os)
{
  bool  ok(true);



  //--- vertex checks ---

  if (_targets & CHECK_VERTICES)
  {
    typename Mesh::ConstVertexIter v_it(mesh_.vertices_begin()), 
                                   v_end(mesh_.vertices_end());
    typename Mesh::VertexHandle    vh;
    typename Mesh::ConstVertexVertexIter vv_it;
    typename Mesh::HalfedgeHandle  heh;
    unsigned int                   count;
    const unsigned int             max_valence(10000);


    for (; v_it != v_end; ++v_it)
    {
      if (!is_deleted(v_it))
      {
	vh = v_it.handle();


      /* The outgoing halfedge of a boundary vertex has to be a 
	 boundary halfedge */
	heh = mesh_.halfedge_handle(vh);
	if (heh.is_valid() && !mesh_.is_boundary(heh))
	{
	  for (typename Mesh::ConstVertexOHalfedgeIter vh_it(mesh_, vh);
	       vh_it; ++vh_it)
	  {
	    if (mesh_.is_boundary(vh_it.handle()))
	    {
	      _os << "MeshChecker: vertex " << vh
		  << ": outgoing halfedge not on boundary error\n";
	      ok = false;
	    }
	  }
	}

      
      
	// outgoing halfedge has to refer back to vertex
	if (mesh_.halfedge_handle(vh).is_valid() &&
	    mesh_.from_vertex_handle(mesh_.halfedge_handle(vh)) != vh)
	{
	  _os << "MeshChecker: vertex " << vh
	      << ": outgoing halfedge does not reference vertex\n";
	  ok = false;
	}


	// check whether circulators are still in order
	vv_it = mesh_.cvv_iter(vh);
	for (count=0; vv_it && (count < max_valence); ++vv_it, ++count);
	if (count == max_valence)
	{
	  _os << "MeshChecker: vertex " << vh
	      << ": ++circulator problem, one ring corrupt\n";
	  ok = false;
	}
	vv_it = mesh_.cvv_iter(vh);
	for (count=0; vv_it && (count < max_valence); --vv_it, ++count);
	if (count == max_valence)
	{
	  _os << "MeshChecker: vertex " << vh
	      << ": --circulator problem, one ring corrupt\n";
	  ok = false;
	}
      }
    }
  }



  //--- halfedge checks ---

  if (_targets & CHECK_EDGES)
  {
    typename Mesh::ConstHalfedgeIter  h_it(mesh_.halfedges_begin()), 
                                      h_end(mesh_.halfedges_end());
    typename Mesh::HalfedgeHandle     hh, hstart, hhh;
    unsigned int                      count, n_halfedges = 2*mesh_.n_edges();

    for (; h_it != h_end; ++h_it)
    {
      if (!is_deleted(mesh_.edge_handle(h_it.handle())))
      {
	hh = h_it.handle();


	// degenerated halfedge ?
	if (mesh_.from_vertex_handle(hh) == mesh_.to_vertex_handle(hh))
	{
	  _os << "MeshChecker: halfedge " << hh
	      << ": to-vertex == from-vertex\n";
	  ok = false;
	}


	// next <-> prev check
	if (mesh_.next_halfedge_handle(mesh_.prev_halfedge_handle(hh)) != hh)
	{
	  _os << "MeshChecker: halfedge " << hh
	      << ": prev->next != this\n";
	  ok = false;
	}


	// halfedges should form a cycle
	count=0; hstart=hhh=hh;
	do 
	{
	  hhh = mesh_.next_halfedge_handle(hhh);
	  ++count;
	} while (hhh != hstart && count < n_halfedges);

	if (count == n_halfedges)
	{
	  _os << "MeshChecker: halfedges starting from " << hh
	      << " do not form a cycle\n";
	  ok = false;
	}
      }
    }
  }



  //--- face checks ---

  if (_targets & CHECK_FACES)
  {
    typename Mesh::ConstFaceIter          f_it(mesh_.faces_begin()), 
                                          f_end(mesh_.faces_end());
    typename Mesh::FaceHandle             fh;
    typename Mesh::ConstFaceHalfedgeIter  fh_it;
    
    for (; f_it != f_end; ++f_it)
    {
      if (!is_deleted(f_it))
      {
	fh = f_it.handle();

	for (fh_it=mesh_.cfh_iter(fh); fh_it; ++fh_it)
	{
	  if (mesh_.face_handle(fh_it.handle()) != fh)
	  {
	    _os << "MeshChecker: face " << fh
		<< ": its halfedge "
		<< fh_it.handle()
		<< " does not reference face\n";
	    ok = false;
	  }
	}
      }
    }
  }



  return ok;
}


//=============================================================================
} // naespace Utils
} // namespace OpenMesh
//=============================================================================
