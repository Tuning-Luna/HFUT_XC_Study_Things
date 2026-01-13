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
//   $Revision: 1.2 $
//   $Date: 2005/04/18 09:07:11 $
//
//=============================================================================

/** \file ModProgMeshT.hh

 */

//=============================================================================
//
//  CLASS ModProgMeshT
//
//=============================================================================

#ifndef OPENMESH_TOOLS_MODPROGMESHT_HH
#define OPENMESH_TOOLS_MODPROGMESHT_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModBaseT.hh>
#include <OpenMesh/Core/Utils/Property.hh>


//== NAMESPACE ================================================================

namespace OpenMesh  {
namespace Decimater {


//== CLASS DEFINITION =========================================================


/** Collect progressive mesh information while decimating.
 *
 *  The progressive mesh data is stored in an internal structure, which
 *  can be evaluated after the decimation process and (!) before calling
 *  the garbage collection of the decimated mesh.
 */
template <class DecimaterType>
class ModProgMeshT : public ModBaseT<DecimaterType>
{
public:

  DECIMATING_MODULE( ModProgMeshT, DecimaterType, ProgMesh );

  /** Struct storing progressive mesh information
   *  \see CollapseInfoT, ModProgMeshT
   */
  struct Info
  {
    /// Initializing constructor copies appropriate handles from
    /// collapse information \c _ci.
    Info( const CollapseInfo& _ci )
      : v0(_ci.v0), v1(_ci.v1), vl(_ci.vl),vr(_ci.vr)
    {}

    typename Mesh::VertexHandle v0; ///< See CollapseInfoT::v0
    typename Mesh::VertexHandle v1; ///< See CollapseInfoT::v1
    typename Mesh::VertexHandle vl; ///< See CollapseInfoT::vl
    typename Mesh::VertexHandle vr; ///< See CollapseInfoT::vr

  };

  /// Type of the list storing the progressive mesh info Info.
  typedef std::vector<Info>           InfoList;


public:

   /// Constructor
  ModProgMeshT( DecimaterType &_dec ) : Base(_dec, true)
  {
    Self::mesh().add_property( idx_ );
  }


  /// Destructor
  ~ModProgMeshT()
  {
    Self::mesh().remove_property( idx_ );
  }

  const InfoList&                            pmi() const
  {
    return pmi_;
  }

public: // inherited


  /// Stores collapse information in a queue.
  /// \see infolist()
  void postprocess_collapse(const CollapseInfo& _ci)
  {
    pmi_.push_back( Info( _ci ) );
  }


  bool is_binary(void) const { return true; }


  // only binary mode possible
  void set_binary(bool _b) 
  {
    if (!_b)
    {
      std::cerr << "ModNormalFlippingT can only be used binary\n";
      exit(1);
    }
  }


public: // specific methods

  /** Write progressive mesh data to a file in proprietary binary format .pm.
   *
   *  The methods uses the collected data to write a progressive mesh
   *  file. It's a binary format with little endian byte ordering:
   *
   *  - The first 8 bytes contain the word "ProgMesh".
   *  - 32-bit int for the number of vertices \c NV in the base mesh.
   *  - 32-bit int for the number of faces in the base mesh.
   *  - 32-bit int for the number of halfedge collapses (now vertex splits)
   *  - Positions of vertices of the base mesh (32-bit float triplets).<br>
   *    \c [x,y,z][x,y,z]...
   *  - Triplets of indices (32-bit int) for each triangle (index in the
   *    list of vertices of the base mesh defined by the positions.<br>
   *    \c [v0,v1,v2][v0,v1,v2]...
   *  - For each collapse/split a detail information package made of
   *    3 32-bit floats for the positions of vertex \c v0, and 3 32-bit
   *    int indices for \c v1, \c vl, and \c vr.
   *    The index for \c vl or \c vr might be -1, if the face on this side
   *    of the edge does not exists.
   *
   *  \remark Write file before calling the garbage collection of the mesh.
   *  \param _ofname Name of the file, where to write the progressive mesh
   *  \return \c true on success of the operation, else \c false.
   */
  bool write( const std::string& _ofname );
  /// Reference to collected information
  const InfoList& infolist() const { return pmi_; }



  InfoList          pmi_;
  VPropHandleT<int> idx_;
};


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_DECIMATER_MODPROGMESH_CC)
#define OSG_MODPROGMESH_TEMPLATES
#include "ModProgMeshT.cc"
#endif
//=============================================================================
#endif // OPENMESH_TOOLS_PROGMESHT_HH defined
//=============================================================================

