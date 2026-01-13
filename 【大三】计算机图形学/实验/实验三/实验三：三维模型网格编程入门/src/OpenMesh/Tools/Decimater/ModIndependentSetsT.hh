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
//   $Date: 2005/05/05 11:17:48 $
//                                                                            
//=============================================================================

/** \file ModQuadricT.hh
    
 */

//=============================================================================
//
//  CLASS ModQuadricT
//
//=============================================================================

#ifndef OPENMESH_TOOLS_MODINDEPENDENTSETST_HH
#define OPENMESH_TOOLS_MODINDEPENDENTSETST_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModBaseT.hh>


//== NAMESPACE ================================================================

namespace OpenMesh { // BEGIN_NS_OPENMESH
namespace Decimater { // BEGIN_NS_DECIMATER


//== CLASS DEFINITION =========================================================


/** Lock one-ring around remaining vertex after a collapse to prevent
 *  further collapses of halfedges incident to the one-ring vertices.
 */
template <class DecimaterType>
class ModIndependentSetsT : public ModBaseT<DecimaterType>
{
public:

  DECIMATING_MODULE( ModIndependentSetsT, DecimaterType, IndependentSets );

  /// Constructor
  ModIndependentSetsT( DecimaterType &_dec ) : Base(_dec, true) {}

  
  // only binary mode possible
  void set_binary(bool _b) 
  {
    if (!_b)
    {
      std::cerr << "ModNormalFlippingT can only be used binary\n";
      exit(1);
    }
  }


  // lock one-ring
  void postprocess_collapse(const CollapseInfo& _ci)
  { 
    typename std::vector<typename Mesh::VertexHandle>::const_iterator
      vh_it(_ci.one_ring.begin()), vh_end(_ci.one_ring.end());
    for (; vh_it!=vh_end; ++vh_it)
      Self::mesh().status(*vh_it).set_locked(true);    
  }
};


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENMESH_TOOLS_MODINDEPENDENTSETST_HH defined
//=============================================================================

