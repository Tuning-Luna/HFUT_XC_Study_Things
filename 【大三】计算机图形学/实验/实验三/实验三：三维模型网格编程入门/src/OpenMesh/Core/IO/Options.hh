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


#ifndef OPENMESH_IO_OPTIONS_HH
#define OPENMESH_IO_OPTIONS_HH


//=== INCLUDES ================================================================


// OpenMesh
#include <OpenMesh/Core/System/config.hh>


//== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO   {


//=== IMPLEMENTATION ==========================================================


/** \name Mesh Reading / Writing
    Option for reader and writer modules.
*/
//@{


//-----------------------------------------------------------------------------

/** \brief Set options for reader/writer modules.
 *
 *  The class is used in a twofold way. 
 *  -# In combination with reader modules the class is used 
 *     - to pass hints to the reading module, whether the input is
 *     binary and what byte ordering the binary data has
 *     - to retrieve information about the file contents after
 *     succesful reading.
 *  -# In combination with write modules the class gives directions to
 *     the writer module, whether to 
 *     - use binary mode or not and what byte order to use
 *     - store one of the standard properties.
 *
 *  The option are defined in \c Options::Flag as bit values and stored in 
 *  an \c int value as a bitset.
 */
class Options
{
public:
  typedef int       enum_type;
  typedef enum_type value_type;
   
  /// Definitions of %Options for reading and writing. The options can be
  /// or'ed.
  enum Flag {
    Default        = 0x0000, ///< No options
    Binary         = 0x0001, ///< Set binary mode for r/w
    MSB            = 0x0002, ///< Assume big endian byte ordering
    LSB            = 0x0004, ///< Assume little endian byte ordering
    Swap           = 0x0006, ///< Swap byte order in binary mode
    VertexNormal   = 0x0010, ///< Has (r) / store (w) vertex normals
    VertexColor    = 0x0020, ///< Has (r) / store (w) vertex colors
    VertexTexCoord = 0x0040, ///< Has (r) / store (w) texture coordinates
    FaceNormal     = 0x0100, ///< Has (r) / store (w) face normals
    FaceColor      = 0x0200  ///< Has (r) / store (w) face colors
  };


public:

  /// Default constructor
  Options() : flags_( Default )
  { }


  /// Copy constructor
  Options(const Options& _opt) : flags_(_opt.flags_)
  { }
   

  /// Initializing constructor setting a single option
  Options(Flag _flg) : flags_( _flg)
  { }

   
  /// Initializing constructor setting multiple options
  Options(const value_type _flgs) : flags_( _flgs)
  { }

   
  ~Options()
  { }
   
  /// Restore state after default constructor.
  void cleanup(void)
  { flags_ = Default; }

  /// Clear all bits.
  void clear(void)
  { flags_ = 0; }

  /// Returns true if all bits are zero.
  bool is_empty(void) const { return !flags_; }

public:
   

  //@{
  /// Copy options defined in _rhs.

  Options& operator = ( const Options& _rhs )
  { flags_ = _rhs.flags_; return *this; }

  Options& operator = ( const value_type _rhs )
  { flags_ = _rhs; return *this; }

  //@}

  
  //@{
  /// Unset options defined in _rhs.

  Options& operator -= ( const value_type _rhs )
  { flags_ &= ~_rhs; return *this; }

  Options& unset( const value_type _rhs)
  { return (*this -= _rhs); }   

  //@}

   

  //@{
  /// Set options defined in _rhs
   
  Options& operator += ( const value_type _rhs )
  { flags_ |= _rhs; return *this; }

  Options& set( const value_type _rhs)
  { return (*this += _rhs); }

  //@}

public:


  // Check if an option or several options are set.
  bool check(const value_type _rhs) const
  {
    return (flags_ & _rhs)==_rhs;
  }
   
  bool vertex_has_normal()   const { return check(VertexNormal); }
  bool vertex_has_color()    const { return check(VertexColor); }
  bool vertex_has_texcoord() const { return check(VertexTexCoord); }
  bool face_has_normal()     const { return check(FaceNormal); }
  bool face_has_color()      const { return check(FaceColor); }


  /// Returns true if _rhs has the same options enabled.
  bool operator == (const value_type _rhs) const
  { return flags_ == _rhs; }
   

  /// Returns true if _rhs does not have the same options enabled.
  bool operator != (const value_type _rhs) const
  { return flags_ != _rhs; }
   

  /// Returns the option set.
  operator value_type ()     const { return flags_; }
   
private:
   
  bool operator && (const value_type _rhs) const;
   
  value_type flags_;
};

//-----------------------------------------------------------------------------




//@}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
