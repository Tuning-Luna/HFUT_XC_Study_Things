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
//  Implements an reader module for STL files
//
//=============================================================================


#ifndef __STLREADER_HH__
#define __STLREADER_HH__


//=== INCLUDES ================================================================


#include <stdio.h>
#include <string>

#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/reader/BaseReader.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {

//== FORWARDS =================================================================

class BaseImporter;

//== IMPLEMENTATION ===========================================================


/** 
    Implementation of the STL format reader. This class is singleton'ed by 
    SingletonT to STLReader.
*/
class _STLReader_ : public BaseReader
{
public:

  // constructor
  _STLReader_();


  std::string get_description() const 
  { return "Stereolithography Interface Format"; }
  std::string get_extensions() const { return "stl stla stlb"; }

  bool read(const std::string& _filename, 
	    BaseImporter& _bi,
            Options& _opt);


  /** Set the threshold to be used for considering two point to be equal.
      Can be used to merge small gaps */
  void set_epsilon(float _eps) { eps_=_eps; }

  /// Returns the threshold to be used for considering two point to be equal.
  float epsilon() const { return eps_; }


  
private:

  enum STL_Type { STLA, STLB, NONE };
  STL_Type check_stl_type(const std::string& _filename) const;

  bool read_stla(const std::string& _filename, BaseImporter& _bi) const;
  bool read_stlb(const std::string& _filename, BaseImporter& _bi) const;


private:

  float eps_;
};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the STL reader
extern _STLReader_  __STLReaderInstance;
_STLReader_&  STLReader();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
