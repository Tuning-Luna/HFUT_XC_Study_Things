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
//   $Date: 2004/09/06 12:35:34 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Implements the Non-Copyable metapher
//
//=============================================================================

#ifndef OPENMESH_NONCOPYABLE_HH
#define OPENMESH_NONCOPYABLE_HH


//-----------------------------------------------------------------------------

#include <OpenMesh/Core/System/config.hh>

//-----------------------------------------------------------------------------

namespace OpenMesh {
namespace Utils {

//-----------------------------------------------------------------------------
   
/** This class demonstrates the non copyable idiom. In some cases it is
    important an object can't be copied. Deriving from Noncopyable makes sure
    all relevant constructor and operators are made inaccessable, for public
    AND derived classes.
**/
class Noncopyable
{
public:
  Noncopyable() { }

private:
  /// Prevent access to copy constructor
  Noncopyable( const Noncopyable& );
  
  /// Prevent access to assignment operator
  const Noncopyable& operator=( const Noncopyable& );
};

//=============================================================================
} // namespace Utils
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_NONCOPYABLE_HH
//=============================================================================
