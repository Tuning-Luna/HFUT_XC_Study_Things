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

#ifndef OPENMESH_PROGRESS_HH
#define OPENMESH_PROGRESS_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Utils/Timer.hh>
#include <set>


//== FORWARDDECLARATIONS ======================================================

class ProgressHandler;


//== CLASS DEFINITION =========================================================

	      
class ProgressManager
{
public:
   
  ProgressManager()  {}
  ~ProgressManager() {}

  void connect(ProgressHandler* _ph)     { handlers_.insert(_ph); }
  void disconnect(ProgressHandler* _ph)  { handlers_.erase(_ph); }

  void set_steps(unsigned int _nsteps, const char* _label);
  bool step(unsigned int _steps=1);
  bool set_progress(unsigned int _progress);
  void finish();

  bool is_active() const { return active_; }
  bool was_cancelled() const { return cancelled_; }


private:

  std::set<ProgressHandler*>  handlers_;
  unsigned int                steps_, progress_, percentage_;
  bool                        active_, cancelled_;
  OpenMesh::Utils::Timer      timer_;
};


ProgressManager&  Progress();



//== CLASS DEFINITION =========================================================


class ProgressHandler
{
public:

  ProgressHandler() { Progress().connect(this); }
  virtual ~ProgressHandler() { Progress().disconnect(this); }

  virtual void set_steps(unsigned int _steps, const char* _label) = 0;
  virtual bool set_progress(unsigned int _progress) = 0;
  virtual void finish() = 0;
};


//=============================================================================
#endif // OPENMESH_PROGRESS_HH defined
//=============================================================================

