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
//   $Date: 2004/09/06 12:35:24 $
//
//=============================================================================

/** \file config.hh
 *  \todo Move content to config.hh and include it to be compatible with old
 *  source.
 */

//=============================================================================

#ifndef OPENMESH_CONFIG_H
#define OPENMESH_CONFIG_H

//=============================================================================

#include <assert.h>
#include <OpenMesh/Core/System/compiler.hh>

// ----------------------------------------------------------------------------

#define OM_VERSION 0x10000

// only defined, if it is a beta version
#define OM_VERSION_BETA 4

#define OM_GET_VER ((OM_VERSION && 0xf0000) >> 16)
#define OM_GET_MAJ ((OM_VERSION && 0x0ff00) >> 8)
#define OM_GET_MIN  (OM_VERSION && 0x000ff)

typedef unsigned int uint;
//=============================================================================
#endif // OPENMESH_CONFIG_H defined
//=============================================================================
