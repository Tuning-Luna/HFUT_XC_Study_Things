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
//   $Revision: 1.4 $
//   $Date: 2005/04/19 08:01:10 $
//
//=============================================================================

/** \file DecimaterT.hh
 */

//=============================================================================
//
//  CLASS DecimaterT
//
//=============================================================================

#ifndef OPENMESH_DECIMATER_DECIMATERT_HH
#define OPENMESH_DECIMATER_DECIMATERT_HH


//== INCLUDES =================================================================

#include <memory>

#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Tools/Utils/HeapT.hh>
#include <OpenMesh/Tools/Decimater/ModBaseT.hh>



//== NAMESPACE ================================================================

namespace OpenMesh  {
namespace Decimater {


//== CLASS DEFINITION =========================================================


/** Decimater framework.
    \see BaseModT, \ref decimater_docu
*/
template < typename MeshT >
class DecimaterT
{
public: //-------------------------------------------------------- public types

  typedef DecimaterT< MeshT >        Self;
  typedef MeshT                      Mesh;
  typedef CollapseInfoT<MeshT>       CollapseInfo;
  typedef ModBaseT<Self>             Module;
  typedef std::vector< Module* >     ModuleList;



public: //------------------------------------------------------ public methods

  /// Constructor
  DecimaterT( Mesh& _mesh );

  /// Destructor
  ~DecimaterT();


  /** Initialize decimater and decimating modules.

      Return values:
      true   ok
      false  No ore more than one non-binary module exist. In that case
             the decimater is uninitialized!
   */
  bool initialize();


  /// Returns whether decimater has been sucessfully initialized.
  bool is_initialized() const { return initialized_; }


  /** Should the decimation process emit progress messages?
      \sa ProgressManager
   */
  void emit_progress(bool _b) { emit_progress_ = _b; }


  /// Print inforation abount modules to _os
  void info( std::ostream& _os );




public: //--------------------------------------------------- module management


  /// access mesh. used in modules.
  Mesh& mesh() { return Self::mesh_; }


  /// add binary module to decimater
  template < typename ModuleT >
  bool add_binary( ModHandleT<ModuleT>& _mh )
  {
    if (_mh.is_valid()) return false;
    ModuleT* m = new ModuleT(*this);
    bmodules_.push_back(m);
    m->set_binary(true);
    _mh.init(m);
    initialized_ = false;
    return true;
  }


  /// add priority module to decimater
  template < typename ModuleT >
  bool add_priority( ModHandleT<ModuleT>& _mh )
  {
    if (_mh.is_valid()) return false;
    ModuleT* m = new ModuleT(*this);
    cmodule_ = m;
    m->set_binary(false);
    _mh.init(m);
    initialized_ = false;
    return true;
  }


  /// remove module from decimater
  template < typename ModuleT >
  bool remove( ModHandleT<ModuleT>& _mh )
  {
    if (!_mh.is_valid()) return false;

    // binary module?
    if (module(_mh).is_binary())
    {
      typename ModuleList::iterator it = 
	std::find(bmodules_.begin(), bmodules_.end(), _mh.module() );
      if ( it != bmodules_.end() )
	bmodules_.erase( it );
      else
	return false;
    }

    // priority module
    else
    {
      if (&module(_mh) == cmodule_)
	cmodule_ = NULL;
      else
	return false;
    }


    delete &module(_mh);
    _mh.clear();

    initialized_ = false;
    return true;
  }


  /// get module referenced by handle _mh
  template < typename Module >
  Module& module( ModHandleT<Module>& _mh )
  {
    assert( _mh.is_valid() );
    return *_mh.module();
  }




public:

  /** Decimate (perform _n_collapses collapses). Return number of
      performed collapses. If _n_collapses is not given reduce as
      much as possible */
  size_t decimate( size_t _n_collapses = 0 )
  {
    if (_n_collapses) 
      return decimate_to( Self::mesh_.n_vertices() - _n_collapses, 0);
    else
      return decimate_to( 0, 0 );
  }

  /// Decimate to target complexity, returns number of collapses
  size_t decimate_to( size_t  _n_vertices=0, size_t _n_faces=0 );


public:

  typedef typename Mesh::VertexHandle    VertexHandle;
  typedef typename Mesh::HalfedgeHandle  HalfedgeHandle;

  /// Heap interface
  class HeapInterface
  {
  public:

    HeapInterface(Mesh&               _mesh,
      VPropHandleT<float> _prio,
      VPropHandleT<int>   _pos)
      : mesh_(_mesh), prio_(_prio), pos_(_pos)
    { }

    inline bool
    less( VertexHandle _vh0, VertexHandle _vh1 )
    { return mesh_.property(prio_, _vh0) < mesh_.property(prio_, _vh1); }

    inline bool
    greater( VertexHandle _vh0, VertexHandle _vh1 )
    { return mesh_.property(prio_, _vh0) > mesh_.property(prio_, _vh1); }

    inline int
    get_heap_position(VertexHandle _vh)
    { return mesh_.property(pos_, _vh); }

    inline void
    set_heap_position(VertexHandle _vh, int _pos)
    { mesh_.property(pos_, _vh) = _pos; }


  private:
    Mesh&                mesh_;
    VPropHandleT<float>  prio_;
    VPropHandleT<int>    pos_;
  };

  typedef Utils::HeapT<VertexHandle, HeapInterface>  DeciHeap;


private: //---------------------------------------------------- private methods

  /// Insert vertex in heap
  void heap_vertex(VertexHandle _vh);

  /// Is an edge collapse legal?  Performs topological test only.
  /// The method evaluates the status bit Locked, Deleted, and Feature.
  /// \attention The method temporarily sets the bit Tagged. After usage
  ///            the bit will be disabled!
  bool is_collapse_legal(const CollapseInfo& _ci);

  /// Calculate priority of an halfedge collapse (using the modules)
  float collapse_priority(const CollapseInfo& _ci);

  /// Pret-process a collapse
  void preprocess_collapse(CollapseInfo& _ci);

  /// Post-process a collapse
  void postprocess_collapse(CollapseInfo& _ci);




private: //------------------------------------------------------- private data


  // reference to mesh
  Mesh&      mesh_;

  // heap
  std::auto_ptr<DeciHeap> heap_;

  // list of modules
  ModuleList bmodules_;
  Module*    cmodule_;

  bool       initialized_, emit_progress_;


  // vertex properties
  VPropHandleT<HalfedgeHandle>  collapse_target_;
  VPropHandleT<float>           priority_;
  VPropHandleT<int>             heap_position_;



private: // Noncopyable

  DecimaterT(const Self&);
  Self& operator = (const Self&);

};

//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_DECIMATER_DECIMATERT_CC)
#define OPENMESH_DECIMATER_TEMPLATES
#include "DecimaterT.cc"
#endif
//=============================================================================
#endif // OPENMESH_DECIMATER_DECIMATERT_HH defined
//=============================================================================

