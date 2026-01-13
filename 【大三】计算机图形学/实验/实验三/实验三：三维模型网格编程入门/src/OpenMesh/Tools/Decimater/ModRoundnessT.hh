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
//   $Date: 2005/04/18 09:07:11 $
//                                                                            
//=============================================================================

/** \file ModRoundnessT.hh
    
 */

//=============================================================================
//
//  CLASS ModRoundnessT
//
//=============================================================================

#ifndef OPENMESH_TOOLS_MODROUNDNESST_HH
#define OPENMESH_TOOLS_MODROUNDNESST_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModBaseT.hh>
#include <math.h>



//== NAMESPACE ================================================================

namespace OpenMesh { // BEGIN_NS_OPENMESH
namespace Decimater { // BEGIN_NS_DECIMATER


//== CLASS DEFINITION =========================================================


/** Compute error value based on roundness criteria.
 */
template <class DecimaterType>
class ModRoundnessT : public ModBaseT<DecimaterType>
{
public:

  DECIMATING_MODULE( ModRoundnessT, DecimaterType, Roundness );

  typedef typename Mesh::Point   Point;
  typedef typename Mesh::Scalar  Scalar;


  /// Constructor
  ModRoundnessT( DecimaterType &_dec ) 
    : Base(_dec, true), mesh_(Self::mesh()), min_roundness_(0.2f) 
  {}
 


public:
   

  float collapse_priority(const CollapseInfo& _ci)  
  {    
    typename Mesh::VertexHandle  v2, v3;
    typename Mesh::Scalar        r, rmin(1.0);
    typename Mesh::CVVIter       vv_it(mesh_, _ci.v0);
    

    v3 = vv_it.handle();
    while (vv_it) 
    {
      v2 = v3;
      v3 = (++vv_it).handle(); 
      
      // if not boundary
      if (mesh_.face_handle(vv_it.current_halfedge_handle()).is_valid())
      {
	if (v2 != _ci.v1 && v3 != _ci.v1)
	{
	  r = roundness(_ci.p1, mesh_.point(v2), mesh_.point(v3));
	  if (r < rmin) rmin = r;
	}
      }
    }


    if (Base::is_binary())
    {
      return (rmin > min_roundness_) ? 0.0f : -1.0f;
    }
    else
    {
      return (rmin > min_roundness_) ? 1.0f - rmin : -1.0f;
    }
  }
  
   

  /** Set a minimum roundness value.
   *  \param _min_roundness in range (0,1)
   */
  void set_min_roundness( Scalar _min_roundness )
  {
    assert( 0.0 <= _min_roundness && _min_roundness <= 1.0 );
    min_roundness_  = _min_roundness;
  }



  // compute roundness as 1.16 / aspect_ratio
  Scalar roundness(const Point& _v0, const Point& _v1, const Point& _v2) const
  {
    const double FOUR_ROOT3 = 6.928203230275509;
    const double area = 0.5*((_v1-_v0)%(_v2-_v0)).norm();
    return (Scalar) (FOUR_ROOT3 * area / ((_v0-_v1).sqrnorm() +
					  (_v1-_v2).sqrnorm() +
					  (_v2-_v0).sqrnorm() ));
  }



private:
  
  Mesh&   mesh_;
  Scalar  min_roundness_;
};


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENMESH_TOOLS_PROGMESHT_HH defined
//=============================================================================

