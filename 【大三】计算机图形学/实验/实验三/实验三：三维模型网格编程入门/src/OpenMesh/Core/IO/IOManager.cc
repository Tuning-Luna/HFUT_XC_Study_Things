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
//  Implements the OpenMesh IOManager singleton
//
//=============================================================================


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.hh>
#include <OpenMesh/Core/IO/IOManager.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=============================================================================


_IOManager_  *__IOManager_instance = 0;


_IOManager_& IOManager()
{
  if (!__IOManager_instance)  __IOManager_instance = new _IOManager_();
  return *__IOManager_instance;
}


//-----------------------------------------------------------------------------


bool 
_IOManager_::
read(const std::string& _filename, BaseImporter& _bi, Options& _opt)
{      
  std::set<BaseReader*>::const_iterator it     =  reader_modules_.begin();
  std::set<BaseReader*>::const_iterator it_end =  reader_modules_.end();
    
  // Try all registered modules
  for(; it != it_end; ++it)
    if ((*it)->can_u_read(_filename))
    {
      _bi.prepare();
      bool ok = (*it)->read(_filename, _bi, _opt);
      _bi.finish();
      return ok;
    }
  
  // All modules failed to read
  return false;
}


//-----------------------------------------------------------------------------


bool
_IOManager_::
write(const std::string& _filename, BaseExporter& _be, Options _opt)
{
  std::set<BaseWriter*>::const_iterator it     = writer_modules_.begin();
  std::set<BaseWriter*>::const_iterator it_end = writer_modules_.end();
  
  if ( it == it_end )
  {
    omerr() << "[OpenMesh::IO::_IOManager_] No writing modules available!\n";
    return false;
  }

  // Try all registered modules
  for(; it != it_end; ++it)
  {
    if ((*it)->can_u_write(_filename))
    {
      return (*it)->write(_filename, _be, _opt); 
    }
  }
  
  // All modules failed to save
  return false;
}


//-----------------------------------------------------------------------------


bool 
_IOManager_::
can_read( const std::string& _format ) const
{
  std::set<BaseReader*>::const_iterator it     = reader_modules_.begin();
  std::set<BaseReader*>::const_iterator it_end = reader_modules_.end();
  std::string filename = "dummy." + _format;
  
  for(; it != it_end; ++it)
    if ((*it)->can_u_read(filename))
      return true;
  
  return false;
}


//-----------------------------------------------------------------------------


bool
_IOManager_::
can_write( const std::string& _format ) const
{
  std::set<BaseWriter*>::const_iterator it     = writer_modules_.begin();
  std::set<BaseWriter*>::const_iterator it_end = writer_modules_.end();
  std::string filename = "dummy." + _format;
    
  // Try all registered modules
  for(; it != it_end; ++it)
    if ((*it)->can_u_write(filename))
      return true;
    
  return false;
}


//-----------------------------------------------------------------------------


const BaseWriter*
_IOManager_::
find_writer(const std::string& _format)
{
  using std::string;
  
  string::size_type dot = _format.rfind('.');

  string ext;
  if (dot == string::npos)
    ext = _format;
  else
    ext = _format.substr(dot+1,_format.length()-(dot+1));
  
  std::set<BaseWriter*>::const_iterator it     = writer_modules_.begin();
  std::set<BaseWriter*>::const_iterator it_end = writer_modules_.end();
  std::string filename = "dummy." + ext;
  
  // Try all registered modules
  for(; it != it_end; ++it)
    if ((*it)->can_u_write(filename))
      return *it;

  return NULL;
}
  

//-----------------------------------------------------------------------------


void
_IOManager_::
update_read_filters()
{
  std::set<BaseReader*>::const_iterator it     = reader_modules_.begin(),
                                        it_end = reader_modules_.end();
  std::string s, all, filters;
  
  for(; it != it_end; ++it)
  {
    filters += (*it)->get_description() + " (";
    
    std::istringstream iss((*it)->get_extensions());
    while (iss && !iss.eof() && (iss >> s))
    { s = " *." + s; filters += s; all += s; }
    
    filters += " );;";
  }

  all = "All files ( " + all + " );;";
  
  read_filters_ = all + filters;
}


//-----------------------------------------------------------------------------


void
_IOManager_::
update_write_filters()
{
  std::set<BaseWriter*>::const_iterator it     = writer_modules_.begin(),
                                        it_end = writer_modules_.end();
  std::string s, all, filters;
    
  for(; it != it_end; ++it)
  {
    filters += (*it)->get_description() + " (";

    std::istringstream iss((*it)->get_extensions());
    while (iss && !iss.eof() && (iss >> s))
    { s = " *." + s; filters += s; all += s; }

    filters += " );;";
  }
  all = "All files ( " + all + " );;";

  write_filters_ = all + filters;
}  


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
