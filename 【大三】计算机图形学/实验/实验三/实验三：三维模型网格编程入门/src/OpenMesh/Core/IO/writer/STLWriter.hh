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
//  Implements a writer module for STL ascii files
//
//=============================================================================
// $Id: STLWriter.hh,v 1.2 2005/02/18 09:08:21 botsch Exp $

#ifndef __STLWRITER_HH__
#define __STLWRITER_HH__


//=== INCLUDES ================================================================

// -------------------- STL
#if defined( OM_CC_MIPS )
#  include <stdio.h>
#else
#  include <cstdio>
#endif
#include <string>
// -------------------- OpenMesh
#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/exporter/BaseExporter.hh>
#include <OpenMesh/Core/IO/writer/BaseWriter.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


/** 
    Implementation of the STL format writer. This class is singleton'ed by 
    SingletonT to STLWriter.
*/
class _STLWriter_ : public BaseWriter
{
public:
  
  _STLWriter_();
  
  std::string get_description() const { return "Stereolithography Format"; }
  std::string get_extensions()  const { return "stl stla stlb"; }
  
  bool write(const std::string&, BaseExporter&, Options) const;
  
  size_t binary_size(BaseExporter&, Options) const;

private:
  bool write_stla(const std::string&, BaseExporter&, Options) const;
  bool write_stlb(const std::string&, BaseExporter&, Options) const;
};


//== TYPE DEFINITION ==========================================================


// Declare the single entity of STL writer.
extern _STLWriter_  __STLWriterInstance;
_STLWriter_& STLWriter();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
