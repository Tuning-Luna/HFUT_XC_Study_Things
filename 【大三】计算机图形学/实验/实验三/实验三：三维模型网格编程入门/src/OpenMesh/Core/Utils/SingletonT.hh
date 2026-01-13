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
//  Implements a simple singleton template
//
//=============================================================================


#ifndef __SINGLETON_HH__
#define __SINGLETON_HH__


//=== INCLUDES ================================================================

// OpenMesh
#include <OpenMesh/Core/System/config.hh>

// STL
#include <stdexcept>
#include <iostream>


//== NAMESPACES ===============================================================


namespace OpenMesh {


//=== IMPLEMENTATION ==========================================================


/** A simple singleton template.
    Encapsulates an arbitrary class and enforces its uniqueness.
*/

template <typename T>
class SingletonT
{
public:

  /** Singleton access function.
      Use this function to obtain a reference to the instance of the 
      encapsulated class. Note that this instance is unique and created
      on the first call to Instance().
  */

  static T& Instance()
  {
    if (!pInstance__)
    {
      // check if singleton alive
      if (destroyed__)
      {
	OnDeadReference();
      }
      // first time request -> initialize
      else
      {
	Create();
      }
    }
    return *pInstance__;
  }


private:

  // Disable constructors/assignment to enforce uniqueness
  SingletonT();
  SingletonT(const SingletonT&);
  SingletonT& operator=(const SingletonT&);

  // Create a new singleton and store its pointer
  static void Create()
  {
    static T theInstance;
    pInstance__ = &theInstance;
  }
  
  // Will be called if instance is accessed after its lifetime has expired
  static void OnDeadReference()
  {
    throw std::runtime_error("[Singelton error] - Dead reference detected!\n");
  }

  virtual ~SingletonT()
  {
    pInstance__ = 0;
    destroyed__ = true;
  }
  
  static T*     pInstance__;
  static bool   destroyed__;
};



//=============================================================================
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_SINGLETON_C)
#  define OPENMESH_SINGLETON_TEMPLATES
#  include "SingletonT.cc"
#endif
//=============================================================================
#endif // __SINGLETON_HH__
//=============================================================================
