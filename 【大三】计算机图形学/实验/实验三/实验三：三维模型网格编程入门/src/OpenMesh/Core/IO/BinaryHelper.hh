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

#ifndef OPENMESH_BINARY_HELPER_HH
#define OPENMESH_BINARY_HELPER_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
// -------------------- STL
#if defined( OM_CC_MIPS )
#  include <stdio.h>
#else
#  include <cstdio>
#endif
#include <iostream>
// -------------------- OpenMesh


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace IO {


//=============================================================================


/** \name Handling binary input/output.
    These functions take care of swapping bytes to get the right Endian.
*/
//@{

//-----------------------------------------------------------------------------


/** Binary read a \c short from \c _is and perform byte swapping if
    \c _swap is true */
short int read_short(FILE* _in, bool _swap=false);

/** Binary read an \c int from \c _is and perform byte swapping if
    \c _swap is true */
int read_int(FILE* _in, bool _swap=false);

/** Binary read a \c float from \c _is and perform byte swapping if
    \c _swap is true */
float read_float(FILE* _in, bool _swap=false);

/** Binary read a \c double from \c _is and perform byte swapping if
    \c _swap is true */
double read_double(FILE* _in, bool _swap=false);


/** Binary write a \c short to \c _os and perform byte swapping if
    \c _swap is true */
void write_short(short int _i, FILE* _out, bool _swap=false);

/** Binary write an \c int to \c _os and perform byte swapping if
    \c _swap is true */
void write_int(int _i, FILE* _out, bool _swap=false);

/** Binary write a \c float to \c _os and perform byte swapping if
    \c _swap is true */
void write_float(float _f, FILE* _out, bool _swap=false);

/** Binary write a \c double to \c _os and perform byte swapping if
    \c _swap is true */
void write_double(double _d, FILE* _out, bool _swap=false);

   
//@}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_MESHREADER_HH defined
//=============================================================================

