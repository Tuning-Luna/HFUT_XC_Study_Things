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
//   $Date: 2004/09/06 12:36:59 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  CLASS TriMesh_OSGArrayKernelT
//
//=============================================================================


#ifndef OPENMESH_KERNEL_OSG_TRIMESH_OSGARRAYKERNEL_HH
#define OPENMESH_KERNEL_OSG_TRIMESH_OSGARRAYKERNEL_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
// --------------------
#include <OpenMesh/Core/Mesh/TriMeshT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/Handles.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh>
// --------------------
#include <OpenMesh/Tools/Kernel_OSG/VectorAdapter.hh>
#include <OpenMesh/Tools/Kernel_OSG/Traits.hh>
#include <OpenMesh/Tools/Kernel_OSG/ArrayKernelT.hh>
// --------------------
#include <OpenSG/OSGGeometry.h>


//== NAMESPACES ===============================================================


namespace OpenMesh   {
namespace Kernel_OSG {

//== CLASS DEFINITION =========================================================


/// Helper class to create a TriMesh-type based on Kernel_OSG::ArrayKernelT
template <class Traits>
struct TriMesh_OSGArrayKernel_GeneratorT
{
  typedef FinalMeshItemsT<ArrayItems, Traits, true>  MeshItems;
  typedef AttribKernelT<MeshItems>                   AttribKernel;
  typedef ArrayKernelT<AttribKernel, MeshItems>      MeshKernel;
  typedef TriMeshT<MeshKernel>                       Mesh;
};



/** \ingroup mesh_types_group 
    Triangle mesh based on the Kernel_OSG::ArrayKernelT.
    \see OpenMesh::TriMeshT
    \see OpenMesh::ArrayKernelT
*/
template <class Traits = Kernel_OSG::Traits>  
class TriMesh_OSGArrayKernelT 
  : public TriMesh_OSGArrayKernel_GeneratorT<Traits>::Mesh 
{};


//=============================================================================
} // namespace Kernel_OSG
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_KERNEL_OSG_TRIMESH_OSGARRAYKERNEL_HH
//=============================================================================
