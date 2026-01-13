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


// STL
#include <map>

#include <float.h>

// OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/reader/STLReader.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/IO/importer/BaseImporter.hh>


//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


// register the STLReader singleton with MeshReader
_STLReader_  __STLReaderInstance;
_STLReader_&  STLReader() { return __STLReaderInstance; }


//=== IMPLEMENTATION ==========================================================


_STLReader_::
_STLReader_() 
  : eps_(FLT_MIN)
{
  IOManager().register_module(this); 
}


//-----------------------------------------------------------------------------


bool 
_STLReader_::
read(const std::string& _filename, BaseImporter& _bi, Options& _opt)
{
  bool result = false;

  STL_Type file_type = NONE;

  if ( check_extension( _filename, "stla" ) )
  {
    file_type = STLA;
  }

  else if ( check_extension( _filename, "stlb" ) )
  {
    file_type = STLB;
  }

  else if ( check_extension( _filename, "stl" ) )
  {
    file_type = check_stl_type(_filename);
  }


  switch (file_type)
  {
    case STLA:
    {
      result = read_stla(_filename, _bi);
      _opt -= Options::Binary;
      break;
    }
    
    case STLB:
    {
      result = read_stlb(_filename, _bi);
      _opt += Options::Binary;
      break;
    }
    
    default: 
    {
      result = false;
      break;
    }
  }


  return result;
}


//-----------------------------------------------------------------------------


#ifndef DOXY_IGNORE_THIS

class CmpVec
{
public:

  CmpVec(float _eps=FLT_MIN) : eps_(_eps) {}

  bool operator()( const Vec3f& _v0, const Vec3f& _v1 ) const
  {
    if (fabs(_v0[0] - _v1[0]) <= eps_) 
    {
      if (fabs(_v0[1] - _v1[1]) <= eps_) 
      {
	return (_v0[2] < _v1[2] - eps_);
      }
      else return (_v0[1] < _v1[1] - eps_);
    }
    else return (_v0[0] < _v1[0] - eps_);
  }

private:
  float eps_;
};

#endif


//-----------------------------------------------------------------------------


bool 
_STLReader_::
read_stla(const std::string& _filename, BaseImporter& _bi) const
{
  omlog() << "[STLReader] : read ascii file\n";


  FILE*  in = fopen(_filename.c_str(), "r");
  if (!in)
  {
    omerr() << "[STLReader] : cannot not open file " 
	  << _filename
	  << std::endl;
    return false;
  }



  char                       line[100], *p;
  unsigned int               i;
  OpenMesh::Vec3f            v;
  VertexHandle               vh;
  BaseImporter::VHandles     vhandles, face_vhandles;

  CmpVec comp(eps_);
  std::map<Vec3f, VertexHandle, CmpVec>            vMap(comp);
  std::map<Vec3f, VertexHandle, CmpVec>::iterator  vMapIt;

  
  while (in && !feof(in) && fgets(line, 100, in))
  {
    for (p=line; isspace(*p) && *p!='\0'; ++p); // skip white-space

    if ((strncmp(p, "outer", 5) == 0) || (strncmp(p, "OUTER", 5) == 0))
    { 
      face_vhandles.clear();
      
      for (i=0; i<3; ++i)
      {
 	fgets(line, 100, in);
	for (p=line; isspace(*p) && *p!='\0'; ++p); // skip white-space
 	sscanf(p+6, "%f %f %f", &v[0], &v[1], &v[2]);

	// has vector been referenced before?
	if ((vMapIt=vMap.find(v)) == vMap.end())
	{
	  // No : add vertex and remember idx/vector mapping
	  vh = _bi.add_vertex(v);
	  face_vhandles.push_back(vh);
	  vMap[v] = vh;
	}
	else 
	  // Yes : get index from map
	  face_vhandles.push_back(vMapIt->second);
      }

      // Add face only if it is not degenerated
      if ((face_vhandles[0] != face_vhandles[1]) &&
	  (face_vhandles[0] != face_vhandles[2]) &&
	  (face_vhandles[1] != face_vhandles[2]))
	_bi.add_face(face_vhandles);
    }
  }


  fclose(in);


  // In general a file has data, there the number of vertices cannot be 0.
  return _bi.n_vertices() != 0;
}


//-----------------------------------------------------------------------------


bool 
_STLReader_::
read_stlb(const std::string& _filename, BaseImporter& _bi) const
{
  omlog() << "[STLReader] : read binary file\n";


  FILE*  in = fopen(_filename.c_str(), "rb");
  if (!in)
  {
    omerr() << "[STLReader] : cannot not open file " 
	  << _filename
	  << std::endl;
    return false;
  }


  char                       dummy[100];
  bool                       swapFlag;
  unsigned int               i, nT;
  OpenMesh::Vec3f            v;
  VertexHandle               vh;
  BaseImporter::VHandles     vhandles, face_vhandles;

  std::map<Vec3f, VertexHandle, CmpVec>  vMap;
  std::map<Vec3f, VertexHandle, CmpVec>::iterator vMapIt;

 
  // check size of types
  if ((sizeof(float) != 4) || (sizeof(int) != 4)) {
    omerr() << "[STLReader] : wrong type size\n";
    fclose(in);
    return false;
  }

  // determine endian mode
  union { unsigned int i; unsigned char c[4]; } endian_test;
  endian_test.i = 1;
  swapFlag = (endian_test.c[3] == 1);
    
  // read number of triangles
  fread(dummy, 1, 80, in);
  nT = read_int(in, swapFlag);
        
  // read triangles
  while (nT)
  {
    face_vhandles.clear();

    // skip triangle normal
    fread(dummy, 1, 12, in);

    // triangle's vertices
    for (i=0; i<3; ++i)
    {
      v[0] = read_float(in, swapFlag);
      v[1] = read_float(in, swapFlag);
      v[2] = read_float(in, swapFlag);

      // has vector been referenced before?
      if ((vMapIt=vMap.find(v)) == vMap.end())
      {
	// No : add vertex and remember idx/vector mapping
	vh = _bi.add_vertex(v);
	face_vhandles.push_back(vh);
	vMap[v] = vh;
      }
      else 
	// Yes : get index from map
	face_vhandles.push_back(vMapIt->second);
    }
    

    // Add face only if it is not degenerated
    if ((face_vhandles[0] != face_vhandles[1]) &&
	(face_vhandles[0] != face_vhandles[2]) &&
	(face_vhandles[1] != face_vhandles[2]))
      _bi.add_face(face_vhandles);
    
    fread(dummy, 1, 2, in);
    --nT;
  }


  fclose(in);

  return true;
}


//-----------------------------------------------------------------------------


_STLReader_::STL_Type
_STLReader_::
check_stl_type(const std::string& _filename) const
{
  // assume it's binary stl, then file size is known from #triangles
  // if size matches, it's really binary


  // open file
  FILE* in = fopen(_filename.c_str(), "rb");
  if (!in) return NONE;


  // determine endian mode
  union { unsigned int i; unsigned char c[4]; } endian_test;
  endian_test.i = 1;
  bool swapFlag = (endian_test.c[3] == 1);

    
  // read number of triangles
  char dummy[100];
  fread(dummy, 1, 80, in);
  unsigned int nT = read_int(in, swapFlag);


  // compute file size from nT
  unsigned int binary_size = 84 + nT*50;


  // get actual file size
  unsigned int file_size(0);
  rewind(in);
  while (!feof(in))
    file_size += fread(dummy, 1, 100, in);
  fclose(in);


  // if sizes match -> it's STLB
  return (binary_size == file_size ? STLB : STLA);
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
