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
//   $Date: 2004/09/06 12:35:33 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Utils for generic/generative programming
//
//=============================================================================

#ifndef OPENMESH_GENPROG_HH
#define OPENMESH_GENPROG_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace GenProg  {
#ifndef DOXY_IGNORE_THIS

//== IMPLEMENTATION ===========================================================


/// This type maps \c true or \c false to different types.
template <bool b> class Bool2Type { enum { my_bool = b }; };

/// This class generates different types from different \c int 's.
template <int i>  class Int2Type  { enum { my_int = i }; };

/// Handy typedef for Bool2Type<true> classes
typedef Bool2Type<true> True;

/// Handy typedef for Bool2Type<false> classes
typedef Bool2Type<false> False;


//-----------------------------------------------------------------------------



//--- Template "if" w/ partial specialization ---------------------------------
#if OM_PARTIAL_SPECIALIZATION


template <bool condition, class Then, class Else>
struct IF { typedef Then Result; };

/** Template \c IF w/ partial specialization
\code
typedef IF<bool, Then, Else>::Result  ResultType;
\endcode    
*/
template <class Then, class Else>
struct IF<false, Then, Else> { typedef Else Result; };





//--- Template "if" w/o partial specialization --------------------------------
#else


struct SelectThen 
{
  template <class Then, class Else> struct Select {
    typedef Then Result;
  };
};

struct SelectElse
{
  template <class Then, class Else> struct Select {
    typedef Else Result;
  };
};

template <bool condition> struct ChooseSelector {
  typedef SelectThen Result;
};

template <> struct ChooseSelector<false> {
  typedef SelectElse Result;
};


/** Template \c IF w/o partial specialization. Use it like
\code
typedef IF<bool, Then, Else>::Result  ResultType;
\endcode    
*/

template <bool condition, class Then, class Else>
class IF 
{ 
  typedef typename ChooseSelector<condition>::Result  Selector;
public:
  typedef typename Selector::template Select<Then, Else>::Result  Result;
};

#endif

//=============================================================================
#endif
} // namespace GenProg
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_GENPROG_HH defined
//=============================================================================
