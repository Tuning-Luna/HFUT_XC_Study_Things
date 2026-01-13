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
//   $Date: 2004/09/06 12:35:06 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


#define OPENMESH_IO_OMFORMAT_CC


//== INCLUDES =================================================================

#include <OpenMesh/Core/IO/OMFormat.hh>
#include <algorithm>
#include <iomanip>

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace IO {

#if !defined(OPENMESH_IO_OMFORMAT_TEMPLATES)

namespace OMFormat {

//== IMPLEMENTATION ===========================================================

  Chunk::Integer_Size needed_bits( size_t s )
  {
    if (s <= 0x000100)   return Chunk::Integer_8;
    if (s <= 0x010000) return Chunk::Integer_16;

#if 0
    // !Not tested yet! This most probably won't work!
    // NEED a 64bit system!
    if ( (sizeof( size_t ) == 8) && (s >= 0x100000000) )
	return Chunk::Integer_64;
#endif

    return Chunk::Integer_32;    
  }

//-----------------------------------------------------------------------------

  uint16& 
  operator << (uint16& val, const Chunk::Header& hdr)
  {
    val = 0;
    val |= hdr.name_   << OMFormat::Chunk::OFF_NAME;
    val |= hdr.entity_ << OMFormat::Chunk::OFF_ENTITY;
    val |= hdr.type_   << OMFormat::Chunk::OFF_TYPE;  
    val |= hdr.signed_ << OMFormat::Chunk::OFF_SIGNED;
    val |= hdr.float_  << OMFormat::Chunk::OFF_FLOAT; 
    val |= hdr.dim_    << OMFormat::Chunk::OFF_DIM;   
    val |= hdr.bits_   << OMFormat::Chunk::OFF_BITS;  
    return val;
  }


//-----------------------------------------------------------------------------

  Chunk::Header&
  operator << (Chunk::Header& hdr, const uint16 val)
  {
    hdr.reserved_ = 0;
    hdr.name_     = val >> OMFormat::Chunk::OFF_NAME;
    hdr.entity_   = val >> OMFormat::Chunk::OFF_ENTITY;
    hdr.type_     = val >> OMFormat::Chunk::OFF_TYPE;
    hdr.signed_   = val >> OMFormat::Chunk::OFF_SIGNED;
    hdr.float_    = val >> OMFormat::Chunk::OFF_FLOAT;
    hdr.dim_      = val >> OMFormat::Chunk::OFF_DIM;
    hdr.bits_     = val >> OMFormat::Chunk::OFF_BITS;
    return hdr;
  }

//-----------------------------------------------------------------------------

  const char *as_string(Chunk::Entity e)
  {
    switch(e)
    {    
      case Chunk::Entity_Vertex:   return "Vertex";
      case Chunk::Entity_Mesh:     return "Mesh";
      case Chunk::Entity_Edge:     return "Edge";
      case Chunk::Entity_Halfedge: return "Halfedge";
      case Chunk::Entity_Face:     return "Face";
      default:
	std::clog << "as_string(Chunk::Entity): Invalid value!";
    }
    return NULL;
  }


//-----------------------------------------------------------------------------

  const char *as_string(Chunk::Type t)
  {
    switch(t)
    {    
      case Chunk::Type_Pos:      return "Pos";
      case Chunk::Type_Normal:   return "Normal";
      case Chunk::Type_Texcoord: return "Texcoord";
      case Chunk::Type_Status:   return "Status";
      case Chunk::Type_Color:    return "Color";
      case Chunk::Type_Custom:   return "Custom";
      case Chunk::Type_Topology: return "Topology";
    }
    return NULL;
  }


//-----------------------------------------------------------------------------

  const char *as_string(Chunk::Dim d)
  {
    switch(d)
    {    
      case Chunk::Dim_1D: return "1D";
      case Chunk::Dim_2D: return "2D";
      case Chunk::Dim_3D: return "3D";
      case Chunk::Dim_4D: return "4D";
      case Chunk::Dim_5D: return "5D";
      case Chunk::Dim_6D: return "6D";
      case Chunk::Dim_7D: return "7D";
      case Chunk::Dim_8D: return "8D";
    }
    return NULL;
  }


//-----------------------------------------------------------------------------

  const char *as_string(Chunk::Integer_Size d)
  {
    switch(d)
    {    
      case Chunk::Integer_8  : return "8";
      case Chunk::Integer_16 : return "16";
      case Chunk::Integer_32 : return "32";
      case Chunk::Integer_64 : return "64";
    }
    return NULL;
  }

  const char *as_string(Chunk::Float_Size d)
  {
    switch(d)
    {    
      case Chunk::Float_32 : return "32";
      case Chunk::Float_64 : return "64";
      case Chunk::Float_128: return "128";
    }
    return NULL;
  }


//-----------------------------------------------------------------------------

  std::ostream& operator << ( std::ostream& _os, const Chunk::Header& _c )
  {
    _os << "Chunk Header : 0x" << std::setw(4)
	<< std::hex << (*(uint16*)(&_c)) << std::dec << std::endl;
    _os << "entity = " 
	<< as_string(Chunk::Entity(_c.entity_)) << std::endl;
    _os << "type   = " 
	<< as_string(Chunk::Type(_c.type_));
    if ( Chunk::Type(_c.type_)!=Chunk::Type_Custom) 
    {
      _os << std::endl 
	  << "signed = " 
	  << _c.signed_ << std::endl;
      _os << "float  = " 
	  << _c.float_ << std::endl;
      _os << "dim    = " 
	  << as_string(Chunk::Dim(_c.dim_)) << std::endl;
      _os << "bits   = " 
	  << (_c.float_
	      ? as_string(Chunk::Float_Size(_c.bits_)) 
	      : as_string(Chunk::Integer_Size(_c.bits_)));
    }
    return _os;
  }


//-----------------------------------------------------------------------------

  std::ostream& operator << ( std::ostream& _os, const Header& _h )
  {
    _os << "magic   = '" << _h.magic_[0] << _h.magic_[1] << "'\n"
	<< "mesh    = '" << _h.mesh_ << "'\n"
	<< "version = 0x" << std::hex << (uint16)_h.version_ << std::dec
	<< " (" << major_version(_h.version_) 
	<< "."  << minor_version(_h.version_) << ")\n"
	<< "#V      = " << _h.n_vertices_ << std::endl
	<< "#F      = " << _h.n_faces_ << std::endl
	<< "#E      = " << _h.n_edges_;
    return _os;
  }


} // namespace OMFormat
  // --------------------------------------------------------------------------

#endif // template stuff


  // helper to store a an integer
  template< typename T > 
  size_t 
  store( std::ostream& _os, 
	 const T& _val, 
	 OMFormat::Chunk::Integer_Size _b, 
	 bool _swap,
	 t_signed)
  {    
    assert( OMFormat::is_integer( _val ) );

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 	
	OMFormat::int8 v = static_cast<OMFormat::int8>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::int16 v = static_cast<OMFormat::int16>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::int32 v = static_cast<OMFormat::int32>(_val);
	return store( _os, v, _swap );
      }      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::int64 v = static_cast<OMFormat::int64>(_val);
	return store( _os, v, _swap );
      }
    }
    return 0;
  }


  // helper to store a an unsigned integer
  template< typename T > 
  size_t 
  store( std::ostream& _os, 
	 const T& _val, 
	 OMFormat::Chunk::Integer_Size _b, 
	 bool _swap,
	 t_unsigned)
  {    
    assert( OMFormat::is_integer( _val ) );

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 
	OMFormat::uint8 v = static_cast<OMFormat::uint8>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::uint16 v = static_cast<OMFormat::uint16>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::uint32 v = static_cast<OMFormat::uint32>(_val);
	return store( _os, v, _swap );
      }
      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::uint64 v = static_cast<OMFormat::uint64>(_val);
	return store( _os, v, _swap );
      }
    }
    return 0;
  }


  // helper to store a an integer
  template< typename T > 
  size_t 
  restore( std::istream& _is, 
	   T& _val, 
	   OMFormat::Chunk::Integer_Size _b, 
	   bool _swap,
	   t_signed)
  {    
    assert( OMFormat::is_integer( _val ) );
    size_t bytes = 0;

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 	
	OMFormat::int8 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::int16 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::int32 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
      }      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::int64 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
      }
    }
    return bytes;
  }


  // helper to store a an unsigned integer
  template< typename T > 
  size_t 
  restore( std::istream& _is, 
	   T& _val, 
	   OMFormat::Chunk::Integer_Size _b, 
	   bool _swap,
	   t_unsigned)
  {    
    assert( OMFormat::is_integer( _val ) );
    size_t bytes = 0;

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 
	OMFormat::uint8 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::uint16 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::uint32 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::uint64 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
    }
    return bytes;
  }

//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
