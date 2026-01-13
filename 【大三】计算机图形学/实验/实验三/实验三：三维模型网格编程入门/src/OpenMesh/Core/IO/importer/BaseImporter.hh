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
//  Implements the baseclass for IOManager importer modules
//
//=============================================================================


#ifndef __BASEIMPORTER_HH__
#define __BASEIMPORTER_HH__


//=== INCLUDES ================================================================


// STL
#include <vector>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


/**  Base class for importer modules. Importer modules provide an
 *   interface between the loader modules and the target data
 *   structure. This is basically a wrapper providing virtual versions
 *   for the required mesh functions.
 */
class BaseImporter
{
public:

  // base class needs virtual destructor
  virtual ~BaseImporter() {}


  // add a vertex with coordinate \c _point
  virtual VertexHandle add_vertex(const Vec3f& _point) = 0;
   
  // add a face with indices _indices refering to vertices
  typedef std::vector<VertexHandle> VHandles;
  virtual FaceHandle add_face(const VHandles& _indices) = 0;

  // set vertex normal
  virtual void set_normal(VertexHandle _vh, const Vec3f& _normal) = 0;

  // set vertex color
  virtual void set_color(VertexHandle _vh, const Vec3uc& _color) = 0;

  // set vertex texture coordinate
  virtual void set_texcoord(VertexHandle _vh, const Vec2f& _texcoord) = 0;

  // set face normal
  virtual void set_normal(FaceHandle _fh, const Vec3f& _normal) = 0;

  // set face color
  virtual void set_color(FaceHandle _fh, const Vec3uc& _color) = 0;

  // get reference to base kernel
  virtual BaseKernel* kernel() { return 0; }

  virtual bool is_triangle_mesh()     const { return false; }

  // reserve mem for elements
  void reserve(unsigned int nV, unsigned int nE, unsigned int nF) {}

  // query number of faces, vertices, normals, texcoords
  virtual size_t n_vertices()   const = 0;
  virtual size_t n_faces()      const = 0;
  virtual size_t n_edges()      const = 0;


  // pre-processing
  virtual void prepare()  {}

  // post-processing
  virtual void finish()  {}
};


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
