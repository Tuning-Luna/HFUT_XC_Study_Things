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
//   $Date: 2004/09/06 12:34:56 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  CLASS Status
//
//=============================================================================


#ifndef OPENMESH_ATTRIBUTE_STATUS_HH
#define OPENMESH_ATTRIBUTE_STATUS_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Attributes {

 
//== CLASS DEFINITION  ========================================================
  

/** Status bits used by the Status class. 
 *  \see OpenMesh::Attributes::StatusInfo
 */
enum StatusBits {

  DELETED  = 1,   ///< Item has been deleted
  LOCKED   = 2,   ///< Item is locked.
  SELECTED = 4,   ///< Item is selected.
  HIDDEN   = 8,   ///< Item is hidden.
  FEATURE  = 16,  ///< Item is a feature or belongs to a feature.
  TAGGED   = 32,  ///< Item is tagged.
  TAGGED2  = 64,  ///< Alternate bit for tagging an item.
  UNUSED   = 128  ///<
};


/** \class StatusInfo Status.hh <OpenMesh/Attributes/Status.hh>
 *
 *   Add status information to a base class.
 *
 *   \see StatusBits
 */
class StatusInfo
{
public:

  typedef unsigned int value_type;
    
  StatusInfo() : status_(0) {}

  /// is deleted ?
  bool deleted() const  { return is_bit_set(DELETED); }
  /// set deleted
  void set_deleted(bool _b) { change_bit(DELETED, _b); }


  /// is locked ?
  bool locked() const  { return is_bit_set(LOCKED); }
  /// set locked
  void set_locked(bool _b) { change_bit(LOCKED, _b); }


  /// is selected ?
  bool selected() const  { return is_bit_set(SELECTED); }
  /// set selected
  void set_selected(bool _b) { change_bit(SELECTED, _b); }


  /// is hidden ?
  bool hidden() const  { return is_bit_set(HIDDEN); }
  /// set hidden
  void set_hidden(bool _b) { change_bit(HIDDEN, _b); }


  /// is feature ?
  bool feature() const  { return is_bit_set(FEATURE); }
  /// set feature
  void set_feature(bool _b) { change_bit(FEATURE, _b); }


  /// is tagged ?
  bool tagged() const  { return is_bit_set(TAGGED); }
  /// set tagged
  void set_tagged(bool _b) { change_bit(TAGGED, _b); }


  /// is tagged2 ? This is just one more tag info.
  bool tagged2() const  { return is_bit_set(TAGGED2); }
  /// set tagged
  void set_tagged2(bool _b) { change_bit(TAGGED2, _b); }


  /// return whole status
  unsigned int bits() const { return status_; }
  /// set whole status at once
  void set_bits(unsigned int _bits) { status_ = _bits; }


  /// is a certain bit set ?
  bool is_bit_set(unsigned int _s) const { return (status_ & _s) > 0; }
  /// set a certain bit
  void set_bit(unsigned int _s) { status_ |= _s; }
  /// unset a certain bit
  void unset_bit(unsigned int _s) { status_ &= ~_s; }
  /// set or unset a certain bit
  void change_bit(unsigned int _s, bool _b) {  
    if (_b) status_ |= _s; else status_ &= ~_s; }


private: 

  value_type status_;
};


//=============================================================================
} // namespace Attributes
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_ATTRIBUTE_STATUS_HH defined
//=============================================================================
