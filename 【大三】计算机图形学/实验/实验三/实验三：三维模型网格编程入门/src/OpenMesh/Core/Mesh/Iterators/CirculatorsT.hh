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
#ifndef OPENMESH_CIRCULATORS_HH
#define OPENMESH_CIRCULATORS_HH
//=============================================================================
//
//  Vertex and Face circulators for PolyMesh/TriMesh
//
//=============================================================================



//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
#include <assert.h>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Iterators {


//== FORWARD DECLARATIONS =====================================================


template <class Mesh> class VertexVertexIterT;
template <class Mesh> class VertexIHalfedgeIterT;
template <class Mesh> class VertexOHalfedgeIterT;
template <class Mesh> class VertexEdgeIterT;
template <class Mesh> class VertexFaceIterT;

template <class Mesh> class ConstVertexVertexIterT;
template <class Mesh> class ConstVertexIHalfedgeIterT;
template <class Mesh> class ConstVertexOHalfedgeIterT;
template <class Mesh> class ConstVertexEdgeIterT;
template <class Mesh> class ConstVertexFaceIterT;

template <class Mesh> class FaceVertexIterT;
template <class Mesh> class FaceHalfedgeIterT;
template <class Mesh> class FaceEdgeIterT;
template <class Mesh> class FaceFaceIterT;

template <class Mesh> class ConstFaceVertexIterT;
template <class Mesh> class ConstFaceHalfedgeIterT;
template <class Mesh> class ConstFaceEdgeIterT;
template <class Mesh> class ConstFaceFaceIterT;



//== CLASS DEFINITION =========================================================

	      
/** \class VertexVertexIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class VertexVertexIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Vertex           value_type;
  typedef typename Mesh::VertexHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Vertex&   reference;
  typedef const typename Mesh::Vertex*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Vertex&         reference;
  typedef typename Mesh::Vertex*         pointer;
#endif



  /// Default constructor
  VertexVertexIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  VertexVertexIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  VertexVertexIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  VertexVertexIterT(const VertexVertexIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  VertexVertexIterT& operator=(const VertexVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  VertexVertexIterT(const VertexVertexIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  VertexVertexIterT& operator=(const VertexVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexVertexIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const VertexVertexIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const VertexVertexIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  VertexVertexIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  VertexVertexIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::VertexHandle handle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_);; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::VertexHandle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_);; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstVertexVertexIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstVertexVertexIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Vertex           value_type;
  typedef typename Mesh::VertexHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Vertex&   reference;
  typedef const typename Mesh::Vertex*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Vertex&         reference;
  typedef typename Mesh::Vertex*         pointer;
#endif



  /// Default constructor
  ConstVertexVertexIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  ConstVertexVertexIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstVertexVertexIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstVertexVertexIterT(const ConstVertexVertexIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstVertexVertexIterT& operator=(const ConstVertexVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstVertexVertexIterT(const VertexVertexIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstVertexVertexIterT& operator=(const VertexVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexVertexIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstVertexVertexIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstVertexVertexIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstVertexVertexIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstVertexVertexIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::VertexHandle handle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_);; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::VertexHandle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_);; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class VertexOHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class VertexOHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Halfedge&   reference;
  typedef const typename Mesh::Halfedge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Halfedge&         reference;
  typedef typename Mesh::Halfedge*         pointer;
#endif



  /// Default constructor
  VertexOHalfedgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  VertexOHalfedgeIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  VertexOHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  VertexOHalfedgeIterT(const VertexOHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  VertexOHalfedgeIterT& operator=(const VertexOHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  VertexOHalfedgeIterT(const VertexOHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  VertexOHalfedgeIterT& operator=(const VertexOHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexOHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const VertexOHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const VertexOHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  VertexOHalfedgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  VertexOHalfedgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return heh_; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return heh_; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstVertexOHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstVertexOHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Halfedge&   reference;
  typedef const typename Mesh::Halfedge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Halfedge&         reference;
  typedef typename Mesh::Halfedge*         pointer;
#endif



  /// Default constructor
  ConstVertexOHalfedgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  ConstVertexOHalfedgeIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstVertexOHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstVertexOHalfedgeIterT(const ConstVertexOHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstVertexOHalfedgeIterT& operator=(const ConstVertexOHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstVertexOHalfedgeIterT(const VertexOHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstVertexOHalfedgeIterT& operator=(const VertexOHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexOHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstVertexOHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstVertexOHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstVertexOHalfedgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstVertexOHalfedgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return heh_; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return heh_; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class VertexIHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class VertexIHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Halfedge&   reference;
  typedef const typename Mesh::Halfedge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Halfedge&         reference;
  typedef typename Mesh::Halfedge*         pointer;
#endif



  /// Default constructor
  VertexIHalfedgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  VertexIHalfedgeIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  VertexIHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  VertexIHalfedgeIterT(const VertexIHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  VertexIHalfedgeIterT& operator=(const VertexIHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  VertexIHalfedgeIterT(const VertexIHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  VertexIHalfedgeIterT& operator=(const VertexIHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexIHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const VertexIHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const VertexIHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  VertexIHalfedgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  VertexIHalfedgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return mesh_->opposite_halfedge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return mesh_->opposite_halfedge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstVertexIHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstVertexIHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Halfedge&   reference;
  typedef const typename Mesh::Halfedge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Halfedge&         reference;
  typedef typename Mesh::Halfedge*         pointer;
#endif



  /// Default constructor
  ConstVertexIHalfedgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  ConstVertexIHalfedgeIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstVertexIHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstVertexIHalfedgeIterT(const ConstVertexIHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstVertexIHalfedgeIterT& operator=(const ConstVertexIHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstVertexIHalfedgeIterT(const VertexIHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstVertexIHalfedgeIterT& operator=(const VertexIHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexIHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstVertexIHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstVertexIHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstVertexIHalfedgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstVertexIHalfedgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return mesh_->opposite_halfedge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return mesh_->opposite_halfedge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class VertexEdgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class VertexEdgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Edge           value_type;
  typedef typename Mesh::EdgeHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Edge&   reference;
  typedef const typename Mesh::Edge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Edge&         reference;
  typedef typename Mesh::Edge*         pointer;
#endif



  /// Default constructor
  VertexEdgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  VertexEdgeIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  VertexEdgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  VertexEdgeIterT(const VertexEdgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  VertexEdgeIterT& operator=(const VertexEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  VertexEdgeIterT(const VertexEdgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  VertexEdgeIterT& operator=(const VertexEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexEdgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const VertexEdgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const VertexEdgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  VertexEdgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  VertexEdgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::EdgeHandle handle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::EdgeHandle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstVertexEdgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstVertexEdgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Edge           value_type;
  typedef typename Mesh::EdgeHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Edge&   reference;
  typedef const typename Mesh::Edge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Edge&         reference;
  typedef typename Mesh::Edge*         pointer;
#endif



  /// Default constructor
  ConstVertexEdgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  ConstVertexEdgeIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstVertexEdgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstVertexEdgeIterT(const ConstVertexEdgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstVertexEdgeIterT& operator=(const ConstVertexEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstVertexEdgeIterT(const VertexEdgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstVertexEdgeIterT& operator=(const VertexEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexEdgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstVertexEdgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstVertexEdgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstVertexEdgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->cw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstVertexEdgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->ccw_rotated_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::EdgeHandle handle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::EdgeHandle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class VertexFaceIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class VertexFaceIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Face           value_type;
  typedef typename Mesh::FaceHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Face&   reference;
  typedef const typename Mesh::Face*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Face&         reference;
  typedef typename Mesh::Face*         pointer;
#endif



  /// Default constructor
  VertexFaceIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  VertexFaceIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Construct with mesh and start halfedge
  VertexFaceIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Copy constructor
  VertexFaceIterT(const VertexFaceIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Assignment operator
  VertexFaceIterT& operator=(const VertexFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  VertexFaceIterT(const VertexFaceIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// assign from non-const circulator
  VertexFaceIterT& operator=(const VertexFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexFaceIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const VertexFaceIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const VertexFaceIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  VertexFaceIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    do heh_=mesh_->cw_rotated_halfedge_handle(heh_);  while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  VertexFaceIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    do	heh_=mesh_->ccw_rotated_halfedge_handle(heh_); while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::FaceHandle handle() const {
    assert(mesh_);
    return mesh_->face_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::FaceHandle() const {
    assert(mesh_);
    return mesh_->face_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstVertexFaceIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstVertexFaceIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Face           value_type;
  typedef typename Mesh::FaceHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Face&   reference;
  typedef const typename Mesh::Face*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Face&         reference;
  typedef typename Mesh::Face*         pointer;
#endif



  /// Default constructor
  ConstVertexFaceIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::VertexHandle
  ConstVertexFaceIterT(mesh_ref _mesh, typename Mesh::VertexHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Construct with mesh and start halfedge
  ConstVertexFaceIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Copy constructor
  ConstVertexFaceIterT(const ConstVertexFaceIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Assignment operator
  ConstVertexFaceIterT& operator=(const ConstVertexFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstVertexFaceIterT(const VertexFaceIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// assign from non-const circulator
  ConstVertexFaceIterT& operator=(const VertexFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstVertexFaceIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstVertexFaceIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstVertexFaceIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstVertexFaceIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    do heh_=mesh_->cw_rotated_halfedge_handle(heh_);  while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstVertexFaceIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    do	heh_=mesh_->ccw_rotated_halfedge_handle(heh_); while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::FaceHandle handle() const {
    assert(mesh_);
    return mesh_->face_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::FaceHandle() const {
    assert(mesh_);
    return mesh_->face_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceVertexIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceVertexIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Vertex           value_type;
  typedef typename Mesh::VertexHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Vertex&   reference;
  typedef const typename Mesh::Vertex*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Vertex&         reference;
  typedef typename Mesh::Vertex*         pointer;
#endif



  /// Default constructor
  FaceVertexIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceVertexIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  FaceVertexIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  FaceVertexIterT(const FaceVertexIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  FaceVertexIterT& operator=(const FaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceVertexIterT(const FaceVertexIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  FaceVertexIterT& operator=(const FaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceVertexIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceVertexIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const FaceVertexIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next ccw target)
  FaceVertexIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->next_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next cw target)
  FaceVertexIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->prev_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::VertexHandle handle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::VertexHandle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceVertexIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceVertexIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Vertex           value_type;
  typedef typename Mesh::VertexHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Vertex&   reference;
  typedef const typename Mesh::Vertex*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Vertex&         reference;
  typedef typename Mesh::Vertex*         pointer;
#endif



  /// Default constructor
  ConstFaceVertexIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceVertexIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstFaceVertexIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstFaceVertexIterT(const ConstFaceVertexIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstFaceVertexIterT& operator=(const ConstFaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceVertexIterT(const FaceVertexIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstFaceVertexIterT& operator=(const FaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceVertexIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceVertexIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceVertexIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next ccw target)
  ConstFaceVertexIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->next_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next cw target)
  ConstFaceVertexIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->prev_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::VertexHandle handle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::VertexHandle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Halfedge&   reference;
  typedef const typename Mesh::Halfedge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Halfedge&         reference;
  typedef typename Mesh::Halfedge*         pointer;
#endif



  /// Default constructor
  FaceHalfedgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceHalfedgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  FaceHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  FaceHalfedgeIterT(const FaceHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  FaceHalfedgeIterT& operator=(const FaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceHalfedgeIterT(const FaceHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  FaceHalfedgeIterT& operator=(const FaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const FaceHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceHalfedgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->next_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceHalfedgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->prev_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return heh_; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return heh_; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Halfedge&   reference;
  typedef const typename Mesh::Halfedge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Halfedge&         reference;
  typedef typename Mesh::Halfedge*         pointer;
#endif



  /// Default constructor
  ConstFaceHalfedgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceHalfedgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstFaceHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstFaceHalfedgeIterT(const ConstFaceHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstFaceHalfedgeIterT& operator=(const ConstFaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceHalfedgeIterT(const FaceHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstFaceHalfedgeIterT& operator=(const FaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceHalfedgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->next_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceHalfedgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->prev_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return heh_; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return heh_; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceEdgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceEdgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Edge           value_type;
  typedef typename Mesh::EdgeHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Edge&   reference;
  typedef const typename Mesh::Edge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Edge&         reference;
  typedef typename Mesh::Edge*         pointer;
#endif



  /// Default constructor
  FaceEdgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceEdgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  FaceEdgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  FaceEdgeIterT(const FaceEdgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  FaceEdgeIterT& operator=(const FaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceEdgeIterT(const FaceEdgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  FaceEdgeIterT& operator=(const FaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceEdgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceEdgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const FaceEdgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceEdgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->next_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceEdgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->prev_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::EdgeHandle handle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::EdgeHandle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceEdgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceEdgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Edge           value_type;
  typedef typename Mesh::EdgeHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Edge&   reference;
  typedef const typename Mesh::Edge*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Edge&         reference;
  typedef typename Mesh::Edge*         pointer;
#endif



  /// Default constructor
  ConstFaceEdgeIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceEdgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstFaceEdgeIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  {  ; }


  /// Copy constructor
  ConstFaceEdgeIterT(const ConstFaceEdgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// Assignment operator
  ConstFaceEdgeIterT& operator=(const ConstFaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceEdgeIterT(const FaceEdgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  {  ; }


  /// assign from non-const circulator
  ConstFaceEdgeIterT& operator=(const FaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceEdgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceEdgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceEdgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceEdgeIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->next_halfedge_handle(heh_);;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceEdgeIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    heh_=mesh_->prev_halfedge_handle(heh_);;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::EdgeHandle handle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::EdgeHandle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceFaceIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceFaceIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Face           value_type;
  typedef typename Mesh::FaceHandle         value_handle;

#if 0
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Face&   reference;
  typedef const typename Mesh::Face*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Face&         reference;
  typedef typename Mesh::Face*         pointer;
#endif



  /// Default constructor
  FaceFaceIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceFaceIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Construct with mesh and start halfedge
  FaceFaceIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Copy constructor
  FaceFaceIterT(const FaceFaceIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Assignment operator
  FaceFaceIterT& operator=(const FaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceFaceIterT(const FaceFaceIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// assign from non-const circulator
  FaceFaceIterT& operator=(const FaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceFaceIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceFaceIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const FaceFaceIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceFaceIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    do heh_=mesh_->next_halfedge_handle(heh_); while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceFaceIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    do heh_=mesh_->prev_halfedge_handle(heh_); while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::FaceHandle handle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::FaceHandle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceFaceIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceFaceIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle   HalfedgeHandle;

  typedef typename Mesh::Face           value_type;
  typedef typename Mesh::FaceHandle         value_handle;

#if 1
  typedef const Mesh&         mesh_ref;
  typedef const Mesh*         mesh_ptr;
  typedef const typename Mesh::Face&   reference;
  typedef const typename Mesh::Face*   pointer;
#else
  typedef Mesh&               mesh_ref;
  typedef Mesh*               mesh_ptr;
  typedef typename Mesh::Face&         reference;
  typedef typename Mesh::Face*         pointer;
#endif



  /// Default constructor
  ConstFaceFaceIterT() : mesh_(0), active_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceFaceIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Construct with mesh and start halfedge
  ConstFaceFaceIterT(mesh_ref _mesh, HalfedgeHandle _heh) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    active_(false)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Copy constructor
  ConstFaceFaceIterT(const ConstFaceFaceIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Assignment operator
  ConstFaceFaceIterT& operator=(const ConstFaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceFaceIterT(const FaceFaceIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    active_(_rhs.active_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// assign from non-const circulator
  ConstFaceFaceIterT& operator=(const FaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    active_ = _rhs.active_;
    return *this;
  }
#else
  friend class ConstFaceFaceIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceFaceIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (active_ == _rhs.active_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceFaceIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceFaceIterT& operator++() { 
    assert(mesh_);
    active_ = true;
    do heh_=mesh_->next_halfedge_handle(heh_); while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceFaceIterT& operator--() { 
    assert(mesh_);
    active_ = true;
    do heh_=mesh_->prev_halfedge_handle(heh_); while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::FaceHandle handle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::FaceHandle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (!active_));
  }


private:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  bool             active_;
};



//=============================================================================
} // namespace Iterators
} // namespace OpenMesh
//=============================================================================
#endif 
//=============================================================================
