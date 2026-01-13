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
//  Implements an reader module for OBJ files
//
//=============================================================================


#ifndef __OBJREADER_HH__
#define __OBJREADER_HH__


//=== INCLUDES ================================================================


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/importer/BaseImporter.hh>
#include <OpenMesh/Core/IO/reader/BaseReader.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//== IMPLEMENTATION ===========================================================


/** 
    Implementation of the OBJ format reader.
*/
class _OBJReader_ : public BaseReader
{
public:

  _OBJReader_();

  virtual ~_OBJReader_() { }

  std::string get_description() const { return "Alias/Wavefront"; }
  std::string get_extensions()  const { return "obj"; }
  
  bool read(const std::string& _filename, 
	    BaseImporter& _bi, 
	    Options& _opt);

private:

#ifndef DOXY_IGNORE_THIS
  class Material
  {
  public:

    Material() { cleanup(); }

    void cleanup()
    {
      Kd_is_set_ = false;
      Ka_is_set_ = false;
      Ks_is_set_ = false;
      Tr_is_set_ = false;
    }

    bool is_valid(void) const 
    { return Kd_is_set_ || Ka_is_set_ || Ks_is_set_ || Tr_is_set_; }

    bool has_Kd(void) { return Kd_is_set_; }
    bool has_Ka(void) { return Ka_is_set_; }
    bool has_Ks(void) { return Ks_is_set_; }
    bool has_Tr(void) { return Tr_is_set_; }
      
    void set_Kd( float r, float g, float b ) 
    { Kd_=Vec3f(r,g,b); Kd_is_set_=true; }

    void set_Ka( float r, float g, float b ) 
    { Ka_=Vec3f(r,g,b); Ka_is_set_=true; }

    void set_Ks( float r, float g, float b ) 
    { Ks_=Vec3f(r,g,b); Ks_is_set_=true; }
    
    void set_Tr( float t )
    { Tr_=t;            Tr_is_set_=true; }
  
    const Vec3f& Kd( void ) const { return Kd_; }
    const Vec3f& Ka( void ) const { return Ka_; }
    const Vec3f& Ks( void ) const { return Ks_; }
    float  Tr( void ) const { return Tr_; }
  
  private:

    Vec3f Kd_;         bool Kd_is_set_; // diffuse
    Vec3f Ka_;         bool Ka_is_set_; // ambient
    Vec3f Ks_;         bool Ks_is_set_; // specular
    float Tr_;         bool Tr_is_set_; // transperency
  };
#endif

  typedef std::map<std::string, Material> MaterialList;

  MaterialList materials_;

  bool read_mtl( FILE* _in );

private:

  bool read(FILE* _in, BaseImporter& _bi, Options& _opt);

  std::string path_;

};


//== TYPE DEFINITION ==========================================================


extern _OBJReader_  __OBJReaderInstance;
_OBJReader_& OBJReader();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
