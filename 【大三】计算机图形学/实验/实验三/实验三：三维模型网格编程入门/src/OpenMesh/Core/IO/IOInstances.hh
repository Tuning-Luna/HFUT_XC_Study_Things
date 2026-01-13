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
//  Helper file for static builds
//
//  In opposite to dynamic builds where the instance of every reader module
//  is generated within the OpenMesh library, static builds only instanciate
//  objects that are at least referenced once. As all reader modules are
//  never used directly, they will not be part of a static build, hence 
//  this file.
//
//=============================================================================


#ifndef __IOINSTANCES_HH__
#define __IOINSTANCES_HH__

#if defined(OM_STATIC_BUILD) || defined(ARCH_DARWIN)

//=============================================================================

#include <OpenMesh/Core/System/config.hh>

#include <OpenMesh/Core/IO/reader/BaseReader.hh>
#include <OpenMesh/Core/IO/reader/OBJReader.hh>
#include <OpenMesh/Core/IO/reader/OFFReader.hh>
#include <OpenMesh/Core/IO/reader/STLReader.hh>
#include <OpenMesh/Core/IO/reader/OMReader.hh>

#include <OpenMesh/Core/IO/writer/BaseWriter.hh>
#include <OpenMesh/Core/IO/writer/OBJWriter.hh>
#include <OpenMesh/Core/IO/writer/OFFWriter.hh>
#include <OpenMesh/Core/IO/writer/STLWriter.hh>
#include <OpenMesh/Core/IO/writer/OMWriter.hh>


//=== NAMESPACES ==============================================================

namespace OpenMesh {
namespace IO {

//=============================================================================


// Instanciate every Reader module 
static BaseReader* OFFReaderInstance = &OFFReader();
static BaseReader* OBJReaderInstance = &OBJReader();
static BaseReader* STLReaderInstance = &STLReader();
static BaseReader* OMReaderInstance  = &OMReader();

// Instanciate every writer module
static BaseWriter* OBJWriterInstance = &OBJWriter();
static BaseWriter* OFFWriterInstance = &OFFWriter();
static BaseWriter* STLWriterInstance = &STLWriter();
static BaseWriter* OMWriterInstance  = &OMWriter();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif // static ?
#endif //__IOINSTANCES_HH__
//=============================================================================
