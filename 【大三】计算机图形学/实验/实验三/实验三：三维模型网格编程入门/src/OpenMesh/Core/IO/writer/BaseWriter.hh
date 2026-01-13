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
//  Implements the baseclass for IOManager writer modules
//
//=============================================================================


#ifndef __BASEWRITER_HH__
#define __BASEWRITER_HH__


//=== INCLUDES ================================================================


// STD C++
#include <iostream>
#include <string>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/IO/exporter/BaseExporter.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


/**
   Base class for all writer modules. The module should register itself at
   the IOManager by calling the register_module function.
*/
class BaseWriter
{
public:

  typedef unsigned int Option;
   
  /// Destructor
  virtual ~BaseWriter() {}

  /// Return short description of the supported file format.
  virtual std::string get_description() const = 0;
  
  /// Return file format's extension.
  virtual std::string get_extensions() const = 0;

  /// Returns true if writer can parse _filename (checks extension)
  virtual bool can_u_write(const std::string& _filename) const;

  /// Write to file _filename. Data source specified by BaseExporter _be.
  virtual bool write(const std::string& _filename, 
		     BaseExporter& _be,
                     Options _opt) const = 0;

  /// Returns expected size of file if binary format is supported else 0.
  virtual size_t binary_size(BaseExporter&, Options) const { return 0; }



protected:

  bool check(BaseExporter& _be, Options _opt) const
  {
    return (_opt.check(Options::VertexNormal ) <= _be.has_vertex_normals())
       &&  (_opt.check(Options::VertexTexCoord)<= _be.has_vertex_texcoords())
       &&  (_opt.check(Options::VertexColor)   <= _be.has_vertex_colors())
       &&  (_opt.check(Options::FaceNormal)    <= _be.has_face_normals())
       &&  (_opt.check(Options::FaceColor)     <= _be.has_face_colors());
  }
};


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
