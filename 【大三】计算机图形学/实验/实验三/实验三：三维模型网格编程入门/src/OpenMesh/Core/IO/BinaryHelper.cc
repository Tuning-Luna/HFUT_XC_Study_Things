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
//   $Date: 2004/09/06 12:35:05 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
// -------------------- STL
#include <algorithm>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/BinaryHelper.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {

#ifndef DOXY_IGNORE_THIS

//== IMPLEMENTATION ===========================================================


union ShortChar   { short int s;  unsigned char c[2]; }  sc;
union IntChar     { int i;        unsigned char c[4]; }  ic;
union FloatChar   { float f;      unsigned char c[4]; }  fc;
union DoubleChar  { double d;     unsigned char c[8]; }  dc;


//-----------------------------------------------------------------------------

short int read_short(FILE* _in, bool _swap) 
{
  fread((char*)sc.c, 1, 2, _in);
  if (_swap) std::swap(sc.c[0], sc.c[1]);
  return sc.s;
}


//-----------------------------------------------------------------------------


int read_int(FILE* _in, bool _swap) 
{
  fread((char*)ic.c, 1, 4, _in);
  if (_swap) {
    std::swap(ic.c[0], ic.c[3]);
    std::swap(ic.c[1], ic.c[2]);
  }
  return ic.i;
}


//-----------------------------------------------------------------------------


float read_float(FILE* _in, bool _swap) 
{
  fread((char*)fc.c, 1, 4, _in);
  if (_swap) {
    std::swap(fc.c[0], fc.c[3]);
    std::swap(fc.c[1], fc.c[2]);
  }
  return fc.f;
}


//-----------------------------------------------------------------------------


double read_double(FILE* _in, bool _swap) 
{
  fread((char*)dc.c, 1, 8, _in);
  if (_swap) {
    std::swap(dc.c[0], dc.c[7]);
    std::swap(dc.c[1], dc.c[6]);
    std::swap(dc.c[2], dc.c[5]);
    std::swap(dc.c[3], dc.c[4]);
  }
  return dc.d;
}


//-----------------------------------------------------------------------------


void write_short(short int _i, FILE* _out, bool _swap) 
{
  sc.s = _i;
  if (_swap) std::swap(sc.c[0], sc.c[1]);
  fwrite((char*)sc.c, 1, 2, _out);
}


//-----------------------------------------------------------------------------


void write_int(int _i, FILE* _out, bool _swap) 
{
  ic.i = _i;
  if (_swap) {
    std::swap(ic.c[0], ic.c[3]);
    std::swap(ic.c[1], ic.c[2]);
  }
  fwrite((char*)ic.c, 1, 4, _out);
}


//-----------------------------------------------------------------------------


void write_float(float _f, FILE* _out, bool _swap) 
{
  fc.f = _f;
  if (_swap) {
    std::swap(fc.c[0], fc.c[3]);
    std::swap(fc.c[1], fc.c[2]);
  }
  fwrite((char*)fc.c, 1, 4, _out);
}


//-----------------------------------------------------------------------------


void write_double(double _d, FILE* _out, bool _swap) 
{
  dc.d = _d;
  if (_swap) {
    std::swap(dc.c[0], dc.c[7]);
    std::swap(dc.c[1], dc.c[6]);
    std::swap(dc.c[2], dc.c[5]);
    std::swap(dc.c[3], dc.c[4]);
  }
  fwrite((char*)dc.c, 1, 8, _out);
}

#endif

//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
