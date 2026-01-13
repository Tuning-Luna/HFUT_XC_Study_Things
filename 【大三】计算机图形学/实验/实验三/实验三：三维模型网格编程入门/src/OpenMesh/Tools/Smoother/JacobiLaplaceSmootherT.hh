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

/** \file JacobiLaplaceSmootherT.hh
    
 */


//=============================================================================
//
//  CLASS JacobiLaplaceSmootherT
//
//=============================================================================

#ifndef OPENMESH_JACOBI_LAPLACE_SMOOTHERT_HH
#define OPENMESH_JACOBI_LAPLACE_SMOOTHERT_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Smoother/LaplaceSmootherT.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Smoother {

//== CLASS DEFINITION =========================================================

/** Laplacian Smoothing.
 *
 */
template <class Mesh>
class JacobiLaplaceSmootherT : public LaplaceSmootherT<Mesh>
{
public:

  typedef JacobiLaplaceSmootherT< Mesh > Self;

  JacobiLaplaceSmootherT( Mesh& _mesh ) : LaplaceSmootherT<Mesh>(_mesh) {}

  // override: alloc umbrellas
  void smooth(unsigned int _n);


protected:

  virtual void compute_new_positions_C0();
  virtual void compute_new_positions_C1();


private:

  OpenMesh::VPropHandleT<typename Mesh::Normal>   umbrellas_;
  OpenMesh::VPropHandleT<typename Mesh::Normal>   squared_umbrellas_;
};


//=============================================================================
} // namespace Smoother
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_JACOBI_LAPLACE_SMOOTHERT_C)
#define OPENMESH_JACOBI_LAPLACE_SMOOTHERT_TEMPLATES
#include "JacobiLaplaceSmootherT.cc"
#endif
//=============================================================================
#endif // OPENMESH_JACOBI_LAPLACE_SMOOTHERT_HH defined
//=============================================================================

