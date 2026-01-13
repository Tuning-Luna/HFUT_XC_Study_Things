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
//  CLASS newClass
//
//=============================================================================


#ifndef OPENMESH_VDPM_STREAMINGDEF_HH
#define OPENMESH_VDPM_STREAMINGDEF_HH

//== INCLUDES =================================================================

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {


//== CLASS DEFINITION =========================================================

#define VDPM_STREAMING_PORT 4096

#define DEBUG_COUT
//#define QDATASTREAM

#ifdef DEBUG_COUT
static bool debug_print_;
static bool debug_print()               { return debug_print_; }
static void set_debug_print(bool flag)  { debug_print_ = flag; }
#endif

enum VDPMDownLinkStatus     { kStarted, kFinished, kStartable };
enum VDPMStreamingPhase     { kBaseMesh, kVSplitHeader, kVSplits };
enum VDPMClientMode         { kStatic, kDynamic };
enum VHierarchySearchMode   { kBruteForce, kUseHashing };


//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_VDPROGMESH_VDPMSTREAMINGDEF_HH defined
//=============================================================================


