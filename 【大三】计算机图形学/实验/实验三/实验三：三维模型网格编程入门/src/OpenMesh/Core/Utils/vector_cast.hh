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
//   $Date: 2004/09/06 12:35:37 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


#ifndef OPENMESH_VECTORCAST_HH
#define OPENMESH_VECTORCAST_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/vector_traits.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <iostream>
#include <algorithm>
#include <OpenMesh/Core/Math/VectorT.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {


//=============================================================================


/** \name Cast vector type to another vector type.
*/
//@{

//-----------------------------------------------------------------------------


template <typename src_t, typename dst_t>
inline void vector_copy( const src_t &_src, dst_t &_dst, GenProg::Int2Type<1> )
{
  _dst[0] = _src[0];
}

template <typename src_t, typename dst_t>
inline void vector_copy( const src_t &_src, dst_t &_dst, GenProg::Int2Type<2> )
{
  _dst[0] = _src[0];
  _dst[1] = _src[1];
}

template <typename src_t, typename dst_t>
inline void vector_copy( const src_t &_src, dst_t &_dst, GenProg::Int2Type<3> )
{
  _dst[0] = _src[0];
  _dst[1] = _src[1];
  _dst[2] = _src[2];
}

template <typename src_t, typename dst_t>
inline void vector_copy( const src_t &_src, dst_t &_dst, GenProg::Int2Type<4> )
{
  _dst[0] = _src[0];
  _dst[1] = _src[1];
  _dst[2] = _src[2];
  _dst[3] = _src[3];
}

template <typename src_t, typename dst_t>
inline void vector_copy( const src_t &_src, dst_t &_dst, GenProg::Int2Type<5> )
{
  _dst[0] = _src[0];
  _dst[1] = _src[1];
  _dst[2] = _src[2];
  _dst[3] = _src[3];
  _dst[4] = _src[4];
}

template <typename src_t, typename dst_t>
inline void vector_copy( const src_t &_src, dst_t &_dst, GenProg::Int2Type<6> )
{
  _dst[0] = _src[0];
  _dst[1] = _src[1];
  _dst[2] = _src[2];
  _dst[3] = _src[3];
  _dst[4] = _src[4];
  _dst[5] = _src[5];
}


//-----------------------------------------------------------------------------
#ifndef DOXY_IGNORE_THIS

template <typename dst_t, typename src_t>
struct vector_caster
{
  typedef dst_t  return_type;

  inline static return_type cast(const src_t& _src)
  {
    dst_t dst;
    vector_copy(_src, dst, GenProg::Int2Type<vector_traits<dst_t>::size_>());
    return dst;
  }
};

#if !defined(OM_CC_MSVC)
template <typename dst_t>
struct vector_caster<dst_t,dst_t>
{
  typedef const dst_t&  return_type;

  inline static return_type cast(const dst_t& _src)
  {
    return _src;
  }
};
#endif

#endif
//-----------------------------------------------------------------------------


/// Cast vector type to another vector type by copying the vector elements
template <typename dst_t, typename src_t>
inline
typename vector_caster<dst_t, src_t>::return_type
vector_cast(const src_t& _src )
{
  return vector_caster<dst_t, src_t>::cast(_src);
}


//@}


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_MESHREADER_HH defined
//=============================================================================
