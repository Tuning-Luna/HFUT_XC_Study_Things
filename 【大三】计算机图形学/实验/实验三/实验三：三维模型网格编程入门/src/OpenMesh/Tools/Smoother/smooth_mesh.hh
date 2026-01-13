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
//   $Date: 2004/09/06 12:37:09 $
//
//=============================================================================

#ifndef SMOOTH_MESH_HH
#define SMOOTH_MESH_HH

//== INCLUDES =================================================================

#include <OpenMesh/Core/Utils/Property.hh>

//== NAMESPACE ================================================================

namespace OpenMesh { //BEGIN_NS_OPENMESH

template <class _Mesh, class _PropertyHandle>
void smooth_mesh_property(unsigned int _n_iters, _Mesh& _m, _PropertyHandle _pph)
{
  typedef typename _PropertyHandle::Value   Value;

  std::vector<Value> temp_values(_m.n_vertices());

  for (unsigned int i=0; i < _n_iters; ++i)
  {
    for ( typename _Mesh::ConstVertexIter cv_it = _m.vertices_begin();
          cv_it != _m.vertices_end(); ++cv_it)
    {
      unsigned int valence = 0;

      Value& temp_value = temp_values[cv_it.handle().idx()];

      temp_value.vectorize(0);

      for ( typename _Mesh::ConstVertexVertexIter cvv_it = _m.cvv_iter(cv_it);
            cvv_it; ++cvv_it)
      {
        temp_value += _m.property(_pph,cvv_it);
        ++valence;
      }
      if (valence > 0)
      {//guard against isolated vertices
        temp_value *= (1.0 / valence);
      }
      else
      {
        temp_value = _m.property(_pph, cv_it);
      }
    }

    for ( typename _Mesh::ConstVertexIter cv_it = _m.vertices_begin();
          cv_it != _m.vertices_end(); ++cv_it)
    {
      _m.property(_pph,cv_it) = temp_values[cv_it.handle().idx()];
    }
  }
}

template <class _Mesh>
void smooth_mesh(_Mesh& _m, uint _n_iters)
{
  smooth_mesh_property(_n_iters, _m, _m.points_pph());
}

};//namespace OpenMesh

#endif//SMOOTH_MESH_HH
