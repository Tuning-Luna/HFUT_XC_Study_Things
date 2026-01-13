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
//   $Date: 2004/09/06 12:35:07 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================

#ifndef OPENMESH_SR_BINARY_HH
#define OPENMESH_SR_BINARY_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
// -------------------- STL
#include <typeinfo>
#include <stdexcept>
#include <sstream>
#include <numeric>   // accumulate
// -------------------- OpenMesh


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace IO {


//=============================================================================


//-----------------------------------------------------------------------------

  const static size_t UnknownSize(size_t(-1));


//-----------------------------------------------------------------------------
// struct binary, helper for storing/restoring

#define X \
   std::ostringstream msg; \
   msg << "Type not supported: " << typeid(value_type).name(); \
   throw std::logic_error(msg.str())

/// \struct binary SR_binary.hh <OpenMesh/Core/IO/SR_binary.hh>
///
/// The struct defines how to store and restore the type T.
/// It's used by the OM reader/writer modules.
///
/// The following specialization are provided:
/// - Fundamental types except \c long \c double
/// - %OpenMesh vector types
/// - %OpenMesh::StatusInfo
/// - std::string (max. length 65535)
///
/// \todo Complete documentation of members
template < typename T > struct binary
{
  typedef T     value_type;

  static const bool is_streamable = false;

  static size_t size_of(void) { return UnknownSize; }
  static size_t size_of(const value_type&) { return UnknownSize; }

  static 
  size_t store(std::ostream& _os, const value_type& _v, bool _swap=false)
  { X; return 0; }

  static 
  size_t restore( std::istream& _is, value_type& _v, bool _swap=false)
  { X; return 0; }
};

#undef X


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_SR_RBO_HH defined
//=============================================================================

