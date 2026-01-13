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

/** \file CompositeSqrt3T.hh
    
 */

//=============================================================================
//
//  CLASS SQRT3T
//
//=============================================================================

#ifndef OPENMESH_SUBDIVIDER_UNIFORM_COMPOSITESQRT3T_HH
#define OPENMESH_SUBDIVIDER_UNIFORM_COMPOSITESQRT3T_HH


//== INCLUDES =================================================================

#include "Composite/CompositeT.hh"
#include "Composite/CompositeTraits.hh"


//== NAMESPACE ================================================================

namespace OpenMesh   { // BEGIN_NS_OPENMESH
namespace Subdivider { // BEGIN_NS_DECIMATER
namespace Uniform    { // BEGIN_NS_UNIFORM


//== CLASS DEFINITION =========================================================

/** Uniform composite sqrt(3) subdivision algorithm
 */
template <typename MeshType, typename RealType=float>
class CompositeSqrt3T : public CompositeT<MeshType, RealType>
{
public:

   typedef CompositeT<MeshType, RealType> Inherited;

public:

  CompositeSqrt3T() : Inherited() {};
  ~CompositeSqrt3T() {}

public:
   
  const char *name() const { return "Uniform Composite Sqrt3"; }

protected: // inherited interface

  void apply_rules(void)  { Inherited::Tvv3(); Inherited::VF(); Inherited::FF(); FVc(coeffs_); }

protected:
   
   typedef typename Inherited::Coeff Coeff;

  /** Helper class
   *  \internal
   */
  struct FVCoeff : public Coeff 
  {
    FVCoeff() : Coeff() { init(50); }

    void init(size_t _max_valence)
    {
      weights_.resize(_max_valence);
      std::generate(weights_.begin(), 
                    weights_.end(), compute_weight() );
    }
    
    double operator()(size_t _valence) { return weights_[_valence]; }

  /** \internal
   */
    struct compute_weight 
    {
      compute_weight() : val_(0) { }

      double operator()(void) // sqrt(3) weights for non-boundary vertices
      { 
        return 2.0/3.0 * (cos(2.0*M_PI/val_++)+1.0); 
      }
      size_t val_;
    };

    std::vector<double> weights_;
    
  } coeffs_;

};


//=============================================================================
} // END_NS_UNIFORM
} // END_NS_SUBDIVIDER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENMESH_SUBDIVIDER_UNIFORM_COMPOSITESQRT3T_HH defined
//=============================================================================
