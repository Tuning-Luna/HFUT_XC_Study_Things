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


//== INCLUDES =================================================================


//STL
#include <fstream>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/writer/OBJWriter.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/System/omstream.hh>


//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


// register the OBJLoader singleton with MeshLoader
_OBJWriter_  __OBJWriterinstance;
_OBJWriter_& OBJWriter() { return __OBJWriterinstance; }


//=== IMPLEMENTATION ==========================================================


_OBJWriter_::_OBJWriter_() { IOManager().register_module(this); }


//-----------------------------------------------------------------------------


bool 
_OBJWriter_::
write(const std::string& _filename, BaseExporter& _be, Options _opt) const
{
  FILE* out = fopen(_filename.c_str(), "w");
  if (!out)
  {
    omerr() << "[OBJWriter] : cannot open file "
	  << _filename 
	  << std::endl;
    return false;
  }

  bool result = write(out, _be, _opt);
  
  fclose(out);
  return result;
}


//-----------------------------------------------------------------------------


bool
_OBJWriter_::
write(FILE* _out, BaseExporter& _be, Options _opt) const
{
  unsigned int i, j, nV, nF, idx;
  Vec3f v, n;
  Vec2f t;
  VertexHandle vh;
  std::vector<VertexHandle> vhandles;


  omlog() << "[OBJWriter] : write file\n";


  // check exporter features
  if (!check( _be, _opt))
     return false;


  // check writer features
  if ( _opt.check(Options::Binary)     || // not supported by format
       _opt.check(Options::FaceNormal) || // ?
       _opt.check(Options::FaceColor)  )  // ?
     return false;
  

  // header
  fprintf(_out, "# %d vertices, %d faces\n", 
	  (int)_be.n_vertices(), (int)_be.n_faces());


  // vertex data (point, normals, texcoords)
  if (_opt.check(Options::VertexTexCoord) && 
      _opt.check(Options::VertexNormal))
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      n  = _be.normal(vh);
      t  = _be.texcoord(vh);
      fprintf(_out, "v %.10f %.10f %.10f\nvn %.10f %.10f %.10f\nvt %f %f\n", 
	      v[0], v[1], v[2], n[0], n[1], n[2], t[0], t[1]);
    }
  }
  else if (_opt.check(Options::VertexTexCoord))
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      t  = _be.texcoord(vh);
      fprintf(_out, "v %.10f %.10f %.10f\nvt %.10f %.10f\n", 
	      v[0], v[1], v[2], t[0], t[1]);
    }
  }
  else if (_opt.check(Options::VertexNormal))
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      n  = _be.normal(vh);
      fprintf(_out, "v %.10f %.10f %.10f\nvn %.10f %.10f %.10f\n", 
	      v[0], v[1], v[2], n[0], n[1], n[2]);
    }
  }
  else
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      fprintf(_out, "v %.10f %.10f %.10f\n", v[0], v[1], v[2]);
    }
  }




  // faces (indices starting at 1 not 0)
  if (_opt.check(Options::VertexTexCoord) && 
      _opt.check(Options::VertexNormal))
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      fprintf(_out, "f");
      for (j=0; j<vhandles.size(); ++j)
      {
	idx = vhandles[j].idx() + 1;
	fprintf(_out, " %d/%d/%d", idx, idx, idx);
      }
      fprintf(_out, "\n");
    }
  }
  else if (_opt.check(Options::VertexTexCoord))
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      fprintf(_out, "f");
      for (j=0; j<vhandles.size(); ++j)
      {
	idx = vhandles[j].idx() + 1;
	fprintf(_out, " %d/%d/", idx, idx);
      }
      fprintf(_out, "\n");
    }
  }
  else if (_opt.check(Options::VertexNormal))
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      fprintf(_out, "f");
      for (j=0; j<vhandles.size(); ++j)
      {
	idx = vhandles[j].idx() + 1;
	fprintf(_out, " %d//%d", idx, idx);
      }
      fprintf(_out, "\n");
    }
  }
  else
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      fprintf(_out, "f");
      for (j=0; j<vhandles.size(); ++j)
	fprintf(_out, " %d", vhandles[j].idx() + 1);
      fprintf(_out, "\n");
    }
  }


  return true;
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
