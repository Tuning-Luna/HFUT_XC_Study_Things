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
//  CLASS Progress - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Utils/Progress.hh>


//== IMPLEMENTATION ========================================================== 


ProgressManager&  Progress()
{
  static ProgressManager  pm;
  return pm;
}


//-----------------------------------------------------------------------------


void
ProgressManager::
set_steps(unsigned int _steps, const char* _label)
{
  std::set<ProgressHandler*>::iterator it, end(handlers_.end());
  for (it=handlers_.begin(); it!=end; ++it)
    (*it)->set_steps(_steps, _label);

  steps_      = _steps ? _steps : 1;
  progress_   = 0;
  percentage_ = 0;

  active_    = true;
  cancelled_ = false;

  timer_.start();
}


//-----------------------------------------------------------------------------


bool
ProgressManager::
step(unsigned int _steps)
{
  return set_progress(progress_ + _steps);
}


//-----------------------------------------------------------------------------


bool
ProgressManager::
set_progress(unsigned int _progress)
{
  // get current progress percentage
  progress_  = _progress;
  unsigned int p = (progress_*100) / steps_;


  // time ellapsed till last update
  timer_.stop();
  double secs = timer_.seconds();
  timer_.cont();


  // update if:
  //   percentage increased by at least 1%
  //   one second ellapsed till last update
  if (p > percentage_ || secs > 1.0)
  {
    percentage_ = p;

    std::set<ProgressHandler*>::iterator it, end(handlers_.end());
    for (it=handlers_.begin(); it!=end; ++it)
      if (!(*it)->set_progress(progress_))
      {
	cancelled_ = true;
	break;
      }

    // reset timer
    timer_.start();
  }


  return !cancelled_;
}


//-----------------------------------------------------------------------------


void
ProgressManager::
finish()
{
  std::set<ProgressHandler*>::iterator it, end(handlers_.end());
  for (it=handlers_.begin(); it!=end; ++it)
    (*it)->finish();

  active_ = false;
}


//=============================================================================
