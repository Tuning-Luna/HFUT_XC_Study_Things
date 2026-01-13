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
//  OpenMesh streams: omlog, omout, omerr
//
//=============================================================================

#ifndef OPENMESH_OMSTREAMS_HH
#define OPENMESH_OMSTREAMS_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/mostream.hh>


//== CLASS DEFINITION =========================================================

/** \file omstream.hh
    This file provides the streams omlog, omout, and omerr.
*/

//@{
/** These stream provide replacements for clog, cout, and cerr. They have
    the advantage that they can easily be multiplexed.
    \see OpenMesh::mostream
*/

OpenMesh::mostream& omlog();
OpenMesh::mostream& omout();
OpenMesh::mostream& omerr();

//@}

//=============================================================================
#endif // OPENMESH_OMSTREAMS_HH defined
//=============================================================================
