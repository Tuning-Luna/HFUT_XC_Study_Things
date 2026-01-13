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
//   $Date: 2004/09/06 12:37:50 $
//                                                                            
//=============================================================================

//=============================================================================
//
//  CLASS StripifierT
//
//=============================================================================


#ifndef OPENMESH_STRIPIFIERT_HH
#define OPENMESH_STRIPIFIERT_HH


//== INCLUDES =================================================================

#include <vector>
#include <OpenMesh/Core/Utils/Property.hh>


//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {


//== CLASS DEFINITION =========================================================



	      
/** \class StripifierT StripifierT.hh <OpenMesh/Tools/Utils/StripifierT.hh>
    This class decomposes a triangle mesh into several triangle strips.
*/

template <class Mesh>
class StripifierT
{
public:
   
  typedef unsigned int                      Index;
  typedef std::vector<Index>                Strip;
  typedef typename Strip::const_iterator    IndexIterator;
  typedef std::vector<Strip>                Strips;
  typedef typename Strips::const_iterator   StripsIterator;


  /// Default constructor
  StripifierT(Mesh& _mesh) : mesh_(_mesh) {}

  /// Destructor
  ~StripifierT() {}


  /// Compute triangle strips, returns number of strips
  unsigned int stripify();

  /// delete all strips
  void clear() { Strips().swap(strips_); }

  /// returns number of strips
  unsigned int n_strips() const { return strips_.size(); }

  /// are strips computed?
  bool is_valid() const { return !strips_.empty(); }

  /// Access strips
  StripsIterator begin() const { return strips_.begin(); }
  /// Access strips
  StripsIterator end()   const { return strips_.end(); }


private:

  typedef std::vector<typename Mesh::FaceHandle>  FaceHandles;


  /// this method does the main work
  void build_strips();

  /// build a strip from a given halfedge (in both directions)
  void build_strip(typename Mesh::HalfedgeHandle _start_hh,
		   Strip& _strip,
		   FaceHandles& _faces);

  FPropHandleT<bool>::reference  processed(typename Mesh::FaceHandle _fh) {
    return mesh_.property(processed_, _fh);
  }
  FPropHandleT<bool>::reference  used(typename Mesh::FaceHandle _fh) {
    return mesh_.property(used_, _fh);
  }



private:

  Mesh&                mesh_;
  Strips               strips_;
  FPropHandleT<bool>   processed_, used_;
};


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_STRIPIFIERT_C)
#define OPENMESH_STRIPIFIERT_TEMPLATES
#include "StripifierT.cc"
#endif
//=============================================================================
#endif // OPENMESH_STRIPIFIERT_HH defined
//=============================================================================
