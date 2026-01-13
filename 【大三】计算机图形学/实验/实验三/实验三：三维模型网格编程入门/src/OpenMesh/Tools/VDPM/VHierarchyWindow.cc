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
//  CLASS newClass - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <OpenMesh/Tools/VDPM/VHierarchyWindow.hh>

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace VDPM {


//== IMPLEMENTATION ========================================================== 


VHierarchyWindow::
VHierarchyWindow()
{
  vhierarchy_  = NULL;
  buffer_      = NULL;

  buffer_min_  = 0;
  buffer_max_  = 0;

  window_min_  = 0;
  window_max_  = 0;
  current_pos_ = 0;

  n_shift_     = 0;
}


VHierarchyWindow::
VHierarchyWindow(VHierarchy &_vhierarchy)
{
  vhierarchy_ = &_vhierarchy;
  buffer_     = NULL;
}


VHierarchyWindow::
~VHierarchyWindow(void)
{
  if (buffer_ != NULL)
    free(buffer_);
}


bool
VHierarchyWindow::
update_buffer(VHierarchyNodeHandle _node_handle)
{
  if (underflow(_node_handle) != true && overflow(_node_handle) != true)
    return  false;

  // tightly update window_min_ & window_max_
  int none_zero_pos;
  for (none_zero_pos=buffer_size()-1; none_zero_pos >= 0; --none_zero_pos)
  {
    if (buffer_[none_zero_pos] != 0)  break;
  }
  window_max_ = buffer_min_ + none_zero_pos + 1;
  for(none_zero_pos=0; none_zero_pos < buffer_size(); ++none_zero_pos)
  {
    if (buffer_[none_zero_pos] != 0)  break;
  }
  window_min_ = buffer_min_ + none_zero_pos;  
  
  assert(window_min_ < window_max_);
  
  while (underflow(_node_handle) == true)   buffer_min_ /= 2;
  while (overflow(_node_handle) == true)
  {
    buffer_max_ *= 2;
    if (buffer_max_ > (int) vhierarchy_->num_nodes() / 8)
      buffer_max_ = (int) (1 + vhierarchy_->num_nodes() / 8);
  }
  
  unsigned char *new_buffer = (unsigned char *) malloc(buffer_size());
  memset(new_buffer, 0, buffer_size());
  memcpy(&(new_buffer[window_min_-buffer_min_]), 
	 &(buffer_[none_zero_pos]), 
	 window_size());
  free(buffer_);
  buffer_ = new_buffer;

  return  true;
}

void
VHierarchyWindow::init(VHierarchyNodeHandleContainer &_roots)
{
  if (buffer_ != NULL)
    free(buffer_);

  buffer_min_ = 0;
  buffer_max_ = _roots.size() / 8;
  if (_roots.size() % 8 > 0)
    ++buffer_max_;

  buffer_ = (unsigned char *) malloc(buffer_size());
  memset(buffer_, 0, buffer_size());

  window_min_ = 0;
  window_max_= 0;
  current_pos_ = 0;
  n_shift_ = 0;
  
  for (unsigned int i=0; i<_roots.size(); i++)
  {
    activate(VHierarchyNodeHandle((int) i));
  }
}


void
VHierarchyWindow::
update_with_vsplit(VHierarchyNodeHandle _parent_handle)
{
  VHierarchyNodeHandle  
    lchild_handle = vhierarchy_->lchild_handle(_parent_handle),
    rchild_handle = vhierarchy_->rchild_handle(_parent_handle);

  assert(is_active(_parent_handle) == true);
  assert(is_active(lchild_handle) != true);
  assert(is_active(rchild_handle) != true);

  inactivate(_parent_handle);
  activate(rchild_handle);
  activate(lchild_handle);
}

void
VHierarchyWindow::
update_with_ecol(VHierarchyNodeHandle _parent_handle)
{
  VHierarchyNodeHandle 
    lchild_handle = vhierarchy_->lchild_handle(_parent_handle),
    rchild_handle = vhierarchy_->rchild_handle(_parent_handle);

  assert(is_active(_parent_handle) != true);
  assert(is_active(lchild_handle) == true);
  assert(is_active(rchild_handle) == true);
  
  activate(_parent_handle);
  inactivate(rchild_handle);
  inactivate(lchild_handle);
}

//=============================================================================
} // namespace VDPM
} // namespace OpenMesh
//=============================================================================
