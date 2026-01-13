/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2004 by Computer Graphics Group, RWTH Aachen      *
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

#ifndef OPENMESH_AutoPropertyHandleT_HH
#define OPENMESH_AutoPropertyHandleT_HH

//== INCLUDES =================================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {

//== CLASS DEFINITION =========================================================

template <class Mesh_, class PropertyHandle_>
class AutoPropertyHandleT : public PropertyHandle_
{
public:

  typedef Mesh_                             Mesh;
  typedef PropertyHandle_                   PropertyHandle;
  typedef PropertyHandle                    Base;
  typedef typename PropertyHandle::Value    Value;
  typedef AutoPropertyHandleT<Mesh, PropertyHandle>
                                            Self;
protected:

  Mesh&                                     m_;
  bool                                      own_property_;//ref counting?

public:

  explicit AutoPropertyHandleT(Mesh& _m, const std::string& _pp_name = std::string())
  : m_(_m)
  {
    own_property_ = _pp_name.empty() || !m_.get_property_handle(*this, _pp_name);
    if (own_property_)
    {
      m_.add_property(*this, _pp_name);
    }
  }

  AutoPropertyHandleT(Mesh& _m, PropertyHandle _pph)
  : Base(_pph.idx()), m_(_m), own_property_(false)
  {}

  AutoPropertyHandleT(const Self& _other)
  : Base(_other.idx()), m_(_other.m_), own_property_(false)
  {}

  ~AutoPropertyHandleT()
  {
    if (own_property_)
    {
      m_.remove_property(*this);
    }
  }

  template <class _Handle>
  inline Value&                             operator [] (_Handle _hnd)
  {
    return m_.property(*this, _hnd);
  }

  template <class _Handle>
  inline const Value&                       operator [] (_Handle _hnd) const
  {
    return m_.property(*this, _hnd);
  }

  inline bool                               own_property() const
  {
    return own_property_;
  }

  inline void                               free_property()
  {
    own_property_ = false;
  }

  inline void                               delete_property()
  {
    assert(own_property_);//only the owner can delete the property
    m_.remove_property(*this);
    own_property_ = false;
    invalidate();
  }
};

//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_AutoPropertyHandleT_HH defined
//=============================================================================
