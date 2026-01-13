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
//  CLASS mostream - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/omstream.hh>


//== IMPLEMENTATION ========================================================== 


OpenMesh::mostream& omlog() 
{
  static bool initialized = false;
  static OpenMesh::mostream mystream;
  if (!initialized)
  {
    mystream.connect(std::clog);
    initialized = true;
#ifdef NDEBUG
    mystream.disable();
#endif
  }
  return mystream;
}


OpenMesh::mostream& omout() 
{
  static bool initialized = false;
  static OpenMesh::mostream mystream;
  if (!initialized) 
  {
    mystream.connect(std::cout);
    initialized = true;
  }
  return mystream;
}


OpenMesh::mostream& omerr() 
{
  static bool initialized = false;
  static OpenMesh::mostream mystream;
  if (!initialized)
  {
    mystream.connect(std::cerr);
    initialized = true;
  }
  return mystream;
}


//=============================================================================
