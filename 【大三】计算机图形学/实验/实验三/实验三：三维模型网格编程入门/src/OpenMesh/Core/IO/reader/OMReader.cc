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


//== INCLUDES =================================================================


//STL
#include <fstream>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/Utils/Endian.hh>
#include <OpenMesh/Core/IO/OMFormat.hh>
#include <OpenMesh/Core/IO/reader/OMReader.hh>


//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


// register the OMReader singleton with MeshReader
_OMReader_  __OMReaderInstance;
_OMReader_&  OMReader() { return __OMReaderInstance; }



//=== IMPLEMENTATION ==========================================================


_OMReader_::_OMReader_()
{
  IOManager().register_module(this); 
}


//-----------------------------------------------------------------------------


bool 
_OMReader_::read(const std::string& _filename, 
		 BaseImporter& _bi,
		 Options& _opt)
{
  // check whether importer can give us an OpenMesh BaseKernel
  if (!_bi.kernel()) return false;

  _opt += Options::Binary; // only binary format supported!

  // clear vhandles
  vhandles_.clear();

  // Open file
  std::ifstream ifs( _filename.c_str(), std::ios::binary );
  if (!ifs.is_open())
  {
    omerr() << "[OMReader] : cannot not open file " 
	  << _filename
	  << std::endl;
    return false;
  }
  
  // Pass stream to read method, remember result
  bool result = read(ifs, _bi, _opt);
  
  // clean up vhandles
  BaseImporter::VHandles().swap(vhandles_);

  // close input stream
  ifs.close();
  
  return result;
}


//-----------------------------------------------------------------------------


bool 
_OMReader_::read(std::istream& _is, BaseImporter& _bi, Options& _opt )
{
  // currently only binary file format is supported
  _opt += Options::Binary;

  return read_binary( _is, _bi, _opt );
}



//-----------------------------------------------------------------------------

bool 
_OMReader_::read_ascii( std::istream& _is, BaseImporter& _bi, 
			Options& _opt)
{
  // not supported yet!
  return false;
}


//-----------------------------------------------------------------------------

bool 
_OMReader_::read_binary( std::istream& _is, BaseImporter& _bi, 
			 Options& _opt)
{
  bool swap = _opt.check(Options::Swap) || (Endian::local() == Endian::MSB);

  // intialize byte counter
  bytes_ = 0;  

  bytes_ += restore( _is, header_, swap );

  size_t data_bytes;

  while( !_is.eof() )
  {
    bytes_ += restore( _is, chunk_header_, swap );

    if ( _is.eof() )
      break;

    // Is this a named property restore the name
    if ( chunk_header_.name_ )
    {
      OMFormat::Chunk::PropertyName pn;
      bytes_ += restore( _is, property_name_, swap );
    }

    // Read in the property data. If it is an anonymous or unknown named
    // property, then skip data.
    data_bytes = bytes_;
    switch( chunk_header_.entity_ )
    {
      case OMFormat::Chunk::Entity_Vertex:	
	if (!read_binary_vertex_chunk( _is, _bi, _opt, swap ))
	  return false;
	break;
      case OMFormat::Chunk::Entity_Face:
	if (!read_binary_face_chunk( _is, _bi, _opt, swap ))
	  return false;
	break;
      case OMFormat::Chunk::Entity_Edge:
	if (!read_binary_edge_chunk( _is, _bi, _opt, swap ))
	  return false;
	break;
      case OMFormat::Chunk::Entity_Halfedge:
	if (!read_binary_halfedge_chunk( _is, _bi, _opt, swap ))
	  return false;
	break;
      case OMFormat::Chunk::Entity_Mesh:
	if (!read_binary_mesh_chunk( _is, _bi, _opt, swap ))
	  return false;
	break;
      default:
	return false;
    }
    data_bytes = bytes_ - data_bytes;
  }

  // File was successfully parsed.
  return true;
}


//-----------------------------------------------------------------------------

bool
_OMReader_::can_u_read(const std::string& _filename) const
{
  // !!! Assuming BaseReader::can_u_parse( std::string& )
  // does not call BaseReader::read_magic()!!!
  if ( this->BaseReader::can_u_read( _filename ) )
  {
    std::ifstream ifile( _filename.c_str() );
    if ( ifile  && can_u_read( ifile ) )
      return true;      
  }
  return false;
}

//-----------------------------------------------------------------------------
  
bool
_OMReader_::can_u_read(std::istream& _is) const
{
  std::vector<char> evt;
  evt.reserve(20);

  // read first 4 characters into a buffer
  while( evt.size() < 4 )
    evt.push_back( static_cast<char>(_is.get()) );

  // put back all read characters   
  std::vector<char>::reverse_iterator it = evt.rbegin();
  while (it != evt.rend() )
    _is.putback( *it++ );


  // evaluate header information
  OMFormat::Header *hdr = (OMFormat::Header*)&evt[0];

  // first two characters must be 'OM'
  if (hdr->magic_[0] != 'O' || hdr->magic_[1] != 'M')
    return false;

  // 3rd characters defines the mesh type:
  switch(hdr->mesh_)
  {
    case 'T': // Triangle Mesh
    case 'Q': // Quad Mesh
    case 'P': // Polygonal Mesh
      break;
    default:  // ?
      return false;
  }
  
  // 4th characters encodes the version
  return supports( hdr->version_ );
}

//-----------------------------------------------------------------------------

bool 
_OMReader_::supports( const OMFormat::uint8 version ) const
{
  return true;
}


//-----------------------------------------------------------------------------

bool 
_OMReader_::read_binary_vertex_chunk( std::istream      &_is, 
				      BaseImporter      &_bi, 
				      Options           &_opt,
				      bool              _swap)
{
  using OMFormat::Chunk;

  assert( chunk_header_.entity_ == Chunk::Entity_Vertex );
  
  OpenMesh::Vec3f  v3f;
  OpenMesh::Vec2f  v2f;
  OpenMesh::Vec3uc v3uc; // rgb

  OMFormat::Chunk::PropertyName custom_prop;

  size_t vidx=0;
  switch (chunk_header_.type_)
  {
    case Chunk::Type_Pos:
      assert( OMFormat::dimensions(chunk_header_) 
 	      == size_t(OpenMesh::Vec3f::dim()) );

      for (; vidx < header_.n_vertices_ && !_is.eof(); ++vidx)
      {
	bytes_ += vector_restore( _is, v3f, _swap );
	vhandles_.push_back( _bi.add_vertex(v3f) );
      }
      break;

      
    case Chunk::Type_Normal:
      assert( OMFormat::dimensions(chunk_header_) 
	      == size_t(OpenMesh::Vec3f::dim()) );

      _opt += Options::VertexNormal;
      for (; vidx < header_.n_vertices_ && !_is.eof(); ++vidx)
      {
	bytes_ += vector_restore( _is, v3f, _swap );
	_bi.set_normal(VertexHandle(vidx), v3f);
      }
      break;

      
    case Chunk::Type_Texcoord:
      assert( OMFormat::dimensions(chunk_header_) 
	      == size_t(OpenMesh::Vec2f::dim()) );

      _opt += Options::VertexTexCoord;
      for (; vidx < header_.n_vertices_ && !_is.eof(); ++vidx)
      {
	bytes_ += vector_restore( _is, v2f, _swap );
	_bi.set_texcoord(VertexHandle(vidx), v2f);
      }
      break;      

    case Chunk::Type_Color:

      assert( OMFormat::dimensions(chunk_header_) == 3 );

      _opt += Options::VertexColor;
      
      for (; vidx < header_.n_vertices_ && !_is.eof(); ++vidx)
      {
	bytes_ += vector_restore( _is, v3uc, _swap );
	_bi.set_color( VertexHandle(vidx), v3uc );
      }
      break;

    case Chunk::Type_Custom:
      
      bytes_ += 
	restore_binary_custom_data( _is,
				    _bi.kernel()->_get_vprop( property_name_ ),
				    header_.n_vertices_,
				    _swap );

      vidx = header_.n_vertices_;

      break;

    default: // skip unknown chunks
    {
      omerr() << "Unknown chunk type ignored!\n";
      size_t size_of = header_.n_vertices_ 
	* OMFormat::vector_size(chunk_header_);
      _is.ignore( size_of );
      bytes_ += size_of;
    }
  }

  // all chunk data has been read..?!
  return vidx == header_.n_vertices_;
}


//-----------------------------------------------------------------------------

bool
_OMReader_::read_binary_face_chunk( std::istream &_is, 
				    BaseImporter &_bi, 
				    Options      &_opt,
				    bool         _swap )
{
  using OMFormat::Chunk;

  assert( chunk_header_.entity_ == Chunk::Entity_Face );

  size_t fidx=0;
  OpenMesh::Vec3f  v3f;  // normal
  OpenMesh::Vec3uc v3uc; // rgb

  switch( chunk_header_.type_ )
  {
    case Chunk::Type_Topology:
    {
      BaseImporter::VHandles  face_vhandles;
      size_t                  nV;
      size_t                  vidx;

      switch( header_.mesh_ )
      {
	case 'T': nV = 3; break;
	case 'Q': nV = 4; break;
      }
      
      for (; fidx < header_.n_faces_; ++fidx)
      {
 	if ( header_.mesh_ == 'P' )
 	  bytes_ += restore( _is, nV, Chunk::Integer_16, _swap );

	face_vhandles.clear();
	for (size_t j=0; j<nV; ++j)
	{	  
	  bytes_ += restore( _is, vidx, 
			     Chunk::Integer_Size(chunk_header_.bits_),
			     _swap );

	  face_vhandles.push_back( vhandles_[ vidx ] );
	}

	_bi.add_face(face_vhandles);
      }
    }
    break;


    case Chunk::Type_Normal:
      assert( OMFormat::dimensions(chunk_header_) 
	      == size_t(OpenMesh::Vec3f::dim()) );

      _opt += Options::FaceNormal;
      for (; fidx < header_.n_faces_ && !_is.eof(); ++fidx)
      {
	bytes_ += vector_restore( _is, v3f, _swap );
	_bi.set_normal(FaceHandle(fidx), v3f);
      }
      break;


    case Chunk::Type_Color:

      assert( OMFormat::dimensions(chunk_header_) == 3 );

      _opt += Options::FaceColor;      
      for (; fidx < header_.n_faces_ && !_is.eof(); ++fidx)
      {
	bytes_ += vector_restore( _is, v3uc, _swap );
	_bi.set_color( FaceHandle(fidx), v3uc );
      }
      break;


    case Chunk::Type_Custom:
      
      bytes_ += 
	restore_binary_custom_data( _is,
				    _bi.kernel()->_get_fprop( property_name_ ),
				    header_.n_faces_,
				    _swap );

      fidx = header_.n_faces_;

      break;
    
    default: // skip unknown chunks
    {
      omerr() << "Unknown chunk type ignore!\n";
      size_t size_of = OMFormat::chunk_data_size(header_, chunk_header_);
      _is.ignore( size_of );
      bytes_ += size_of;      
    }
  }
  return fidx == header_.n_faces_;
}


//-----------------------------------------------------------------------------

bool
_OMReader_::read_binary_edge_chunk( std::istream &_is, 
				    BaseImporter &_bi, 
				    Options      &_opt,
				    bool         _swap )
{
  using OMFormat::Chunk;

  assert( chunk_header_.entity_ == Chunk::Entity_Edge );

  size_t b=bytes_;

  switch( chunk_header_.type_ )
  {
    case Chunk::Type_Custom:      

      bytes_ += 
	restore_binary_custom_data( _is,
				    _bi.kernel()->_get_eprop( property_name_ ),
				    header_.n_edges_,
				    _swap );

      break;

    default:
      // skip unknown type
      size_t size_of = OMFormat::chunk_data_size(header_, chunk_header_);
      _is.ignore( size_of );
      bytes_ += size_of;
  }

  return b < bytes_;
}


//-----------------------------------------------------------------------------

bool
_OMReader_::read_binary_halfedge_chunk( std::istream &_is, 
					BaseImporter &_bi, 
					Options      &_opt,
					bool         _swap )
{
  using OMFormat::Chunk;

  assert( chunk_header_.entity_ == Chunk::Entity_Halfedge );

  size_t b = bytes_;

  switch( chunk_header_.type_ )
  {
    case Chunk::Type_Custom:

      bytes_ += 
	restore_binary_custom_data( _is,
				    _bi.kernel()->_get_hprop( property_name_ ),
				    2*header_.n_edges_,
				    _swap );      
      break;

    default:
      // skip unknown chunk
      omerr() << "Unknown chunk type ignored!\n";
      size_t size_of = OMFormat::chunk_data_size(header_, chunk_header_);
      _is.ignore( size_of );
      bytes_ += size_of;
  }

  return b < bytes_;
}


//-----------------------------------------------------------------------------

bool
_OMReader_::read_binary_mesh_chunk( std::istream &_is, 
				    BaseImporter &_bi, 
				    Options      &_opt,
				    bool         _swap )
{
  using OMFormat::Chunk;

  assert( chunk_header_.entity_ == Chunk::Entity_Mesh );

  size_t b = bytes_;

  switch( chunk_header_.type_ )
  {
    case Chunk::Type_Custom:
      
      bytes_ += 
	restore_binary_custom_data( _is,
				    _bi.kernel()->_get_mprop( property_name_ ),
				    1,
				    _swap );
      
      break;

    default:
      // skip unknown chunk
      size_t size_of = OMFormat::chunk_data_size(header_, chunk_header_);
      _is.ignore( size_of );
      bytes_ += size_of;
  }

  return b < bytes_;
}


//-----------------------------------------------------------------------------


size_t 
_OMReader_::restore_binary_custom_data( std::istream& _is, BaseProperty* _bp,
					 size_t _n_elem, bool _swap)
{  
  assert( !_bp || (_bp->name() == property_name_) );

  using OMFormat::Chunk;

  size_t              bytes = 0;
  Chunk::esize_t      block_size;
  Chunk::PropertyName custom_prop;

  bytes += binary<Chunk::esize_t>::restore( _is, block_size, _swap );


  if ( _bp )
  {
#if defined(OM_DEBUG)
    size_t b;
#endif
    size_t n_bytes = _bp->size_of( _n_elem );

    if ( ((n_bytes == BaseProperty::UnknownSize) || (n_bytes == block_size)) &&
	 (_bp->element_size() == BaseProperty::UnknownSize ||
	  (_n_elem * _bp->element_size() == block_size) ) )      
    {
#if defined(OM_DEBUG)
      bytes += (b=_bp->restore( _is, _swap ));
      assert( block_size == b );
#else
      bytes += _bp->restore( _is, _swap );
#endif
      assert( block_size == _bp->size_of() );

      block_size = 0;
    }
    else
    {
      omerr() << "Warning! Property " << _bp->name() << " not loaded: " 
	    << "Mismatching data sizes!n";
    }
  }

  if (block_size)
  {
    _is.ignore( block_size );
    bytes  += block_size;
  }

  return bytes;
}


//-----------------------------------------------------------------------------

//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
