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

#ifndef OPENMESH_PROPERTY_HH
#define OPENMESH_PROPERTY_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/Handles.hh>
#include <OpenMesh/Core/IO/StoreRestore.hh>
#include <vector>
#include <string>
#include <algorithm>


//== FORWARDDECLARATIONS ======================================================

namespace OpenMesh {
  class BaseKernel;
}


//== NAMESPACES ===============================================================

namespace OpenMesh {


//== CLASS DEFINITION =========================================================

/** \class BaseProperty Property.hh <OpenMesh/Core/Utils/PropertyT.hh>

    Abstract class defining the basic interface of a dynamic property.
**/

class BaseProperty
{
public:

  /// Indicates an error when a size is returned by a member.
  static const size_t UnknownSize = size_t(-1);

public:

  /// \brief Default constructor.
  ///
  /// In %OpenMesh all mesh data is stored in so-called properties.
  /// We distinuish between standard properties, which can be defined at
  /// compile time using the Attributes in the traits definition and
  /// at runtime using the request property functions defined in one of
  /// the kernels.
  ///
  /// If the property should be stored along with the default properties
  /// in the OM-format one must name the property and enable the persistant
  /// flag with set_persistent().
  ///
  /// \param _name Optional textual name for the property.
  ///
  BaseProperty(const std::string& _name = "<unknown>")
    : name_(_name), persistent_(false) {}

  /// Destructor.
  virtual ~BaseProperty() {}

public: // synchronized array interface

  /// Reserve memory for n elements.
  virtual void reserve(size_t _n) = 0;

  /// Resize storage to hold n elements.
  virtual void resize(size_t _n) = 0;

  /// Free unused memory.
  virtual void free_mem() = 0;

  /// Extend the number of elements by one.
  virtual void push_back() = 0;

  /// Let two elements swap their storage place.
  virtual void swap(size_t _i0, size_t _i1) = 0;

  /// Return a deep copy of self.
  virtual BaseProperty* clone () const = 0;

public: // named property interface

  /// Return the name of the property
  const std::string& name() const { return name_; }

  virtual void stats(std::ostream& _ostr) const;

public: // I/O support

  /// Returns true if the persistent flag is enabled else false.
  bool persistent(void) const { return persistent_; }

  /// Enable or disable persistency. Self must be a named property to enable
  /// persistency.
  virtual void set_persistent( bool _yn ) = 0;

  /// Number of elements in property
  virtual size_t       n_elements() const = 0;

  /// Size of one element in bytes or UnknownSize if not known.
  virtual size_t       element_size() const = 0;

  /// Return size of property in bytes
  virtual size_t       size_of() const
  {
    return size_of( n_elements() );
  }

  /// Estimated size of property if it has _n_elem elements.
  /// The member returns UnknownSize if the size cannot be estimated.
  virtual size_t       size_of(size_t _n_elem) const
  {
    return (element_size()!=UnknownSize)
      ? (_n_elem*element_size())
      : UnknownSize;
  }

  /// Store self as one binary block
  virtual size_t store( std::ostream& _ostr, bool _swap ) const = 0;

  /** Restore self from a binary block. Uses reserve() to set the
      size of self before restoring.
  **/
  virtual size_t restore( std::istream& _istr, bool _swap ) = 0;

protected:

  // To be used in a derived class, when overloading set_persistent()
  template < typename T >
  void check_and_set_persistent( bool _yn )
  {
    if ( _yn && !IO::is_streamable<T>() )
      omerr() << "Warning! Type of property value is not binary storable!\n";
    persistent_ = IO::is_streamable<T>() && _yn;
  }

private:

  std::string name_;
  bool        persistent_;
};

//-----------------------------------------------------------------------------


/** \class PropertyT Property.hh <OpenMesh/Core/Utils/PropertyT.hh>
 *
 *  \brief Default property class for any type T.
 *
 *  The default property class for any type T.
 *
 *  The property supports persistency if T is a "fundamental" type:
 *  - integer fundamental types except bool:
 *    char, short, int, long, long long (__int64 for MS VC++) and
 *    their unsigned companions.
 *  - float fundamentals except <tt>long double</tt>:
 *    float, double
 *  - %OpenMesh vector types
 *
 *  Persistency of non-fundamental types is supported if and only if a
 *  specialization of struct IO::binary<> exists for the wanted type.
 */
template <class T>
class PropertyT : public BaseProperty
{
public:

  typedef T                                       Value;
  typedef std::vector<T>                          vector_type;
  typedef T                                       value_type;
  typedef typename vector_type::reference         reference;
  typedef typename vector_type::const_reference   const_reference;

public:

  /// Default constructor
  PropertyT(const std::string& _name = "<unknown>")
  : BaseProperty(_name)
  {}

public: // inherited from BaseProperty

  virtual void reserve(size_t _n) { data_.reserve(_n);    }
  virtual void resize(size_t _n)  { data_.resize(_n);     }
  virtual void push_back()        { data_.push_back(T()); }
  virtual void free_mem()         { vector_type(data_).swap(data_); }
  virtual void swap(size_t _i0, size_t _i1)
  { std::swap(data_[_i0], data_[_i1]); }

public:

  virtual void set_persistent( bool _yn )
  { check_and_set_persistent<T>( _yn ); }

  virtual size_t       n_elements()   const { return data_.size(); }
  virtual size_t       element_size() const { return IO::size_of<T>(); }

#ifndef DOXY_IGNORE_THIS
  struct plus {
    size_t operator () ( size_t _b, const T& _v )
    { return _b + IO::size_of<T>(_v); }
  };
#endif

  virtual size_t size_of(void) const
  {
    if (element_size() != IO::UnknownSize)
      return this->BaseProperty::size_of(n_elements());
    return std::accumulate(data_.begin(), data_.end(), 0, plus());
  }

  virtual size_t size_of(size_t _n_elem) const
  { return this->BaseProperty::size_of(_n_elem); }

  virtual size_t store( std::ostream& _ostr, bool _swap ) const
  {
    if ( IO::is_streamable<vector_type>() )
      return IO::store(_ostr, data_, _swap );
    size_t bytes = 0;
    for (size_t i=0; i<n_elements(); ++i)
      bytes += IO::store( _ostr, data_[i], _swap );
    return bytes;
  }

  virtual size_t restore( std::istream& _istr, bool _swap )
  {
    if ( IO::is_streamable<vector_type>() )
      return IO::restore(_istr, data_, _swap );
    size_t bytes = 0;
    for (size_t i=0; i<n_elements(); ++i)
      bytes += IO::restore( _istr, data_[i], _swap );
    return bytes;
  }

public: // data access interface

  /// Get pointer to array (does not work for T==bool)
  const T* data() const { return &data_[0]; }

  /// Access the i'th element. No range check is performed!
  reference operator[](int _idx)
  {
    assert( size_t(_idx) < data_.size() );
    return data_[_idx];
  }

  /// Const access to the i'th element. No range check is performed!
  const_reference operator[](int _idx) const
  {
    assert( size_t(_idx) < data_.size());
    return data_[_idx];
  }

  /// Make a copy of self.
  PropertyT<T>* clone() const
  {
    PropertyT<T>* p = new PropertyT<T>();
    p->data_ = data_;
    return p;
  }


private:

  vector_type data_;
};

//-----------------------------------------------------------------------------


/** \class PropertyT<bool> Property.hh <OpenMesh/Core/Utils/PropertyT.hh>

  Property specialization for bool type. The data will be stored as
  a bitset.
 */
template <>
class PropertyT<bool> : public BaseProperty
{
public:

  typedef std::vector<bool>                       vector_type;
  typedef bool                                    value_type;
  typedef vector_type::reference                  reference;
  typedef vector_type::const_reference            const_reference;

public:

  PropertyT(const std::string& _name = "<unknown>")
    : BaseProperty(_name)
  { }

public: // inherited from BaseProperty

  virtual void reserve(size_t _n) { data_.reserve(_n);    }
  virtual void resize(size_t _n)  { data_.resize(_n);     }
  virtual void push_back()        { data_.push_back(bool()); }
  virtual void free_mem()         { vector_type(data_).swap(data_); }
  virtual void swap(size_t _i0, size_t _i1)
  { bool t(data_[_i0]); data_[_i0]=data_[_i1]; data_[_i1]=t; }

public:

  virtual void set_persistent( bool _yn )
  {
    check_and_set_persistent<bool>( _yn );
  }

  virtual size_t       n_elements()   const { return data_.size();  }
  virtual size_t       element_size() const { return UnknownSize;    }
  virtual size_t       size_of() const      { return size_of( n_elements() ); }
  virtual size_t       size_of(size_t _n_elem) const
  {
    return _n_elem / 8 + ((_n_elem % 8)!=0);
  }

  size_t store( std::ostream& _ostr, bool _swap ) const
  {
    size_t bytes = 0;

    size_t N = data_.size() / 8;
    size_t R = data_.size() % 8;

    size_t        idx;  // element index
    size_t        bidx;
    unsigned char bits; // bitset

    for (bidx=idx=0; idx < N; ++idx, bidx+=8)
    {
      bits = !!data_[bidx]
        | (!!data_[bidx+1] << 1)
        | (!!data_[bidx+2] << 2)
        | (!!data_[bidx+3] << 3)
        | (!!data_[bidx+4] << 4)
        | (!!data_[bidx+5] << 5)
        | (!!data_[bidx+6] << 6)
        | (!!data_[bidx+7] << 7);
      _ostr << bits;
    }
    bytes = N;

    if (R)
    {
      bits = 0;
      for (idx=0; idx < R; ++idx)
        bits |= !!data_[bidx+idx] << idx;
      _ostr << bits;
      ++bytes;
    }

    std::cout << std::endl;

    assert( bytes == size_of() );

    return bytes;
  }

  size_t restore( std::istream& _istr, bool _swap )
  {
    size_t bytes = 0;

    size_t N = data_.size() / 8;
    size_t R = data_.size() % 8;

    size_t        idx;  // element index
    size_t        bidx; //
    unsigned char bits; // bitset

    for (bidx=idx=0; idx < N; ++idx, bidx+=8)
    {
      _istr >> bits;
      data_[bidx+0] = !!(bits & 0x01);
      data_[bidx+1] = !!(bits & 0x02);
      data_[bidx+2] = !!(bits & 0x04);
      data_[bidx+3] = !!(bits & 0x08);
      data_[bidx+4] = !!(bits & 0x10);
      data_[bidx+5] = !!(bits & 0x20);
      data_[bidx+6] = !!(bits & 0x40);
      data_[bidx+7] = !!(bits & 0x80);
    }
    bytes = N;

    if (R)
    {
      _istr >> bits;
      for (idx=0; idx < R; ++idx)
        data_[bidx+idx] = !!(bits & (1<<idx));
      ++bytes;
    }

    std::cout << std::endl;

    return bytes;
  }


public:

  /// Access the i'th element. No range check is performed!
  reference operator[](int _idx)
  {
    assert( size_t(_idx) < data_.size() );
    return data_[_idx];
  }

  /// Const access to the i'th element. No range check is performed!
  const_reference operator[](int _idx) const
  {
    assert( size_t(_idx) < data_.size());
    return data_[_idx];
  }

  /// Make a copy of self.
  PropertyT<bool>* clone() const
  {
    PropertyT<bool>* p = new PropertyT<bool>();
    p->data_ = data_;
    return p;
  }


private:

  vector_type data_;
};


//-----------------------------------------------------------------------------


/** \class PropertyT<std::string> Property.hh <OpenMesh/Core/Utils/PropertyT.hh>

  Property specialization for std::string type.
*/
template <>
class PropertyT<std::string> : public BaseProperty
{
public:

  typedef std::string                             Value;
  typedef std::vector<std::string>                vector_type;
  typedef std::string                             value_type;
  typedef vector_type::reference                  reference;
  typedef vector_type::const_reference            const_reference;

public:

  PropertyT(const std::string& _name = "<unknown>")
    : BaseProperty(_name)
  { }

public: // inherited from BaseProperty

  virtual void reserve(size_t _n) { data_.reserve(_n);    }
  virtual void resize(size_t _n)  { data_.resize(_n);     }
  virtual void push_back()        { data_.push_back(std::string()); }
  virtual void free_mem()         { vector_type(data_).swap(data_); }
  virtual void swap(size_t _i0, size_t _i1) {
    std::swap(data_[_i0], data_[_i1]);
  }

public:

  virtual void set_persistent( bool _yn )
  { check_and_set_persistent<std::string>( _yn ); }

  virtual size_t       n_elements()   const { return data_.size();  }
  virtual size_t       element_size() const { return UnknownSize;    }
  virtual size_t       size_of() const
  { return IO::size_of( data_ ); }

  virtual size_t       size_of(size_t _n_elem) const
  { return UnknownSize; }

  /// Store self as one binary block. Max. length of a string is 65535 bytes.
  size_t store( std::ostream& _ostr, bool _swap ) const
  { return IO::store( _ostr, data_, _swap ); }

  size_t restore( std::istream& _istr, bool _swap )
  { return IO::restore( _istr, data_, _swap ); }

public:

  const value_type* data() const { return (value_type*) &data_[0]; }

  /// Access the i'th element. No range check is performed!
  reference operator[](int _idx) {
    assert( size_t(_idx) < data_.size());
    return ((value_type*) &data_[0])[_idx];
  }

  /// Const access the i'th element. No range check is performed!
  const_reference operator[](int _idx) const {
    assert( size_t(_idx) < data_.size());
    return ((value_type*) &data_[0])[_idx];
  }

  PropertyT<value_type>* clone() const {
    PropertyT<value_type>* p = new PropertyT<value_type>();
    p->data_ = data_;
    return p;
  }


private:

  vector_type data_;

};


//== CLASS DEFINITION =========================================================


/// Base property handle.
template <class T>
struct BasePropHandleT : public BaseHandle
{
  typedef T                                       Value;
  typedef std::vector<T>                          vector_type;
  typedef T                                       value_type;
  typedef typename vector_type::reference         reference;
  typedef typename vector_type::const_reference   const_reference;

  explicit BasePropHandleT(int _idx=-1) : BaseHandle(_idx) {}
};


/** \ingroup mesh_property_handle_group
 *  Handle representing a vertex property
 */
template <class T>
struct VPropHandleT : public BasePropHandleT<T>
{
  typedef T                       Value;
  typedef T                       value_type;

  explicit VPropHandleT(int _idx=-1) : BasePropHandleT<T>(_idx) {}
  explicit VPropHandleT(const BasePropHandleT<T>& _b) : BasePropHandleT<T>(_b) {}
};


/** \ingroup mesh_property_handle_group
 *  Handle representing a halfedge property
 */
template <class T>
struct HPropHandleT : public BasePropHandleT<T>
{
  typedef T                       Value;
  typedef T                       value_type;

  explicit HPropHandleT(int _idx=-1) : BasePropHandleT<T>(_idx) {}
  explicit HPropHandleT(const BasePropHandleT<T>& _b) : BasePropHandleT<T>(_b) {}
};


/** \ingroup mesh_property_handle_group
 *  Handle representing an edge property
 */
template <class T>
struct EPropHandleT : public BasePropHandleT<T>
{
  typedef T                       Value;
  typedef T                       value_type;

  explicit EPropHandleT(int _idx=-1) : BasePropHandleT<T>(_idx) {}
  explicit EPropHandleT(const BasePropHandleT<T>& _b) : BasePropHandleT<T>(_b) {}
};


/** \ingroup mesh_property_handle_group
 *  Handle representing a face property
 */
template <class T>
struct FPropHandleT : public BasePropHandleT<T>
{
  typedef T                       Value;
  typedef T                       value_type;

  explicit FPropHandleT(int _idx=-1) : BasePropHandleT<T>(_idx) {}
  explicit FPropHandleT(const BasePropHandleT<T>& _b) : BasePropHandleT<T>(_b) {}
};


/** \ingroup mesh_property_handle_group
 *  Handle representing a mesh property
 */
template <class T>
struct MPropHandleT : public BasePropHandleT<T>
{
  typedef T                       Value;
  typedef T                       value_type;

  explicit MPropHandleT(int _idx=-1) : BasePropHandleT<T>(_idx) {}
  explicit MPropHandleT(const BasePropHandleT<T>& _b) : BasePropHandleT<T>(_b) {}
};



//-----------------------------------------------------------------------------


/// A a container for properties.
class PropertyContainer
{
public:

  //-------------------------------------------------- constructor / destructor

  PropertyContainer() {}
  virtual ~PropertyContainer() { clear(); }


  //------------------------------------------------------------- info / access

  typedef std::vector<BaseProperty*> Properties;
  const Properties& properties() const { return properties_; }
  size_t size() const { return properties_.size(); }



  //--------------------------------------------------------- copy / assignment

  PropertyContainer(const PropertyContainer& _rhs) { operator=(_rhs); }

  PropertyContainer& operator=(const PropertyContainer& _rhs)
  {
    clear();
    properties_ = _rhs.properties_;
    Properties::iterator p_it=properties_.begin(), p_end=properties_.end();
    for (; p_it!=p_end; ++p_it)
      if (*p_it)
        *p_it = (*p_it)->clone();
    return *this;
  }



  //--------------------------------------------------------- manage properties

  template <class T>
  BasePropHandleT<T> add(const T&, const std::string& _name="<unknown>")
  {
    Properties::iterator p_it=properties_.begin(), p_end=properties_.end();
    int idx=0;
    for (; p_it!=p_end && *p_it!=NULL; ++p_it, ++idx);
    if (p_it==p_end) properties_.push_back(NULL);
    properties_[idx] = new PropertyT<T>(_name);
    return BasePropHandleT<T>(idx);
  }


  template <class T>
  BasePropHandleT<T> handle(const T&, const std::string& _name) const
  {
    Properties::const_iterator p_it = properties_.begin();
    for (int idx=0; p_it != properties_.end(); ++p_it, ++idx)
    {
      if (*p_it != NULL && (*p_it)->name() == _name && //skip deleted properties
          dynamic_cast<PropertyT<T>*>(properties_[idx]) != NULL)//check type
      {
        return BasePropHandleT<T>(idx);
      }
    }
    return BasePropHandleT<T>();
  }

  BaseProperty* property( const std::string& _name ) const
  {
    Properties::const_iterator p_it = properties_.begin();
    for (int idx=0; p_it != properties_.end(); ++p_it, ++idx)
    {
      if (*p_it != NULL && (*p_it)->name() == _name) //skip deleted properties
      {
        return *p_it;
      }
    }
    return NULL;
  }

  template <class T> PropertyT<T>& property(BasePropHandleT<T> _h)
  {
    assert(_h.idx() >= 0 && _h.idx() < (int)properties_.size());
    assert(properties_[_h.idx()] != NULL);
#ifdef NDEBUG
    return *static_cast  <PropertyT<T>*> (properties_[_h.idx()]);
#else
    PropertyT<T>* p = dynamic_cast<PropertyT<T>*>(properties_[_h.idx()]);
    assert(p != NULL);
    return *p;
#endif
  }


  template <class T> const PropertyT<T>& property(BasePropHandleT<T> _h) const
  {
    assert(_h.idx() >= 0 && _h.idx() < (int)properties_.size());
    assert(properties_[_h.idx()] != NULL);
#ifdef NDEBUG
    return *static_cast<PropertyT<T>*>(properties_[_h.idx()]);
#else
    PropertyT<T>* p = dynamic_cast<PropertyT<T>*>(properties_[_h.idx()]);
    assert(p != NULL);
    return *p;
#endif
  }


  template <class T> void remove(BasePropHandleT<T> _h)
  {
    assert(_h.idx() >= 0 && _h.idx() < (int)properties_.size());
    delete properties_[_h.idx()];
    properties_[_h.idx()] = NULL;
  }


  void clear()
  {
    std::for_each(properties_.begin(), properties_.end(), Delete());
  }


  //---------------------------------------------------- synchronize properties

  void reserve(size_t _n) const {
    std::for_each(properties_.begin(), properties_.end(), Reserve(_n));
  }

  void resize(size_t _n) const {
    std::for_each(properties_.begin(), properties_.end(), Resize(_n));
  }

  void free_mem() const {
    std::for_each(properties_.begin(), properties_.end(), FreeMem());
  }

  void swap(size_t _i0, size_t _i1) const {
    std::for_each(properties_.begin(), properties_.end(), Swap(_i0, _i1));
  }



protected: // generic add/get

  size_t _add( BaseProperty* _bp )
  {
    Properties::iterator p_it=properties_.begin(), p_end=properties_.end();
    size_t idx=0;
    for (; p_it!=p_end && *p_it!=NULL; ++p_it, ++idx);
    if (p_it==p_end) properties_.push_back(NULL);
    properties_[idx] = _bp;
    return idx;
  }

  BaseProperty& _property( size_t _idx )
  {
    assert( _idx < properties_.size());
    assert( properties_[_idx] != NULL);
    BaseProperty *p = properties_[_idx];
    assert( p != NULL );
    return *p;
  }

  const BaseProperty& _property( size_t _idx ) const
  {
    assert( _idx < properties_.size());
    assert( properties_[_idx] != NULL);
    BaseProperty *p = properties_[_idx];
    assert( p != NULL );
    return *p;
  }


  typedef Properties::iterator       iterator;
  typedef Properties::const_iterator const_iterator;
  iterator begin() { return properties_.begin(); }
  iterator end()   { return properties_.end(); }
  const_iterator begin() const { return properties_.begin(); }
  const_iterator end() const   { return properties_.end(); }

  friend class BaseKernel;

private:

  //-------------------------------------------------- synchronization functors

#ifndef DOXY_IGNORE_THIS

  struct Reserve
  {
    Reserve(size_t _n) : n_(_n) {}
    void operator()(BaseProperty* _p) const { if (_p) _p->reserve(n_); }
    size_t n_;
  };

  struct Resize
  {
    Resize(size_t _n) : n_(_n) {}
    void operator()(BaseProperty* _p) const { if (_p) _p->resize(n_); }
    size_t n_;
  };

  struct FreeMem
  {
    FreeMem() {}
    void operator()(BaseProperty* _p) const { if (_p) _p->free_mem(); }
  };

  struct Swap
  {
    Swap(size_t _i0, size_t _i1) : i0_(_i0), i1_(_i1) {}
    void operator()(BaseProperty* _p) const { if (_p) _p->swap(i0_, i1_); }
    size_t i0_, i1_;
  };

  struct Delete
  {
    Delete() {}
    void operator()(BaseProperty* _p) const { if (_p) delete _p; _p=NULL; }
  };

#endif

  Properties   properties_;
};



//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_PROPERTY_HH defined
//=============================================================================
