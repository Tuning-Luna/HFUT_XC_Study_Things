//=============================================================================
//                                                
//   Code framework for the lecture
//
//   "Surface Representation and Geometric Modeling"
//
//   Mark Pauly, Mario Botsch, Balint Miklos, and Hao Li
//
//   Copyright (C) 2007 by  Applied Geometry Group and 
//							Computer Graphics Laboratory, ETH Zurich
//                                                                         
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, 
//   Boston, MA  02110-1301, USA.
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS GlutViewer
//
//=============================================================================


#ifndef GLUTVIEWER_HH
#define GLUTVIEWER_HH


//== INCLUDES =================================================================

#include "gl.hh"
#include <map>
#include <vector>
#include <string>


//== CLASS DEFINITION =========================================================

	      

/** \class GlutViewer GlutViewer.hh
    Simple Glut viewer. 
    Based on C++ glut interface of George Stetten and Korin Crawford.
**/

class GlutViewer
{
public:
   
  GlutViewer(const char* _title, int _width, int _height);
  virtual ~GlutViewer();



protected:

  virtual void draw(const std::string& _drawmode) = 0;
  void clear_draw_modes();
  unsigned int add_draw_mode(const std::string& _s);
  void set_draw_mode(int _id);  


  virtual void display(void);
  virtual void idle(void); 
  virtual void keyboard(int key, int x, int y);
  virtual void motion(int x, int y);
  virtual void mouse(int button, int state, int x, int y);
  virtual void passivemotion(int x, int y);
  virtual void reshape(int w, int h); 
  virtual void visibility(int visible);
  virtual void processmenu(int i);

  int  width_, height_;


private:

  static void display__(void);
  static void idle__(void); 
  static void keyboard__(unsigned char key, int x, int y);
  static void motion__(int x, int y);
  static void mouse__(int button, int state, int x, int y);
  static void passivemotion__(int x, int y);
  static void reshape__(int w, int h); 
  static void special__(int key, int x, int y);   
  static void visibility__(int visible);
  static void processmenu__(int i);

  static std::map<int, GlutViewer*>  windows__;
  static GlutViewer* current_window();

  

private:

  int  windowID_, menuID_; 

  bool fullscreen_;
  int  bak_left_, bak_top_, bak_width_, bak_height_;

  unsigned int              draw_mode_;
  unsigned int              n_draw_modes_;
  std::vector<std::string>  draw_mode_names_;
};


//=============================================================================
#endif // ACG_GLUTVIEWER_HH defined
//=============================================================================

