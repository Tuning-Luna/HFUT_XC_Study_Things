/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Library General Public License as published   *
 *  by the Free Software Foundation, version 2.                              *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Library General Public License for more details.                         *
 *                                                                           *
 *  You should have received a copy of the GNU Library General Public        *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/


//=== INCLUDES ================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/writer/BaseWriter.hh>
#include <algorithm>
#include <string>
#include <iterator>
#if defined(OM_CC_MIPS)
#  include <ctype.h>
#else
#  include <cctype>
#endif


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================

#ifndef DOXY_IGNORE_THIS

//static inline char tolower(char c)
//{
//  using namespace std;
//  return ::tolower(c); 
//}

#endif

//-----------------------------------------------------------------------------


bool 
BaseWriter::
can_u_write(const std::string& _filename) const 
{
  // get file extension
  std::string extension;
  std::string::size_type pos(_filename.rfind("."));

  if (pos != std::string::npos)
  { 
    extension = _filename.substr(pos+1, _filename.length()-pos-1);

    std::transform( extension.begin(), extension.end(), 
		    extension.begin(), tolower );
  }

  // locate extension in extension string
  return (get_extensions().find(extension) != std::string::npos);
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
