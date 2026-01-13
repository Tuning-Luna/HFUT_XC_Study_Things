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


#ifndef OPENMESH_UTILS_GLCONSTASSTRING_HH
#define OPENMESH_UTILS_GLCONSTASSTRING_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>
#include <GL/glut.h>


//== FORWARDDECLARATIONS ======================================================


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Utils {

//== CLASS DEFINITION =========================================================

inline
const char *GLenum_as_string( GLenum _m )
{
#define MODE(M) case M:return #M
  switch( _m )
  {
    MODE(GL_POINTS);
    MODE(GL_LINES);
    MODE(GL_LINE_STRIP);
    MODE(GL_LINE_LOOP);
    MODE(GL_TRIANGLES);
    MODE(GL_TRIANGLE_STRIP);
    MODE(GL_TRIANGLE_FAN);
    MODE(GL_QUADS);
    MODE(GL_QUAD_STRIP);
    MODE(GL_POLYGON);
    default: return "<unknown>";
  }
#undef MODE
}

//=============================================================================
} // namespace Utils
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_UTILS_GLCONSTASSTRING_HH defined
//=============================================================================

