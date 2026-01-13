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

#ifndef OPENMESH_SR_BINARY_SPEC_HH
#define OPENMESH_SR_BINARY_SPEC_HH

//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
// -------------------- STL
#include <iterator>
#include <string>
#if defined(OM_CC_GCC) && (OM_CC_VERSION < 30000)
#  include <OpenMesh/Tools/Utils/NumLimitsT.hh>
#else
#  include <limits>
#endif
#include <vector>
#include <stdexcept> // logic_error
#include <numeric>   // accumulate
// -------------------- OpenMesh
#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Attributes/Status.hh>
#include <OpenMesh/Core/IO/SR_types.hh>
#include <OpenMesh/Core/IO/SR_rbo.hh>
#include <OpenMesh/Core/IO/SR_binary.hh>

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace IO {


//=============================================================================

#ifndef DOXY_IGNORE_THIS

//-----------------------------------------------------------------------------
// struct binary, helper for storing/restoring

#define SIMPLE_BINARY( T ) \
  template <> struct binary< T > {                           \
    typedef T value_type;                                    \
    static const bool is_streamable = true;                  \
    static size_t size_of(const value_type&) { return sizeof(value_type); }   \
    static size_t size_of(void) { return sizeof(value_type); }   \
    static size_t store( std::ostream& _os, const value_type& _val, \
		         bool _swap=false) {                 \
      value_type tmp = _val;                                 \
      if (_swap) reverse_byte_order(tmp);                    \
      _os.write( (const char*)&tmp, sizeof(value_type) );    \
      return _os.good() ? sizeof(value_type) : 0;            \
    }                                                        \
                                                             \
    static size_t restore( std::istream& _is, value_type& _val, \
			 bool _swap=false) {                 \
      _is.read( (char*)&_val, sizeof(value_type) );          \
      if (_swap) reverse_byte_order(_val);                   \
      return _is.good() ? sizeof(value_type) : 0;            \
    }                                                        \
  }

SIMPLE_BINARY(bool);
SIMPLE_BINARY(int);
SIMPLE_BINARY(unsigned int);
SIMPLE_BINARY(float);
SIMPLE_BINARY(double);
SIMPLE_BINARY(long double);

SIMPLE_BINARY(int8_t);
SIMPLE_BINARY(int16_t);
SIMPLE_BINARY(int32_t);
SIMPLE_BINARY(int64_t);
SIMPLE_BINARY(uint8_t);
SIMPLE_BINARY(uint16_t);
SIMPLE_BINARY(uint32_t);
SIMPLE_BINARY(uint64_t);

#undef SIMPLE_BINARY


#define VECTORT_BINARY( T ) \
  template <> struct binary< T > {                              \
    typedef T value_type;                                       \
    static const bool is_streamable = true;                     \
    static size_t size_of(void) { return sizeof(value_type); }  \
    static size_t size_of(const value_type&) { return size_of(); } \
    static size_t store( std::ostream& _os, const value_type& _val, \
		         bool _swap=false) {                    \
      value_type tmp = _val;                                    \
      size_t i, b = size_of(_val), N = value_type::size_;       \
      if (_swap) for (i=0; i<N; ++i)                            \
	reverse_byte_order( tmp[i] );                           \
      _os.write( (const char*)&tmp[0], b );                     \
      return _os.good() ? b : 0;                                \
    }                                                           \
                                                                \
    static size_t restore( std::istream& _is, value_type& _val, \
			 bool _swap=false) {                    \
      size_t i, N=value_type::size_;                            \
      size_t b = N * sizeof(value_type::value_type);            \
      _is.read( (char*)&_val[0], b );                           \
      if (_swap) for (i=0; i<N; ++i)                            \
        reverse_byte_order( _val[i] );                          \
      return _is.good() ? b : 0;                                \
    }                                                           \
  }

#define VECTORTS_BINARY( N ) \
   VECTORT_BINARY( Vec##N##c  ); \
   VECTORT_BINARY( Vec##N##uc ); \
   VECTORT_BINARY( Vec##N##s  ); \
   VECTORT_BINARY( Vec##N##us ); \
   VECTORT_BINARY( Vec##N##i  ); \
   VECTORT_BINARY( Vec##N##ui ); \
   VECTORT_BINARY( Vec##N##f  ); \
   VECTORT_BINARY( Vec##N##d  );

VECTORTS_BINARY( 1 );
VECTORTS_BINARY( 2 );
VECTORTS_BINARY( 3 );
VECTORTS_BINARY( 4 );
VECTORTS_BINARY( 6 );

#undef VECTORTS_BINARY
#undef VECTORT_BINARY

template <> struct binary< std::string > {                           
  typedef std::string value_type;
  typedef uint16_t    length_t;

  static const bool is_streamable = true;

  static size_t size_of() { return UnknownSize; }
  static size_t size_of(const value_type &_v) 
  { return sizeof(length_t) + _v.size(); }

  static 
  size_t store(std::ostream& _os, const value_type& _v, bool _swap=false)
  {
#if defined(OM_CC_GCC) && (OM_CC_VERSION < 30000)
    if (_v.size() < Utils::NumLimitsT<length_t>::max() )    
#else
    if (_v.size() < std::numeric_limits<length_t>::max() )
#endif
    {
      length_t len = _v.size();

      if (_swap) reverse_byte_order(len);

      size_t bytes = binary<length_t>::store( _os, len, _swap );
      _os.write( _v.data(), len );
      return _os.good() ? len+bytes : 0;
    }
    throw std::runtime_error("Cannot store string longer than 64Kb");
  }                                                        
                                                             
  static 
  size_t restore(std::istream& _is, value_type& _val, bool _swap=false) 
  {
    length_t len;
    size_t   bytes = binary<length_t>::restore( _is, len, _swap );
    if (_swap)
      reverse_byte_order(len);    
    _val.resize(len);
    _is.read( const_cast<char*>(_val.data()), len );          

    return _is.good() ? (len+bytes) : 0;            
  }                                                        
};


template <> struct binary<OpenMesh::Attributes::StatusInfo>
{
  typedef OpenMesh::Attributes::StatusInfo value_type;
  typedef value_type::value_type           status_t;

  static const bool is_streamable = true;

  static size_t size_of() { return sizeof(status_t); }
  static size_t size_of(const value_type&) { return size_of(); }

  static size_t n_bytes(size_t _n_elem) 
  { return _n_elem*sizeof(status_t); }

  static 
  size_t store(std::ostream& _os, const value_type& _v, bool _swap=false)
  { 
    status_t v=_v.bits(); 
    return binary<status_t>::store(_os, v, _swap); 
  }

  static 
  size_t restore( std::istream& _os, value_type& _v, bool _swap=false)
  { 
    status_t v;
    size_t   b = binary<status_t>::restore(_os, v, _swap);
    _v.set_bits(v);
    return b;
  }
};


//-----------------------------------------------------------------------------
// std::vector<T> specializations for struct binary<>

template <typename T>
struct FunctorStore {
  FunctorStore( std::ostream& _os, bool _swap) : os_(_os), swap_(_swap) { }
  size_t operator () ( size_t _v1, const T& _s2 )
  { return _v1+binary<T>::store(os_, _s2, swap_ ); }

  std::ostream& os_;
  bool          swap_;
};


template <typename T>
struct FunctorRestore {
  FunctorRestore( std::istream& _is, bool _swap) : is_(_is), swap_(_swap) { }
  size_t operator () ( size_t _v1, T& _s2 )
  { return _v1+binary<T>::restore(is_, _s2, swap_ ); }
  std::istream& is_;
  bool          swap_;
};

#include <OpenMesh/Core/IO/SR_binary_vector_of_fundamentals.inl>
#include <OpenMesh/Core/IO/SR_binary_vector_of_string.inl>
#include <OpenMesh/Core/IO/SR_binary_vector_of_bool.inl>

// ----------------------------------------------------------------------------

#endif // DOXY_IGNORE_THIS

//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_SR_BINARY_SPEC_HH defined
//=============================================================================

