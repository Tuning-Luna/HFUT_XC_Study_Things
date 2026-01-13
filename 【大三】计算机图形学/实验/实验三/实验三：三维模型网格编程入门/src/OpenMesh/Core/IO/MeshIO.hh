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


#ifndef OM_MESHIO_HH
#define OM_MESHIO_HH


//=== INCLUDES ================================================================

// -------------------- system settings
#include <OpenMesh/Core/System/config.hh>
// -------------------- check include order
#if defined (OPENMESH_TRIMESH_ARRAY_KERNEL_HH) || \
    defined (OPENMESH_POLYMESH_ARRAY_KERNEL_HH)
#  error "Include MeshIO.hh before including a mesh type!"
#endif
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/SR_store.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/IO/importer/ImporterT.hh>
#include <OpenMesh/Core/IO/exporter/ExporterT.hh>


//== NAMESPACES ==============================================================

namespace OpenMesh {
namespace IO   {


//=== IMPLEMENTATION ==========================================================


/** \name Mesh Reading / Writing
    Convenience functions the map to IOManager functions.
    \see OpenMesh::IO::_IOManager_
*/
//@{


//-----------------------------------------------------------------------------


/** Read a mesh from file _filename. The file format is determined by
    the file extension. */
template <class Mesh>
bool 
read_mesh(Mesh&               _mesh, 
	  const std::string&  _filename, 
	  bool                _clear = true) 
{
  Options opt;
  return read_mesh(_mesh, _filename, opt, _clear);
}


/** Read a mesh from file _filename. The file format is determined by
    the file extension. */
template <class Mesh>
bool 
read_mesh(Mesh&               _mesh, 
	  const std::string&  _filename, 
	  Options&            _opt, 
	  bool                _clear = true) 
{
  if (_clear) _mesh.clear();
  ImporterT<Mesh> importer(_mesh);
  return IOManager().read(_filename, importer, _opt); 
}


//-----------------------------------------------------------------------------


/** Write a mesh to the file _filename. The file format is determined
    by _filename's extension. */
template <class Mesh>
bool write_mesh(const Mesh& _mesh, const std::string& _filename,
                Options _opt = Options::Default) 
{ 
  ExporterT<Mesh> exporter(_mesh);
  return IOManager().write(_filename, exporter, _opt); 
}


//-----------------------------------------------------------------------------


template <class Mesh>
size_t binary_size(const Mesh& _mesh, const std::string& _format,
                   Options _opt = Options::Default)
{
  ExporterT<Mesh> exporter(_mesh);
  return IOManager().binary_size(_format, exporter, _opt);
}


//-----------------------------------------------------------------------------

//@}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#if defined(OM_STATIC_BUILD) || defined(ARCH_DARWIN)
#  include <OpenMesh/Core/IO/IOInstances.hh>
#endif
//=============================================================================
#endif
//=============================================================================
