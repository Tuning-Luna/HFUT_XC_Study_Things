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


//=============================================================================
//
//  Implements an exporter module for arbitrary OpenMesh meshes
//
//=============================================================================


#ifndef __EXPORTERT_HH__
#define __EXPORTERT_HH__


//=== INCLUDES ================================================================

// C++
#include <vector>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/color_cast.hh>
#include <OpenMesh/Core/IO/exporter/BaseExporter.hh>


//=== NAMESPACES ==============================================================

namespace OpenMesh {
namespace IO {


//=== EXPORTER CLASS ==========================================================

/**
 *  This class template provides an exporter module for OpenMesh meshes.
 */
template <class Mesh>
class ExporterT : public BaseExporter
{
public:

  // Constructor
  ExporterT(const Mesh& _mesh) : mesh_(_mesh) {}
   

  // get vertex data

  Vec3f  point(VertexHandle _vh)    const 
  { 
    return vector_cast<Vec3f>(mesh_.point(_vh)); 
  }

  Vec3f  normal(VertexHandle _vh)   const 
  { 
    return (mesh_.has_vertex_normals() 
	    ? vector_cast<Vec3f>(mesh_.normal(_vh)) 
	    : Vec3f(0.0f, 0.0f, 0.0f));
  }

  Vec3uc color(VertexHandle _vh)    const
  {
    return (mesh_.has_vertex_colors() 
	    ? color_cast<Vec3uc>(mesh_.color(_vh)) 
	    : Vec3uc(0, 0, 0));
  }

  Vec2f  texcoord(VertexHandle _vh) const
  {
#if defined(OM_CC_GCC) && (OM_CC_VERSION<30000)
    // Workaround! 
    // gcc 2.95.3 exits with internal compiler error at the
    // code below!??? **)
    if (mesh_.has_vertex_texcoords2D())
      return vector_cast<Vec2f>(mesh_.texcoord2D(_vh));
    return Vec2f(0.0f, 0.0f);
#else // **)
    return (mesh_.has_vertex_texcoords2D() 
	    ? vector_cast<Vec2f>(mesh_.texcoord2D(_vh)) 
	    : Vec2f(0.0f, 0.0f));
#endif
  }

  
  // get face data

  unsigned int get_vhandles(FaceHandle _fh, 
			    std::vector<VertexHandle>& _vhandles) const
  {
    unsigned int count(0);
    _vhandles.clear();
    for (typename Mesh::CFVIter fv_it=mesh_.cfv_iter(_fh); fv_it; ++fv_it)
    {
      _vhandles.push_back(fv_it.handle());
      ++count;
    }
    return count;
  }

  Vec3f  normal(FaceHandle _fh)   const 
  { 
    return (mesh_.has_face_normals() 
            ? vector_cast<Vec3f>(mesh_.normal(_fh)) 
            : Vec3f(0.0f, 0.0f, 0.0f));
  }

  Vec3uc  color(FaceHandle _fh)   const 
  { 
    return (mesh_.has_face_colors() 
            ? vector_cast<Vec3uc>(mesh_.color(_fh)) 
            : Vec3uc(0, 0, 0));
  }

  virtual const BaseKernel* kernel() { return &mesh_; }


  // query number of faces, vertices, normals, texcoords
  size_t n_vertices()  const { return mesh_.n_vertices(); }   
  size_t n_faces()     const { return mesh_.n_faces(); }
  size_t n_edges()     const { return mesh_.n_edges(); }


  // property information
  bool is_triangle_mesh() const
  {
    typedef typename Mesh::Face Face; // must use typedef for gcc 2.95.x
    return Face::is_triangle();
  }
  bool has_vertex_normals()   const { return mesh_.has_vertex_normals();   }
  bool has_vertex_colors()    const { return mesh_.has_vertex_colors();    }
  bool has_vertex_texcoords() const { return mesh_.has_vertex_texcoords2D(); }
  bool has_face_normals()     const { return mesh_.has_face_normals();     }
  bool has_face_colors()      const { return mesh_.has_face_colors();      }

private:
  
   const Mesh& mesh_;
};


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
