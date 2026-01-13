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
//   $Date: 2004/09/06 12:37:16 $
//                                                                            
//=============================================================================

/** \file Subdivider/Adaptive/Composite/CompositeTraits.hh
    Mesh traits for adaptive composite subdivider.
 */

//=============================================================================
//
//  CLASS Traits
//
//=============================================================================

#ifndef OPENMESH_SUBDIVIDER_ADAPTIVE_COMPOSITETRAITS_HH
#define OPENMESH_SUBDIVIDER_ADAPTIVE_COMPOSITETRAITS_HH


//== INCLUDES =================================================================

#include <map>
#include <OpenMesh/Core/Mesh/Traits.hh>

//== NAMESPACE ================================================================

namespace OpenMesh   { // BEGIN_NS_OPENMESH
namespace Subdivider { // BEGIN_NS_DECIMATER
namespace Adaptive   { // BEGIN_NS_UNIFORM


//== CLASS DEFINITION =========================================================

/** Adaptive Composite Subdivision framework.
*/

// typedef unsigned short state_t;
// const state_t mask_final = 1 << ((sizeof(state_t)*8)-1);
// const state_t mask_state = ~mask_final;

/** Mesh traits for adaptive composite subdivision
 */
struct CompositeTraits : public OpenMesh::DefaultTraits
{
  typedef int  state_t; ///< External representation for intermediate state
  typedef bool final_t; ///< External representation for final flag
  
  
  /// Storage type for intermediate states and the final flag of a mesh entity.
  struct State
  {
    int      state : 31;
    unsigned final : 1;
  };  

  // ---------------------------------------- attributes

  // add face normals
  FaceAttributes( OpenMesh::Attributes::Normal );
  
  // add vertex normals
  VertexAttributes( OpenMesh::Attributes::Normal );
  
  // add previous halfedge handle
  HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

  // ---------------------------------------- items

  FaceTraits
  {
    
  private:
    
    typedef typename Refs::Point Point;
    typedef typename Refs::HalfedgeHandle HalfedgeHandle;
    typedef std::map<state_t, Point> PositionHistory;
    
    State                state_;
    HalfedgeHandle       red_halfedge_;
    
    PositionHistory      pos_map_;
    
  public:
    
    // face state
    state_t state() const { return state_t(state_.state); }
    void    set_state(const state_t _s) { state_.state = _s; }
    void    inc_state() { ++state_.state; }
    
    // face not final if divided (loop) or edge not flipped (sqrt(3))
    final_t final() const   { return final_t(state_.final); }
    void    set_final()     { state_.final = true; }
    void    set_not_final() { state_.final = false; }
    
    // halfedge of dividing edge (red-green triangulation)
    const HalfedgeHandle& red_halfedge() const { return red_halfedge_; }
    void  set_red_halfedge(const HalfedgeHandle& _h) { red_halfedge_ = _h; }
    
    // position of face, depending on generation _i. 
    void set_position(const int& _i, const Point& _p) { pos_map_[_i] = _p; }
    const Point position(const int& _i) { 
      if (pos_map_.find(_i) != pos_map_.end())
	return pos_map_[_i];
      else {
	
	if (_i <= 0) {
	  return Point(0.0, 0.0, 0.0);
	}
        
	return position(_i - 1);
      }
    }
  }; // end class FaceTraits
  
  
  EdgeTraits
  {
    
  private:
    
    typedef typename Refs::Point Point;
    typedef std::map<state_t, Point> PositionHistory;
    
    State           state_;
    PositionHistory pos_map_;
    
  public: 
    
    typedef typename Refs::Scalar Scalar;
    
    // Scalar weight_;
    
    // state of edge
    state_t state() const { return state_t(state_.state); }
    void    set_state(const state_t _s) { state_.state = _s; }
    void    inc_state() { ++state_.state; }
    
    // edge not final if dividing face (Loop) or edge not flipped (SQRT(3))
    final_t final() const   { return final_t(state_.final); }
    void    set_final()     { state_.final = true; }
    void    set_not_final() { state_.final = false; }
    
    // position of edge, depending on generation _i. 
    void set_position(const int& _i, const Point& _p) { pos_map_[_i] = _p; }
    
    const Point position(const int& _i) {
        
      if (pos_map_.find(_i) != pos_map_.end()) 
        return pos_map_[_i];
      else 
      {
	if (_i <= 0) 
	{
	  const Point zero_point(0.0, 0.0, 0.0);
	  return zero_point;
	}
        
	return position(_i - 1);
      }
    }
  }; // end class EdgeTraits


  VertexTraits
  {

  private:

    typedef typename Refs::Point Point;
    typedef std::map<state_t, Point> PositionHistory;

    State           state_;
    PositionHistory pos_map_;

  public:

    // state of vertex
    state_t state() const { return state_.state; }
    void    set_state(const state_t _s) { state_.state = _s; }
    void    inc_state() { ++state_.state; }


    // usually not needed by loop or sqrt(3)
    final_t final() const   { return state_.final; }
    void    set_final()     { state_.final = true; }
    void    set_not_final() { state_.final = false; }

    // position of vertex, depending on generation _i. (not for display)
    void set_position(const int& _i, const Point& _p) { pos_map_[_i] = _p; }
    const Point position(const int& _i) { 

      if (pos_map_.find(_i) != pos_map_.end()) 

	return pos_map_[_i];

      else {

	if (_i <= 0) {

	  const Point zero_point(0.0, 0.0, 0.0);
	  return zero_point;
	}

	return position(_i - 1);
      }
    }
  }; // end class VertexTraits
}; // end class CompositeTraits


// export items to namespace to maintain compatibility
typedef CompositeTraits::state_t state_t;
typedef CompositeTraits::final_t final_t;
typedef CompositeTraits::State   State;

//=============================================================================
} // END_NS_ADAPTIVE
} // END_NS_SUBDIVIDER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENMESH_SUBDIVIDER_ADAPTIVE_COMPOSITETRAITS_HH defined
//=============================================================================
