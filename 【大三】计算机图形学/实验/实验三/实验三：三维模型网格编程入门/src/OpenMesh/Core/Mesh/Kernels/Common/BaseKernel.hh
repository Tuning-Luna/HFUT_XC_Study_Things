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
//   $Date: 2005/04/19 07:59:45 $
//
//=============================================================================


//=============================================================================
//
//  CLASS BaseKernel
//
//=============================================================================


#ifndef OPENMESH_BASE_KERNEL_HH
#define OPENMESH_BASE_KERNEL_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
// --------------------
#include <vector>
#include <string>
#include <algorithm>
// --------------------
#include <OpenMesh/Core/Utils/Property.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {


//== CLASS DEFINITION =========================================================

/// This class provides the basic property management like adding/removing
/// properties and access to properties.
/// All operations provided by %BaseKernel need at least a property handle
/// (VPropHandleT, EPropHandleT, HPropHandleT, FPropHandleT, MPropHandleT).
/// which keeps the data type of the property, too.
///
/// There are two types of properties:
/// -# Standard properties - mesh data (e.g. vertex normal or face color)
/// -# Custom properties - user defined data
///
/// The differentiation is only semantically, technically both are
/// equally handled. Therefore the methods provided by the %BaseKernel
/// are applicable to both property types.
///
/// \attention Since the class PolyMeshT derives from a kernel, hence all public
/// elements of %BaseKernel are usable.

class BaseKernel
{
public: //-------------------------------------------- constructor / destructor

  BaseKernel() {}
  virtual ~BaseKernel() {}


public: //-------------------------------------------------- add new properties

  /// \name Add a property to a mesh item

  //@{

  /** Adds a property
   *
   *  Depending on the property handle type a vertex, (half-)edge, face or
   *  mesh property is added to the mesh. If the action fails the handle
   *  is invalid.
   *  On success the handle must be used to access the property data with
   *  property().
   *
   *  \param  _ph   A property handle defining the data type to bind to mesh.
   *                On success the handle is valid else invalid.
   *  \param  _name Optional name of property. Following restrictions apply
   *                to the name:
   *                -# Maximum length of name is 256 characters
   *                -# The prefixes matching "^[vhefm]:" are reserved for
   *                   internal usage.
   *                -# The expression "^<.*>$" is reserved for internal usage.
   *  \return \c true on success else \c false.
   *
   */

  template <class T> void add_property( VPropHandleT<T>& _ph,
          const std::string& _name="<vprop>" )
  {
    _ph = VPropHandleT<T>( vprops_.add(T(), _name) );
    vprops_.resize(n_vertices());
  }

  template <class T> void add_property( HPropHandleT<T>& _ph,
          const std::string& _name="<hprop>" )
  {
    _ph = HPropHandleT<T>( hprops_.add(T(), _name) );
    hprops_.resize(n_halfedges());
  }

  template <class T> void add_property( EPropHandleT<T>& _ph,
          const std::string& _name="<eprop>" )
  {
    _ph = EPropHandleT<T>( eprops_.add(T(), _name) );
    eprops_.resize(n_edges());
  }

  template <class T> void add_property( FPropHandleT<T>& _ph,
          const std::string& _name="<fprop>" )
  {
    _ph = FPropHandleT<T>( fprops_.add(T(), _name) );
    fprops_.resize(n_faces());
  }

  template <class T> void add_property( MPropHandleT<T>& _ph,
          const std::string& _name="<mprop>" )
  {
    _ph = MPropHandleT<T>( mprops_.add(T(), _name) );
    mprops_.resize(1);
  }

  //@}


public: //--------------------------------------------------- remove properties

  /// \name Removing a property from a mesh tiem
  //@{

  /** Remove a property.
   *
   *  Removes the property represented by the handle from the apropriate
   *  mesh item.
   *  \param _ph Property to be removed. The handle is invalid afterwords.
   */

  template <typename T>
  void remove_property(VPropHandleT<T>& _ph) {
    if (_ph.is_valid())
      vprops_.remove(_ph);
    _ph.reset();
  }

  template <typename T>
  void remove_property(HPropHandleT<T>& _ph) {
    if (_ph.is_valid())
      hprops_.remove(_ph);
    _ph.reset();
  }

  template <typename T>
  void remove_property(EPropHandleT<T>& _ph) {
    if (_ph.is_valid())
      eprops_.remove(_ph);
    _ph.reset();
  }

  template <typename T>
  void remove_property(FPropHandleT<T>& _ph) {
    if (_ph.is_valid())
      fprops_.remove(_ph);
    _ph.reset();
  }

  template <typename T>
  void remove_property(MPropHandleT<T>& _ph) {
    if (_ph.is_valid())
      mprops_.remove(_ph);
    _ph.reset();
  }

  //@}


public: //------------------------------------------------ get handle from name

  /// \name Get property handle by name
  //@{

  /** Retrieves the handle to a named property by it's name.
   *
   *  \param _ph    A property handle. On success the handle is valid else
   *                invalid.
   *  \param _name  Name of wanted property.
   *  \return \c true if such a named property is available, else \c false.
   */

  template <class T>
  bool get_property_handle(VPropHandleT<T>& _ph,
         const std::string& _name) const
  {
    return (_ph = VPropHandleT<T>(vprops_.handle(T(), _name))).is_valid();
  }

  template <class T>
  bool get_property_handle(HPropHandleT<T>& _ph,
         const std::string& _name) const
  {
    return (_ph = HPropHandleT<T>(hprops_.handle(T(), _name))).is_valid();
  }

  template <class T>
  bool get_property_handle(EPropHandleT<T>& _ph,
         const std::string& _name) const
  {
    return (_ph = EPropHandleT<T>(eprops_.handle(T(), _name))).is_valid();
  }

  template <class T>
  bool get_property_handle(FPropHandleT<T>& _ph,
         const std::string& _name) const
  {
    return (_ph = FPropHandleT<T>(fprops_.handle(T(), _name))).is_valid();
  }

  template <class T>
  bool get_property_handle(MPropHandleT<T>& _ph,
         const std::string& _name) const
  {
    return (_ph = MPropHandleT<T>(mprops_.handle(T(), _name))).is_valid();
  }

  //@}


public: //--------------------------------------------------- access properties

  /// \name Access a property
  //@{

  /** Access a property
   *
   *  This method returns a reference to property. The property handle
   *  must be valid! The result is unpredictable if the handle is invalid!
   *
   *  \param  _ph     A \em valid (!) property handle.
   *  \return The wanted property if the handle is valid.
   */

  template <class T>
  PropertyT<T>& property(VPropHandleT<T> _ph) {
    return vprops_.property(_ph);
  }
  template <class T>
  const PropertyT<T>& property(VPropHandleT<T> _ph) const {
    return vprops_.property(_ph);
  }


  template <class T>
  PropertyT<T>& property(HPropHandleT<T> _ph) {
    return hprops_.property(_ph);
  }
  template <class T>
  const PropertyT<T>& property(HPropHandleT<T> _ph) const {
    return hprops_.property(_ph);
  }


  template <class T>
  PropertyT<T>& property(EPropHandleT<T> _ph) {
    return eprops_.property(_ph);
  }
  template <class T>
  const PropertyT<T>& property(EPropHandleT<T> _ph) const {
    return eprops_.property(_ph);
  }


  template <class T>
  PropertyT<T>& property(FPropHandleT<T> _ph) {
    return fprops_.property(_ph);
  }
  template <class T>
  const PropertyT<T>& property(FPropHandleT<T> _ph) const {
    return fprops_.property(_ph);
  }

  template <class T>
  PropertyT<T>& mproperty(MPropHandleT<T> _ph) {
    return mprops_.property(_ph);
  }
  template <class T>
  const PropertyT<T>& mproperty(MPropHandleT<T> _ph) const {
    return mprops_.property(_ph);
  }

  //@}

public: //-------------------------------------------- access property elements

  /// \name Access a property element using a handle to a mesh item
  //@{

  /** Return value of property for an item
   */

  template <class T>
  typename VPropHandleT<T>::reference
  property(VPropHandleT<T> _ph, VertexHandle _vh) {
    return vprops_.property(_ph)[_vh.idx()];
  }
  template <class T>
  typename VPropHandleT<T>::const_reference
  property(VPropHandleT<T> _ph, VertexHandle _vh) const {
    return vprops_.property(_ph)[_vh.idx()];
  }


  template <class T>
  typename HPropHandleT<T>::reference
  property(HPropHandleT<T> _ph, HalfedgeHandle _hh) {
    return hprops_.property(_ph)[_hh.idx()];
  }

  template <class T>
  typename HPropHandleT<T>::const_reference
  property(HPropHandleT<T> _ph, HalfedgeHandle _hh) const {
    return hprops_.property(_ph)[_hh.idx()];
  }


  template <class T>
  typename EPropHandleT<T>::reference
  property(EPropHandleT<T> _ph, EdgeHandle _eh) {
    return eprops_.property(_ph)[_eh.idx()];
  }

  template <class T>
  typename EPropHandleT<T>::const_reference
  property(EPropHandleT<T> _ph, EdgeHandle _eh) const {
    return eprops_.property(_ph)[_eh.idx()];
  }


  template <class T>
  typename FPropHandleT<T>::reference
  property(FPropHandleT<T> _ph, FaceHandle _fh) {
    return fprops_.property(_ph)[_fh.idx()];
  }

  template <class T>
  typename FPropHandleT<T>::const_reference
  property(FPropHandleT<T> _ph, FaceHandle _fh) const {
    return fprops_.property(_ph)[_fh.idx()];
  }


  template <class T>
  typename MPropHandleT<T>::reference
  property(MPropHandleT<T> _ph) {
    return mprops_.property(_ph)[0];
  }

  template <class T>
  typename MPropHandleT<T>::const_reference
  property(MPropHandleT<T> _ph) const {
    return mprops_.property(_ph)[0];
  }

  //@}

protected: //------------------------------------------------- low-level access

public: // used by non-native kernel and MeshIO, should be protected

  size_t n_vprops(void) const { return vprops_.size(); }

  size_t n_eprops(void) const { return eprops_.size(); }

  size_t n_hprops(void) const { return hprops_.size(); }

  size_t n_fprops(void) const { return fprops_.size(); }

  size_t n_mprops(void) const { return mprops_.size(); }

  BaseProperty* _get_vprop( const std::string& _name)
  { return vprops_.property(_name); }

  BaseProperty* _get_eprop( const std::string& _name)
  { return eprops_.property(_name); }

  BaseProperty* _get_hprop( const std::string& _name)
  { return hprops_.property(_name); }

  BaseProperty* _get_fprop( const std::string& _name)
  { return fprops_.property(_name); }

  BaseProperty* _get_mprop( const std::string& _name)
  { return mprops_.property(_name); }

  const BaseProperty* _get_vprop( const std::string& _name) const
  { return vprops_.property(_name); }

  const BaseProperty* _get_eprop( const std::string& _name) const
  { return eprops_.property(_name); }

  const BaseProperty* _get_hprop( const std::string& _name) const
  { return hprops_.property(_name); }

  const BaseProperty* _get_fprop( const std::string& _name) const
  { return fprops_.property(_name); }

  const BaseProperty* _get_mprop( const std::string& _name) const
  { return mprops_.property(_name); }

  BaseProperty& _vprop( size_t _idx ) { return vprops_._property( _idx ); }
  BaseProperty& _eprop( size_t _idx ) { return eprops_._property( _idx ); }
  BaseProperty& _hprop( size_t _idx ) { return hprops_._property( _idx ); }
  BaseProperty& _fprop( size_t _idx ) { return fprops_._property( _idx ); }
  BaseProperty& _mprop( size_t _idx ) { return mprops_._property( _idx ); }

  const BaseProperty& _vprop( size_t _idx ) const
  { return vprops_._property( _idx ); }
  const BaseProperty& _eprop( size_t _idx ) const
  { return eprops_._property( _idx ); }
  const BaseProperty& _hprop( size_t _idx ) const
  { return hprops_._property( _idx ); }
  const BaseProperty& _fprop( size_t _idx ) const
  { return fprops_._property( _idx ); }
  const BaseProperty& _mprop( size_t _idx ) const
  { return mprops_._property( _idx ); }

  size_t _add_vprop( BaseProperty* _bp ) { return vprops_._add( _bp ); }
  size_t _add_eprop( BaseProperty* _bp ) { return eprops_._add( _bp ); }
  size_t _add_hprop( BaseProperty* _bp ) { return hprops_._add( _bp ); }
  size_t _add_fprop( BaseProperty* _bp ) { return fprops_._add( _bp ); }
  size_t _add_mprop( BaseProperty* _bp ) { return mprops_._add( _bp ); }

protected: // low-level access non-public

  BaseProperty& _vprop( BaseHandle _h )
  { return vprops_._property( _h.idx() ); }
  BaseProperty& _eprop( BaseHandle _h )
  { return eprops_._property( _h.idx() ); }
  BaseProperty& _hprop( BaseHandle _h )
  { return hprops_._property( _h.idx() ); }
  BaseProperty& _fprop( BaseHandle _h )
  { return fprops_._property( _h.idx() ); }
  BaseProperty& _mprop( BaseHandle _h )
  { return mprops_._property( _h.idx() ); }

  const BaseProperty& _vprop( BaseHandle _h ) const
  { return vprops_._property( _h.idx() ); }
  const BaseProperty& _eprop( BaseHandle _h ) const
  { return eprops_._property( _h.idx() ); }
  const BaseProperty& _hprop( BaseHandle _h ) const
  { return hprops_._property( _h.idx() ); }
  const BaseProperty& _fprop( BaseHandle _h ) const
  { return fprops_._property( _h.idx() ); }
  const BaseProperty& _mprop( BaseHandle _h ) const
  { return mprops_._property( _h.idx() ); }


public: //----------------------------------------------------- element numbers


  virtual unsigned int n_vertices()  const { return 0; }
  virtual unsigned int n_halfedges() const { return 0; }
  virtual unsigned int n_edges()     const { return 0; }
  virtual unsigned int n_faces()     const { return 0; }


protected: //------------------------------------------- synchronize properties

  void vprops_reserve(unsigned int _n) const { vprops_.reserve(_n); }
  void vprops_resize(unsigned int _n) const { vprops_.resize(_n); }
  void vprops_free_mem() const { vprops_.free_mem(); }
  void vprops_swap(unsigned int _i0, unsigned int _i1) const {
    vprops_.swap(_i0, _i1);
  }

  void hprops_reserve(unsigned int _n) const { hprops_.reserve(_n); }
  void hprops_resize(unsigned int _n) const { hprops_.resize(_n); }
  void hprops_free_mem() const { hprops_.free_mem(); }
  void hprops_swap(unsigned int _i0, unsigned int _i1) const {
    hprops_.swap(_i0, _i1);
  }

  void eprops_reserve(unsigned int _n) const { eprops_.reserve(_n); }
  void eprops_resize(unsigned int _n) const { eprops_.resize(_n); }
  void eprops_free_mem() const { eprops_.free_mem(); }
  void eprops_swap(unsigned int _i0, unsigned int _i1) const {
    eprops_.swap(_i0, _i1);
  }

  void fprops_reserve(unsigned int _n) const { fprops_.reserve(_n); }
  void fprops_resize(unsigned int _n) const { fprops_.resize(_n); }
  void fprops_free_mem() const { fprops_.free_mem(); }
  void fprops_swap(unsigned int _i0, unsigned int _i1) const {
    fprops_.swap(_i0, _i1);
  }

  void mprops_resize(unsigned int _n) const { mprops_.resize(_n); }

public:

  void property_stats(std::ostream& _ostr = std::clog) const;

public:

  typedef PropertyContainer::iterator prop_iterator;
  typedef PropertyContainer::const_iterator const_prop_iterator;

  prop_iterator vprops_begin() { return vprops_.begin(); }
  prop_iterator vprops_end()   { return vprops_.end(); }
  const_prop_iterator vprops_begin() const { return vprops_.begin(); }
  const_prop_iterator vprops_end()   const { return vprops_.end(); }

  prop_iterator eprops_begin() { return eprops_.begin(); }
  prop_iterator eprops_end()   { return eprops_.end(); }
  const_prop_iterator eprops_begin() const { return eprops_.begin(); }
  const_prop_iterator eprops_end()   const { return eprops_.end(); }

  prop_iterator hprops_begin() { return hprops_.begin(); }
  prop_iterator hprops_end()   { return hprops_.end(); }
  const_prop_iterator hprops_begin() const { return hprops_.begin(); }
  const_prop_iterator hprops_end()   const { return hprops_.end(); }

  prop_iterator fprops_begin() { return fprops_.begin(); }
  prop_iterator fprops_end()   { return fprops_.end(); }
  const_prop_iterator fprops_begin() const { return fprops_.begin(); }
  const_prop_iterator fprops_end()   const { return fprops_.end(); }

  prop_iterator mprops_begin() { return mprops_.begin(); }
  prop_iterator mprops_end()   { return mprops_.end(); }
  const_prop_iterator mprops_begin() const { return mprops_.begin(); }
  const_prop_iterator mprops_end()   const { return mprops_.end(); }

private:

  PropertyContainer  vprops_;
  PropertyContainer  hprops_;
  PropertyContainer  eprops_;
  PropertyContainer  fprops_;
  PropertyContainer  mprops_;
};


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_BASE_KERNEL_HH defined
//=============================================================================
