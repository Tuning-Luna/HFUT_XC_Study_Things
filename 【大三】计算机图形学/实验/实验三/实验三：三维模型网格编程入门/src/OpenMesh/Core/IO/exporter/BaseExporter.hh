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
//  Implements the baseclass for MeshWriter exporter modules
//
//=============================================================================


#ifndef __BASEEXPORTER_HH__
#define __BASEEXPORTER_HH__


//=== INCLUDES ================================================================


// STL
#include <vector>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh>


//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== EXPORTER ================================================================


/**
   Base class for exporter modules.
   The exporter modules provide an interface between the writer modules and
   the target data structure. 
*/

class BaseExporter
{
public:

  virtual ~BaseExporter() { }
   

  // get vertex data
  virtual Vec3f  point(VertexHandle _vh)    const = 0;
  virtual Vec3f  normal(VertexHandle _vh)   const = 0;
  virtual Vec3uc color(VertexHandle _vh)    const = 0;
  virtual Vec2f  texcoord(VertexHandle _vh) const = 0;

  
  // get face data
  virtual unsigned int 
  get_vhandles(FaceHandle _fh, 
	       std::vector<VertexHandle>& _vhandles) const=0;
  virtual Vec3f  normal(FaceHandle _fh)     const = 0;
  virtual Vec3uc color(FaceHandle _fh)      const = 0;
  

  // get reference to base kernel
  virtual const BaseKernel* kernel() { return 0; }


  // query number of faces, vertices, normals, texcoords
  virtual size_t n_vertices()   const = 0;
  virtual size_t n_faces()      const = 0;
  virtual size_t n_edges()      const = 0;


  // property information
  virtual bool is_triangle_mesh()     const { return false; }
  virtual bool has_vertex_normals()   const { return false; }
  virtual bool has_vertex_colors()    const { return false; }
  virtual bool has_vertex_texcoords() const { return false; }      
  virtual bool has_face_normals()     const { return false; }
  virtual bool has_face_colors()      const { return false; }
};


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
