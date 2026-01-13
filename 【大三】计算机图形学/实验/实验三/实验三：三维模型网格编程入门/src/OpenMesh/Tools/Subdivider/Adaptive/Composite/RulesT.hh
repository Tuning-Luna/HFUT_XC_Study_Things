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

/** \file RulesT.hh
    
 */


//=============================================================================
//
//  Composite Subdivision and Averaging Rules
//
//=============================================================================

#ifndef OPENMESH_SUBDIVIDER_ADAPTIVE_RULEST_HH
#define OPENMESH_SUBDIVIDER_ADAPTIVE_RULEST_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Tools/Subdivider/Adaptive/Composite/RuleInterfaceT.hh>
// -------------------- STL
#include <vector>


#if defined(OM_CC_MIPS) // avoid warnings
#  define MIPS_WARN_WA( Item ) \
  void raise(typename M:: ## Item ## Handle &_h, state_t _target_state ) \
  { Inherited::raise(_h, _target_state); }
#else
#  define MIPS_WARN_WA( Item )
#endif

//== NAMESPACE ================================================================

namespace OpenMesh   { // BEGIN_NS_OPENMESH
namespace Subdivider { // BEGIN_NS_SUBDIVIDER
namespace Adaptive   { // BEGIN_NS_ADAPTIVE


//== CLASS DEFINITION =========================================================

/** Adaptive Composite Subdivision framework.
*/


//=============================================================================
  
/** Topological composite rule Tvv,3 doing a 1-3 split of a face.
 */
template <class M> class Tvv3 : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( Tvv3, M );

public:

  typedef RuleInterfaceT<M> Inherited;

  Tvv3(M& _mesh) : Inherited(_mesh) { Inherited::set_subdiv_type(3); };

  void raise(typename M::FaceHandle&   _fh, state_t _target_state);
  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Edge); // avoid warning
};


//=============================================================================


/** Topological composite rule Tvv,4 doing a 1-4 split of a face
 */
template <class M> class Tvv4 : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( Tvv4, M );

public:
  typedef typename M::HalfedgeHandle HEH;
  typedef typename M::VertexHandle   VH;
   
  typedef RuleInterfaceT<M> Inherited;

  Tvv4(M& _mesh) : Inherited(_mesh) { Inherited::set_subdiv_type(4); };

  void raise(typename M::FaceHandle&   _fh, state_t _target_state); 
  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  void raise(typename M::EdgeHandle&   _eh, state_t _target_state);

private:

  void split_edge(HEH& _hh, VH& _vh, state_t _target_state);
  void check_edge(const typename M::HalfedgeHandle& _hh, 
                  state_t _target_state);
};


//=============================================================================


/** Composite rule VF
 */
template <class M> class VF : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( VF, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  VF(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::FaceHandle& _fh, state_t _target_state);
  MIPS_WARN_WA(Edge);
  MIPS_WARN_WA(Vertex);
};


//=============================================================================


/** Composite rule FF
 */
template <class M> class FF : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( FF, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  FF(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::FaceHandle& _fh, state_t _target_state);
  MIPS_WARN_WA(Vertex); // avoid warning
  MIPS_WARN_WA(Edge  ); // avoid warning
};


//=============================================================================


/** Composite rule FFc
 */
template <class M> class FFc : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( FFc, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  FFc(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::FaceHandle& _fh, state_t _target_state);
  MIPS_WARN_WA(Vertex); // avoid warning
  MIPS_WARN_WA(Edge  ); // avoid warning
};


//=============================================================================


/** Composite rule FV
 */
template <class M> class FV : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( FV, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  FV(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Face); // avoid warning
  MIPS_WARN_WA(Edge); // avoid warning
};


//=============================================================================


/** Composite rule FVc
 */
template <class M> class FVc : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( FVc, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  FVc(M& _mesh) : Inherited(_mesh) { init_coeffs(50); }

  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Face); // avoid warning
  MIPS_WARN_WA(Edge); // avoid warning

  static void init_coeffs(size_t _max_valence);
  static const std::vector<double>& coeffs() { return coeffs_; }

  double coeff( size_t _valence )
  {
    assert(_valence < coeffs_.size());
    return coeffs_[_valence];
  }

private:

  static std::vector<double> coeffs_;

};


//=============================================================================


/** Composite rule VV
 */
template <class M> class VV : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( VV, M );

public:

  typedef RuleInterfaceT<M> Inherited;

  VV(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Face); // avoid warning
  MIPS_WARN_WA(Edge); // avoid warning
};


//=============================================================================


/** Composite rule VVc
 */
template <class M> class VVc : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( VVc, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  VVc(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Face); // avoid warning
  MIPS_WARN_WA(Edge); // avoid warning
};


//=============================================================================


/** Composite rule VE
 */
template <class M> class VE : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( VE, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  VE(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::EdgeHandle& _eh, state_t _target_state);
  MIPS_WARN_WA(Face  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};


//=============================================================================


/** Composite rule VdE
 */
template <class M> class VdE : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( VdE, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  VdE(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::EdgeHandle& _eh, state_t _target_state);
  MIPS_WARN_WA(Face  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};


//=============================================================================


/** Composite rule VdEc
 */
template <class M> class VdEc : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( VdEc, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  VdEc(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::EdgeHandle& _eh, state_t _target_state);
  MIPS_WARN_WA(Face  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};


//=============================================================================


/** Composite rule EV
 */
template <class M> class EV : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( EV, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  EV(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Face); // avoid warning
  MIPS_WARN_WA(Edge); // avoid warning
};


//=============================================================================


/** Composite rule EVc
 */
template <class M> class EVc : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( EVc, M );

public:

  typedef RuleInterfaceT<M> Inherited;

  EVc(M& _mesh) : Inherited(_mesh) { init_coeffs(50); }

  void raise(typename M::VertexHandle& _vh, state_t _target_state);
  MIPS_WARN_WA(Face); // avoid warning
  MIPS_WARN_WA(Edge); // avoid warning

  static void init_coeffs(size_t _max_valence);
  static const std::vector<double>& coeffs() { return coeffs_; }

  double coeff( size_t _valence )
  {
    assert(_valence < coeffs_.size());
    return coeffs_[_valence];
  }

private:

  static std::vector<double> coeffs_;
  
};


//=============================================================================


/** Composite rule EF
 */
template <class M> class EF : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( EF, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  EF(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::FaceHandle& _fh, state_t _target_state);
  MIPS_WARN_WA(Edge  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};


//=============================================================================


/** Composite rule FE
 */
template <class M> class FE : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( FE, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  FE(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::EdgeHandle& _eh, state_t _target_state);
  MIPS_WARN_WA(Face  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};


//=============================================================================


/** Composite rule EdE
 */
template <class M> class EdE : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( EdE, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  EdE(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::EdgeHandle& _eh, state_t _target_state);
  MIPS_WARN_WA(Face  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};


//=============================================================================


/** Composite rule EdEc
 */
template <class M> class EdEc : public RuleInterfaceT<M>
{
  COMPOSITE_RULE( EdEc, M );

public:
  typedef RuleInterfaceT<M> Inherited;

  EdEc(M& _mesh) : Inherited(_mesh) {}

  void raise(typename M::EdgeHandle& _eh, state_t _target_state);
  MIPS_WARN_WA(Face  ); // avoid warning
  MIPS_WARN_WA(Vertex); // avoid warning
};

// ----------------------------------------------------------------------------

#undef MIPS_WARN_WA

//=============================================================================
} // END_NS_ADAPTIVE
} // END_NS_SUBDIVIDER
} // END_NS_OPENMESH
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_SUBDIVIDER_ADAPTIVE_RULEST_CC)
#  define OPENMESH_SUBDIVIDER_TEMPLATES
#  include "RulesT.cc"
#endif
//=============================================================================
#endif // OPENMESH_SUBDIVIDER_ADAPTIVE_RULEST_HH defined
//=============================================================================

