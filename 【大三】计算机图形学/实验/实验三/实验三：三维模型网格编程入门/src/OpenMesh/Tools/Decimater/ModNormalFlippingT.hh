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

/** \file ModNormalFlippingT.hh
    
 */

//=============================================================================
//
//  CLASS ModNormalFlipping
//
//=============================================================================


#ifndef OPENMESH_DECIMATER_MODNORMALFLIPPING_HH
#define OPENMESH_DECIMATER_MODNORMALFLIPPING_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModBaseT.hh>

//== NAMESPACES ===============================================================

namespace OpenMesh { // BEGIN_NS_OPENMESH
namespace Decimater { // BEGIN_NS_DECIMATER


//== CLASS DEFINITION =========================================================

/** Decimating module to avoid flipping of faces.
 *  
 *  This module can be used only as a binary module. The criterion
 *  of allowing/disallowing the collapse is the angular deviation between
 *  the face normal of the orignal faces and normals of the faces after the
 *  collapse. The collapse will pass the test, if the deviation is below
 *  a given threshold.
 */	      
template <typename DecimaterT>
class ModNormalFlippingT : public ModBaseT< DecimaterT >
{
public:

  DECIMATING_MODULE( ModNormalFlippingT, DecimaterT, NormalFlipping );

  typedef typename Mesh::Scalar  Scalar;
  

  ModNormalFlippingT( DecimaterT &_dec) 
    : Base(_dec, true), mesh_(Self::mesh())
  {
    set_max_normal_deviation( 90.0f );
  }
  

  
  /** Compute collapse priority due to angular deviation of face normals
   *  before and after a collapse.
   *
   *  -# Compute for each adjacent face of \c _ci.v0 the face
   *  normal if the collpase would be executed.  
   *
   *  -# Prevent the collapse, if the angle between the original and the
   *  new normal is below a given threshold.
   *  
   *  \param _ci The collapse description
   *  \return LEGAL_COLLAPSE or ILLEGAL_COLLAPSE
   *
   *  \see set_max_normal_deviation()
   */
  float collapse_priority(const CollapseInfo& _ci)
  {
    // simulate collapse
    mesh_.set_point(_ci.v0, _ci.p1);
    
    // check for flipping normals
    typename Mesh::ConstVertexFaceIter vf_it(mesh_, _ci.v0);
    typename Mesh::FaceHandle          fh;
    typename Mesh::Scalar              c(1.0);
    
    for (; vf_it; ++vf_it) 
    {
      fh = vf_it.handle();
      if (fh != _ci.fl && fh != _ci.fr)
      {
        typename Mesh::Normal n1 = mesh_.normal(fh);
        typename Mesh::Normal n2 = mesh_.calc_face_normal(fh);

        c = dot(n1, n2);
        
        if (c < min_cos_)
          break;
      }
    }
      
    // undo simulation changes
    mesh_.set_point(_ci.v0, _ci.p0);

    return float( (c < min_cos_) ? 
		  Self::ILLEGAL_COLLAPSE : 
		  Self::LEGAL_COLLAPSE );
  }



  /// get normal deviation
  Scalar max_normal_deviation() const { return max_deviation_ / M_PI * 180.0; }
  
  
  /** Set normal deviation
   *  
   *  Set the maximum angular deviation of the orignal normal and the new
   *  normal in degrees.
   */
  void set_max_normal_deviation(Scalar _f) { 
    max_deviation_ = (Scalar)(_f / 180.0 * M_PI); 
    min_cos_       = cos(max_deviation_);
  }

  
  
  // only binary mode possible
  void set_binary(bool _b) 
  {
    if (!_b)
    {
      std::cerr << "ModNormalFlippingT can only be used binary\n";
      exit(1);
    }
  }
   

private:

  Mesh&   mesh_;
  Scalar  max_deviation_, min_cos_;
};


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENACG_MODNORMALFLIPPING_HH defined
//=============================================================================

