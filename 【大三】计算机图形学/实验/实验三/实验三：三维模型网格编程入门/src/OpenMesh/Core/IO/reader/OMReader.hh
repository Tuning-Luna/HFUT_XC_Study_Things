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
//  Implements a reader module for OFF files
//
//=============================================================================


#ifndef __OMREADER_HH__
#define __OMREADER_HH__


//=== INCLUDES ================================================================

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/OMFormat.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/IO/importer/BaseImporter.hh>
#include <OpenMesh/Core/IO/reader/BaseReader.hh>

// STD C++
#include <iostream>
#include <string>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//== IMPLEMENTATION ===========================================================


/** 
    Implementation of the OM format reader. This class is singleton'ed by 
    SingletonT to OMReader.
*/
class _OMReader_ : public BaseReader
{
public:

  _OMReader_();
  virtual ~_OMReader_() { }

  std::string get_description() const { return "OpenMesh File Format"; }
  std::string get_extensions()  const { return "om"; }
  std::string get_magic()       const { return "OM"; }
   
  bool read(const std::string& _filename, 
	    BaseImporter& _bi, 
	    Options& _opt );

  virtual bool can_u_read(const std::string& _filename) const;
  virtual bool can_u_read(std::istream& _is) const;

  
private:

  bool supports( const OMFormat::uint8 version ) const;

  bool read(std::istream& _is, BaseImporter& _bi, Options& _opt );
  bool read_ascii(std::istream& _is, BaseImporter& _bi, Options& _opt);
  bool read_binary(std::istream& _is, BaseImporter& _bi, Options& _opt);

  typedef OMFormat::Header              Header;
  typedef OMFormat::Chunk::Header       ChunkHeader;
  typedef OMFormat::Chunk::PropertyName PropertyName;

  // initialized/updated by read_binary*/read_ascii*
  size_t                  bytes_;
  Header                  header_;
  ChunkHeader             chunk_header_;
  PropertyName            property_name_;
  BaseImporter::VHandles  vhandles_;


  bool read_binary_vertex_chunk(   std::istream      &_is, 
				   BaseImporter      &_bi, 
				   Options           &_opt,
				   bool              _swap);

  bool read_binary_face_chunk(     std::istream      &_is, 
			           BaseImporter      &_bi, 
			           Options           &_opt,
				   bool              _swap);

  bool read_binary_edge_chunk(     std::istream      &_is, 
			           BaseImporter      &_bi, 
			           Options           &_opt,
				   bool              _swap);

  bool read_binary_halfedge_chunk( std::istream      &_is, 
				   BaseImporter      &_bi, 
				   Options           &_opt,
				   bool              _swap);

  bool read_binary_mesh_chunk(     std::istream      &_is, 
				   BaseImporter      &_bi, 
				   Options           &_opt,
				   bool              _swap);

  size_t restore_binary_custom_data( std::istream& _is, 
				     BaseProperty* _bp,
				     size_t _n_elem, 
				     bool _swap);

};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the OM reader.
extern _OMReader_  __OMReaderInstance;
_OMReader_&  OMReader();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
