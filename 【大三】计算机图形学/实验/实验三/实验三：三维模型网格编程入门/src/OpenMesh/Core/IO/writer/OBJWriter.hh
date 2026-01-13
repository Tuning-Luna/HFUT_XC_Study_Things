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


//=============================================================================
//
//  Implements an IOManager writer module for OBJ files
//
//=============================================================================


#ifndef __OBJWRITER_HH__
#define __OBJWRITER_HH__


//=== INCLUDES ================================================================


#include <string>
#include <stdio.h>

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/exporter/BaseExporter.hh>
#include <OpenMesh/Core/IO/writer/BaseWriter.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


/** 
    This class defines the OBJ writer. This class is further singleton'ed
    by SingletonT to OBJWriter.
*/
class _OBJWriter_ : public BaseWriter
{
public:

  _OBJWriter_();

  std::string get_description() const  { return "Alias/Wavefront"; }
  std::string get_extensions()  const  { return "obj"; }

  bool write(const std::string&, BaseExporter&, Options) const;

  size_t binary_size(BaseExporter&, Options) const { return 0; }

private:

  bool write(FILE*, BaseExporter&, Options) const;
};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the OBJ writer
extern _OBJWriter_  __OBJWriterinstance;
_OBJWriter_& OBJWriter();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
