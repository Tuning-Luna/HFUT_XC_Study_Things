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


/** \file Tools/Kernel_OSG/Traits.hh
    This file defines the default traits and some convenienve macros.
*/


//=============================================================================
//
//  CLASS Traits
//
//=============================================================================

#ifndef OPENMESH_KERNEL_OSG_TRAITS_HH
#define OPENMESH_KERNEL_OSG_TRAITS_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Tools/Kernel_OSG/VectorAdapter.hh>
//
#include <OpenSG/OSGGeometry.h>

//== NAMESPACES ===============================================================

namespace OpenMesh  {
namespace Kernel_OSG {

//== CLASS DEFINITION =========================================================


//== CLASS DEFINITION =========================================================


/** Base class for all mesh traits using the OSGArrayKernelT.
 *
 *  \see The Mesh docu section on \ref mesh_type.
 *  \see Traits.hh for a list of macros for traits classes.
 */
struct Traits : DefaultTraits
{
  typedef osg::Pnt3f            Point;
  typedef osg::Color3ub         Color;
  typedef osg::Vec3f            Normal;
  typedef osg::Vec2f            TexCoord2D;
  typedef osg::Vec3f::ValueType Scalar;
  
};

//=============================================================================
} // namespace Kernel_OSG
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_TRAITS_HH defined
//=============================================================================

