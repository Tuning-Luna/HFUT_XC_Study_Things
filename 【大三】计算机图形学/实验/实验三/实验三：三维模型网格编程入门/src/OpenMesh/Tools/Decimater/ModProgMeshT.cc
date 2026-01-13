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
//   $Date: 2005/04/18 09:07:10 $
//                                                                            
//=============================================================================

/** \file ModProgMeshT.cc
 */


//=============================================================================
//
//  CLASS ModProgMeshT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_DECIMATER_MODPROGMESH_CC


//== INCLUDES =================================================================

#include <vector>
#include <fstream>
// --------------------
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/Utils/Endian.hh>
// --------------------
#include <OpenMesh/Tools/Decimater/ModProgMeshT.hh>


//== NAMESPACE =============================================================== 

namespace OpenMesh  {
namespace Decimater {

   

//== IMPLEMENTATION ========================================================== 


template <class DecimaterType>
bool 
ModProgMeshT<DecimaterType>::
write( const std::string& _ofname )
{
  // sort vertices
  size_t i=0, N=Self::mesh().n_vertices(), n_base_vertices(0), n_base_faces(0);
  std::vector<typename Mesh::VertexHandle>  vhandles(N);


  // base vertices
  typename Mesh::VertexIter 
    v_it=Self::mesh().vertices_begin(), 
    v_end=Self::mesh().vertices_end();

  for (; v_it != v_end; ++v_it)  
    if (!Self::mesh().status(v_it).deleted()) 
    {
      vhandles[i] = v_it.handle();  
      Self::mesh().property( idx_, v_it ) = i;  
      ++i;
    }
  n_base_vertices = i;


  // deleted vertices
  typename InfoList::reverse_iterator
    r_it=pmi_.rbegin(), r_end=pmi_.rend();

  for (; r_it!=r_end; ++r_it)  
  { 
    vhandles[i] = r_it->v0;  
    Self::mesh().property( idx_, r_it->v0) = i;  
    ++i; 
  }


  // base faces
  typename Mesh::ConstFaceIter f_it  = Self::mesh().faces_begin(), 
                               f_end = Self::mesh().faces_end();
  for (; f_it != f_end; ++f_it) 
    if (!Self::mesh().status(f_it).deleted()) 
      ++n_base_faces;

  // ---------------------------------------- write progressive mesh

  std::ofstream out( _ofname.c_str(), std::ios::binary );
    
  if (!out)
    return false;   

  // always use little endian byte ordering
  bool swap = Endian::local() != Endian::LSB;

  // write header
  out << "ProgMesh";
  IO::store( out, n_base_vertices, swap );
  IO::store( out, n_base_faces   , swap );
  IO::store( out, pmi_.size()    , swap );

  Vec3f p;

  // write base vertices
  for (i=0; i<n_base_vertices; ++i)
  {
    assert (!Self::mesh().status(vhandles[i]).deleted());
    p  = vector_cast< Vec3f >( Self::mesh().point(vhandles[i]) );
    
    IO::store( out, p, swap );
  }
    

  // write base faces
  for (f_it=Self::mesh().faces_begin(); f_it != f_end; ++f_it)  
  {
    if (!Self::mesh().status(f_it).deleted()) 
    {
      typename Mesh::ConstFaceVertexIter fv_it(Self::mesh(), f_it.handle());
      
      IO::store( out, Self::mesh().property( idx_,   fv_it ) );
      IO::store( out, Self::mesh().property( idx_, ++fv_it ) );
      IO::store( out, Self::mesh().property( idx_, ++fv_it ) );
    }
  }
  
  
  // write detail info
  for (r_it=pmi_.rbegin(); r_it!=r_end; ++r_it)  
  { 
    // store v0.pos, v1.idx, vl.idx, vr.idx
    IO::store( out, vector_cast<Vec3f>(Self::mesh().point(r_it->v0)));
    IO::store( out, Self::mesh().property( idx_, r_it->v1 ) );
    IO::store( out, 
	       r_it->vl.is_valid() ? Self::mesh().property(idx_, r_it->vl) : -1 );
    IO::store( out, 
	       r_it->vr.is_valid() ? Self::mesh().property(idx_, r_it->vr) : -1 );
  }

  return true;
}



//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================

