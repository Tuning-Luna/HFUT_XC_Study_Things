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
//  Implements a writer module for OM files
//
//=============================================================================


#ifndef __OMWRITER_HH__
#define __OMWRITER_HH__


//=== INCLUDES ================================================================


// STD C++
#include <iostream>
#include <string>

// OpenMesh
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/OMFormat.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/IO/writer/BaseWriter.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {

//=== FORWARDS ================================================================


class BaseExporter;


//=== IMPLEMENTATION ==========================================================


/** 
 *  Implementation of the OM format writer. This class is singleton'ed by 
 *  SingletonT to OMWriter.
 */
class _OMWriter_ : public BaseWriter
{
public:

  // constructor
  _OMWriter_();

  std::string get_description() const
  { return "OpenMesh Format"; }
  
  std::string get_extensions() const
  { return "om"; }

  bool write(std::ostream&, BaseExporter&, Options) const;


  size_t binary_size(BaseExporter& _be, Options _opt) const;


protected:

  static const OMFormat::uchar magic_[3];
  static const OMFormat::uint8 version_;

  bool write(const std::string&, BaseExporter&, Options) const;

  bool write_binary(std::ostream&, BaseExporter&, Options) const;

  size_t store_binary_custom_chunk( std::ostream&, const BaseProperty&,
				    OMFormat::Chunk::Entity, bool) const;
};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the OM writer.
extern _OMWriter_  __OMWriterInstance;
_OMWriter_& OMWriter();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
