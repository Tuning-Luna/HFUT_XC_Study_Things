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


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/Utils/Endian.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/writer/OFFWriter.hh>


//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


// register the OFFLoader singleton with MeshLoader
_OFFWriter_  __OFFWriterInstance;
_OFFWriter_& OFFWriter() { return __OFFWriterInstance; }


//=== IMPLEMENTATION ==========================================================


_OFFWriter_::_OFFWriter_() { IOManager().register_module(this); }


//-----------------------------------------------------------------------------


bool
_OFFWriter_::
write(const std::string& _filename, BaseExporter& _be, Options _opt) const
{
  // check exporter features
  if ( !check( _be, _opt ) )
    return false;


  // check writer features
  if ( _opt.check(Options::VertexColor) || // not supported by module
       _opt.check(Options::FaceNormal)  || // not supported by format
       _opt.check(Options::FaceColor)  )   // not supported by module
    return false;
     

  // open file
  FILE* out = fopen(_filename.c_str(), "w");
  if (!out)
  {
    omerr() << "[OFFWriter] : cannot open file "
	  << _filename
	  << std::endl;
    return false;
  }


  // write header line
  if (_opt.check(Options::VertexTexCoord)) fprintf(out, "ST");
  if (_opt.check(Options::VertexColor))    fprintf(out, "C");
  if (_opt.check(Options::VertexNormal))   fprintf(out, "N");
  fprintf(out, "OFF");
  if (_opt.check(Options::Binary))         fprintf(out, " BINARY");
  fprintf(out, "\n");


  // write to file
  bool result = (_opt.check(Options::Binary) ?
		 write_binary(out, _be, _opt) :
		 write_ascii(out, _be, _opt));


  // return result
  fclose(out);
  return result;
}


//-----------------------------------------------------------------------------


bool
_OFFWriter_::
write_ascii(FILE* _out, BaseExporter& _be, Options _opt) const
{
  omlog() << "[OFFWriter] : write ascii file\n";


  unsigned int i, j, nV, nF;
  Vec3f v, n;
  Vec2f t;
  VertexHandle vh;
  std::vector<VertexHandle> vhandles;


  // #vertices, #faces
  fprintf(_out, "%d %d 0\n", (int)_be.n_vertices(), (int)_be.n_faces());


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
      fprintf(_out, "%.10f %.10f %.10f %.10f %.10f %.10f %f %f\n", 
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
      fprintf(_out, "%.10f %.10f %.10f %f %f\n", 
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
      fprintf(_out, "%.10f %.10f %.10f %.10f %.10f %.10f\n", 
	      v[0], v[1], v[2], n[0], n[1], n[2]);
    }
  }
  else
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      fprintf(_out, "%.10f %.10f %.10f\n", v[0], v[1], v[2]);
    }
  }




  // faces (indices starting at 0)
  if (_be.is_triangle_mesh())
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      _be.get_vhandles(FaceHandle(i), vhandles);
      fprintf(_out, "3 %d %d %d\n", 
	      vhandles[0].idx(),
	      vhandles[1].idx(), 
	      vhandles[2].idx());
    }
  }
  else
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      fprintf(_out, "%d", nV);
      for (j=0; j<vhandles.size(); ++j)
	fprintf(_out, " %d", vhandles[j].idx());
      fprintf(_out, "\n");
    }
  }


  return true;
}


//-----------------------------------------------------------------------------


bool 
_OFFWriter_::
write_binary(FILE* _out, BaseExporter& _be, Options _opt) const
{
  omlog() << "[OFFWriter] : write bindary file\n";


  unsigned int i, j, nV, nF;
  Vec3f v, n;
  Vec2f t;
  VertexHandle vh;
  std::vector<VertexHandle> vhandles;

  bool swap = 
    ( (_opt.check(Options::MSB) && Endian::local()==Endian::LSB) ||
      (_opt.check(Options::LSB) && Endian::local()==Endian::MSB) );
  
   
  // header: #vertices, #faces, #edges
  write_int(_be.n_vertices(), _out, swap);
  write_int(_be.n_faces(),    _out, swap);
  write_int(0,                _out, swap);


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
      write_float(v[0], _out, swap);
      write_float(v[1], _out, swap);
      write_float(v[2], _out, swap);
      write_float(n[0], _out, swap);
      write_float(n[1], _out, swap);
      write_float(n[2], _out, swap);
      write_float(t[0], _out, swap);
      write_float(t[1], _out, swap);
    }
  }
  else if (_opt.check(Options::VertexTexCoord))
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      t  = _be.texcoord(vh);
      write_float(v[0], _out, swap);
      write_float(v[1], _out, swap);
      write_float(v[2], _out, swap);
      write_float(t[0], _out, swap);
      write_float(t[1], _out, swap);
    }
  }
  else if (_opt.check(Options::VertexNormal))
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      n  = _be.normal(vh);
      write_float(v[0], _out, swap);
      write_float(v[1], _out, swap);
      write_float(v[2], _out, swap);
      write_float(n[0], _out, swap);
      write_float(n[1], _out, swap);
      write_float(n[2], _out, swap);
    }
  }
  else
  {
    for (i=0, nV=_be.n_vertices(); i<nV; ++i)
    {
      vh = VertexHandle(i);
      v  = _be.point(vh);
      write_float(v[0], _out, swap);
      write_float(v[1], _out, swap);
      write_float(v[2], _out, swap);
    }
  }


  // faces (indices starting at 0)
  if (_be.is_triangle_mesh())
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      _be.get_vhandles(FaceHandle(i), vhandles);
      write_int(3, _out, swap);
      write_int(vhandles[0].idx(), _out, swap);
      write_int(vhandles[1].idx(), _out, swap);
      write_int(vhandles[2].idx(), _out, swap);
    }
  }
  else
  {
    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      write_int(nV, _out, swap);
      for (j=0; j<vhandles.size(); ++j)
	write_int(vhandles[j].idx(), _out, swap);
    }
  }


  return true;
}

// ----------------------------------------------------------------------------


size_t
_OFFWriter_::
binary_size(BaseExporter& _be, Options _opt) const
{
  size_t header(0);
  size_t data(0);
  size_t _3longs(3*sizeof(long));
  size_t _3floats(3*sizeof(float));
  
  if ( !_opt.check(Options::Binary) )
    return 0;
  else
  {
    header += 11;                             // 'OFF BINARY\n'
    header += _3longs;                        // #V #F #E
    data   += _be.n_vertices() * _3floats;    // vertex data
  }

  if ( _opt.check(Options::VertexNormal) && _be.has_vertex_normals() )
  {
    header += 1; // N
    data   += _be.n_vertices() * _3floats;
  }

  if ( _opt.check(Options::VertexTexCoord) && _be.has_vertex_texcoords() )
  {
    size_t _2floats(2*sizeof(float));
    header += 2; // ST
    data   += _be.n_vertices() * _2floats;
  }
   

  // topology
  if (_be.is_triangle_mesh())
  {
    size_t _4ui(4*sizeof(unsigned int));                  
    data += _be.n_faces() * _4ui;
  }
  else
  {
    unsigned int i, nV, nF;
    std::vector<VertexHandle> vhandles;

    for (i=0, nF=_be.n_faces(); i<nF; ++i)
    {
      nV = _be.get_vhandles(FaceHandle(i), vhandles);
      data += nV * sizeof(unsigned int);
    }
  }
   
  return header+data;
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
