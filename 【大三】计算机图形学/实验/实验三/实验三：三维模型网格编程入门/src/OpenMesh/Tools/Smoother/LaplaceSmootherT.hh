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
//   $Date: 2005/04/19 07:55:40 $
//                                                                            
//=============================================================================

/** \file LaplaceSmootherT.hh
    
 */

//=============================================================================
//
//  CLASS LaplaceSmootherT
//
//=============================================================================

#ifndef OPENMESH_LAPLACE_SMOOTHERT_HH
#define OPENMESH_LAPLACE_SMOOTHERT_HH



//== INCLUDES =================================================================

#include <OpenMesh/Tools/Smoother/SmootherT.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Smoother {

//== CLASS DEFINITION =========================================================

/// Laplacian Smoothing.      
template <class Mesh>
class LaplaceSmootherT : public SmootherT<Mesh>
{
public:

  typedef SmootherT<Mesh>                         Base;
  typedef typename SmootherT<Mesh>::Component     Component;
  typedef typename SmootherT<Mesh>::Continuity    Continuity;
  typedef typename SmootherT<Mesh>::Scalar        Scalar;
  typedef typename SmootherT<Mesh>::VertexHandle  VertexHandle;
  typedef typename SmootherT<Mesh>::EdgeHandle    EdgeHandle;
  

  LaplaceSmootherT( Mesh& _mesh );
  virtual ~LaplaceSmootherT();


  void initialize(Component _comp, Continuity _cont);


protected:

  // misc helpers

  Scalar weight(VertexHandle _vh) const 
  { return Base::mesh_.property(vertex_weights_, _vh); }

  Scalar weight(EdgeHandle _eh) const 
  { return Base::mesh_.property(edge_weights_, _eh); }


private:

  enum LaplaceWeighting { UniformWeighting, CotWeighting };
  void compute_weights(LaplaceWeighting _mode);


  OpenMesh::VPropHandleT<Scalar>  vertex_weights_;
  OpenMesh::EPropHandleT<Scalar>  edge_weights_;
};


//=============================================================================
} // namespace Smoother
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_LAPLACE_SMOOTHERT_C)
#define OPENMESH_LAPLACE_SMOOTHERT_TEMPLATES
#include "LaplaceSmootherT.cc"
#endif
//=============================================================================
#endif // OPENMESH_LAPLACE_SMOOTHERT_HH defined
//=============================================================================

