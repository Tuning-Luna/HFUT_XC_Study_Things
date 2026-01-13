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
//   $Date: 2004/09/06 12:35:36 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


#ifndef OPENMESH_COLOR_CAST_HH
#define OPENMESH_COLOR_CAST_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {


//=============================================================================


/** \name Cast vector type to another vector type.
*/
//@{

//-----------------------------------------------------------------------------
#ifndef DOXY_IGNORE_THIS

/// Cast one color vector to another.
template <typename dst_t, typename src_t>
struct color_caster
{
  typedef dst_t  return_type;

  inline static return_type cast(const src_t& _src)
  {
    dst_t dst;
    vector_copy(_src, dst, GenProg::Int2Type<vector_traits<dst_t>::size_>());
    return dst;
  }
};


template <>
struct color_caster<Vec3uc,Vec3f>
{
  typedef Vec3uc return_type;

  inline static return_type cast(const Vec3f& _src)
  {
    return Vec3uc( (unsigned char)(_src[0]* 255.0f + 0.5f),
		   (unsigned char)(_src[1]* 255.0f + 0.5f),
		   (unsigned char)(_src[2]* 255.0f + 0.5f) );
  }
};


template <>
struct color_caster<Vec3f, Vec3uc>
{
  typedef Vec3f return_type;

  inline static return_type cast(const Vec3uc& _src)
  {
    const float f = 1.0f / 255.0f;
    return Vec3f(_src[0] * f, _src[1] *  f, _src[2] * f );
  }
};


// ----------------------------------------------------------------------------


#ifndef DOXY_IGNORE_THIS

#if !defined(OM_CC_MSVC)
template <typename dst_t>
struct color_caster<dst_t,dst_t>
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


template <typename dst_t, typename src_t>
inline
typename color_caster<dst_t, src_t>::return_type
color_cast(const src_t& _src )
{
  return color_caster<dst_t, src_t>::cast(_src);
}

#endif
//-----------------------------------------------------------------------------

//@}


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_COLOR_CAST_HH defined
//=============================================================================

