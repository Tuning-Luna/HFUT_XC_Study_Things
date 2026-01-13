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
//   $Revision: 1.3 $
//   $Date: 2005/04/18 09:07:12 $
//                                                                            
//=============================================================================

/** \file CompositeLoopT.hh
    
 */

//=============================================================================
//
//  CLASS LoopT
//
//=============================================================================

#ifndef OPENMESH_SUBDIVIDER_UNIFORM_COMPOSITELOOPT_HH
#define OPENMESH_SUBDIVIDER_UNIFORM_COMPOSITELOOPT_HH


//== INCLUDES =================================================================

#include "Composite/CompositeT.hh"
#include "Composite/CompositeTraits.hh"


//== NAMESPACE ================================================================

namespace OpenMesh   { // BEGIN_NS_OPENMESH
namespace Subdivider { // BEGIN_NS_DECIMATER
namespace Uniform    { // BEGIN_NS_DECIMATER


//== CLASS DEFINITION =========================================================

/** Uniform composite Loop subdivision algorithm
 */
template <class MeshType, class RealType=float>
class CompositeLoopT : public CompositeT<MeshType, RealType>
{
public:

  typedef CompositeT<MeshType, RealType>  Inherited;
   
public:

  CompositeLoopT() : Inherited() {};
  ~CompositeLoopT() {}

public:
  
  const char *name() const { return "Uniform Composite Loop"; }
  
protected: // inherited interface

  void apply_rules(void)  { Inherited::Tvv4(); Inherited::VdE(); EVc(coeffs_); Inherited::VdE(); EVc(coeffs_); }

protected:
  
  typedef typename Inherited::Coeff Coeff;
  
  struct EVCoeff : public Coeff 
  {
    EVCoeff() : Coeff() { init(50); }

    void init(size_t _max_valence)
    {
      weights_.resize(_max_valence);
      std::generate(weights_.begin(), 
                    weights_.end(), compute_weight() );
    }
    
    double operator()(size_t _valence) { return weights_[_valence]; }


    struct compute_weight 
    {
      compute_weight() : val_(0) { }

      double operator()(void) // Loop weights for non-boundary vertices
      {
        // 1      3          2 * pi
        // - * ( --- + cos ( ------- ) )² - 1.0
        // 2      2          valence        
        double f1 = 1.5 + cos(2.0*M_PI/val_++);
        return 0.5 * f1 * f1 - 1.0;
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
#endif // OPENMESH_SUBDIVIDER_UNIFORM_COMPOSITELOOPT_HH defined
//=============================================================================
