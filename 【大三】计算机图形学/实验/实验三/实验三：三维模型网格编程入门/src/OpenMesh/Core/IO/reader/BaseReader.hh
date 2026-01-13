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
//  Implements the baseclass for IOManager file access modules
//
//=============================================================================


#ifndef __BASEREADER_HH__
#define __BASEREADER_HH__


//=== INCLUDES ================================================================


// STD C++
#include <iostream>
#include <string>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/IO/importer/BaseImporter.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


/**
   Base class for reader modules.
   Reader modules access persistent data and pass them to the desired
   data structure by the means of a BaseImporter derivative.
   All reader modules must be derived from this class.
*/
class BaseReader
{
public:

  /// Destructor
  virtual ~BaseReader() {}

  /// Returns a brief description of the file type that can be parsed.
  virtual std::string get_description() const = 0;
  
  /** Returns a string with the accepted file extensions separated by a 
      whitespace and in small caps.
  */
  virtual std::string get_extensions() const = 0;

  /// Return magic bits used to determine file format
  virtual std::string get_magic() const { return std::string(""); }


  /** Reads a mesh given by a filename. Usually this method opens a stream
      and passes it to stream read method. Acceptance checks by filename
      extension can be placed here.
  */
  virtual bool read(const std::string& _filename, 
		    BaseImporter& _bi,
                    Options& _opt) = 0;


  /// Returns true if reader can parse _filename (checks extension)
  virtual bool can_u_read(const std::string& _filename) const;


protected:

  // case insensitive search for _ext in _fname.
  bool check_extension(const std::string& _fname, 
		       const std::string& _ext) const;
};


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
