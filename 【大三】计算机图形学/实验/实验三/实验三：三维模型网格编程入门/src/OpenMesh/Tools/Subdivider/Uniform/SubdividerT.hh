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
//   $Date: 2004/09/06 12:37:23 $
//                                                                            
//=============================================================================

/** \file SubdividerT.hh
    
 */

//=============================================================================
//
//  CLASS SubdividerT
//
//=============================================================================

#ifndef OPENMESH_SUBDIVIDER_UNIFORM_SUDIVIDERT_HH
#define OPENMESH_SUBDIVIDER_UNIFORM_SUDIVIDERT_HH

//== INCLUDE ==================================================================

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/Noncopyable.hh>
#if defined(_DEBUG) || defined(DEBUG)
// Makes life lot easier, when playing/messing around with low-level topology
// changing methods of OpenMesh
#  include <OpenMesh/Tools/Utils/MeshCheckerT.hh>
#  define ASSERT_CONSISTENCY( T, m ) \
     assert(OpenMesh::Utils::MeshCheckerT<T>(m).check())
#else
#  define ASSERT_CONSISTENCY( T, m )
#endif

//== NAMESPACE ================================================================

namespace OpenMesh   {
namespace Subdivider {
namespace Uniform    {


//== CLASS DEFINITION =========================================================

/** Abstract base class for uniform subdivision algorithms.
 *
 *  A derived class must overload the following functions:
 *  -# name()
 *  -# prepare()
 *  -# subdivide()
 *  -# cleanup()
 */
template <typename MeshType, typename RealType=float>
class SubdividerT : private Utils::Noncopyable
{
public:

  typedef MeshType mesh_t;
  typedef RealType real_t;

public:


  /** Constructor. Need to call attach()
      \see attach(), operator()(size_t), detach()
  */
  SubdividerT(void) : attached_(NULL) { }


  /// Descructor (calls detach())
  virtual ~SubdividerT() 
  { detach(); }

  /// Return name of subdivision algorithm
  virtual const char *name( void ) const = 0;


public: /// \name Interface 1

  //@{
  /// Subdivide the mesh \c _m \c _n times.
  /// \see SubdividerT(MeshType&)
  bool operator () ( MeshType& _m, size_t _n )
  {    
    return prepare(_m) && subdivide( _m, _n ) && cleanup( _m );
  }
  //@}

public: /// \name Interface 2
  //@{
  /// Attach mesh \c _m to self
  /// \see SubdividerT(), operator()(size_t), detach()
  bool attach( MeshType& _m )
  {
    if ( attached_ == &_m )
      return true;

    detach();
    if (prepare( _m ))
    {
      attached_ = &_m;
      return true;
    }
    return false;
  }

  /// Subdivide the attached \c _n times.
  /// \see SubdividerT(), attach(), detach()
  bool operator()( size_t _n )
  {
    return attached_ ? subdivide( *attached_, _n ) : false;
  }

  /// Detach an eventually attached mesh.
  /// \see SubdividerT(), attach(), operator()(size_t)
  void detach(void)
  {
    if ( attached_ )
    {
      cleanup( *attached_ );
      attached_ = NULL;
    }
  }
  //@}

protected: 

  /// \name Overload theses methods
  //@{
  /// Prepare mesh, e.g. add properties
  virtual bool prepare( MeshType& _m ) = 0;

  /// Subdivide mesh \c _m \c _n times
  virtual bool subdivide( MeshType& _m, size_t _n ) = 0;

  /// Cleanup mesh after usage, e.g. remove added properties
  virtual bool cleanup( MeshType& _m ) = 0;
  //@}

private:
 
  MeshType *attached_;

};

//=============================================================================
} // namespace Uniform
} // namespace Subdivider
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_SUBDIVIDER_UNIFORM_SUBDIVIDERT_HH
//=============================================================================
