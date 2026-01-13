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
//  CLASS GlutExaminer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================


#include "GlutExaminer.hh"
#include "gl.hh"
#include <OpenMesh/Tools/Utils/Timer.hh>


//== IMPLEMENTATION ========================================================== 


GlutExaminer::
GlutExaminer(const char* _title, int _width, int _height)
  : GlutViewer(_title, _width, _height)
{
  init();

  // init mouse buttons
  for (int i=0; i<10; ++i)
    button_down_[i] = false;

  // draw mode
  add_draw_mode("Wireframe");
  add_draw_mode("Hidden Line");
  add_draw_mode("Solid Flat");
  add_draw_mode("Solid Smooth");
  set_draw_mode(1);
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::init()
{
  // OpenGL state
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glDisable( GL_DITHER );
  glEnable( GL_DEPTH_TEST );


  // some performance settings
//   glEnable( GL_CULL_FACE );
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );


  // material
  GLfloat mat_a[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat mat_d[] = {0.4, 0.4, 0.4, 1.0};
  GLfloat mat_s[] = {0.8, 0.8, 0.8, 1.0};
  GLfloat shine[] = {128.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_a);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_d);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_s);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  
  // lighting
  glLoadIdentity();
  
  GLfloat pos1[] = { 0.1, 0.1, -0.02, 0.0};
  GLfloat pos2[] = {-0.1, 0.1, -0.02, 0.0};
  GLfloat pos3[] = { 0.0, 0.0, 0.1, 0.0};
  GLfloat col1[] = {.05, .05, .6, 1.0};
  GLfloat col2[] = {.6, .05, .05, 1.0};
  GLfloat col3[] = {1.0, 1.0, 1.0, 1.0};

  glEnable(GL_LIGHT0);    
  glLightfv(GL_LIGHT0,GL_POSITION, pos1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,  col1);
  glLightfv(GL_LIGHT0,GL_SPECULAR, col1);
  
  glEnable(GL_LIGHT1);  
  glLightfv(GL_LIGHT1,GL_POSITION, pos2);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,  col2);
  glLightfv(GL_LIGHT1,GL_SPECULAR, col2);
  
  glEnable(GL_LIGHT2);  
  glLightfv(GL_LIGHT2,GL_POSITION, pos3);
  glLightfv(GL_LIGHT2,GL_DIFFUSE,  col3);
  glLightfv(GL_LIGHT2,GL_SPECULAR, col3);
  

  // scene pos and size
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
  set_scene(Vec3f(0.0, 0.0, 0.0), 1.0);


  // projection
  near_ = 0.1;
  far_  = 100.0;
  fovy_ = 45.0;
}

 
//-----------------------------------------------------------------------------

void
GlutExaminer::reshape(int _w, int _h)
{
  width_  = _w; 
  height_ = _h;
  glViewport(0, 0, _w, _h);
  update_projection_matrix();
  glutPostRedisplay();
}


//----------------------------------------------------------------------------


void
GlutExaminer::update_projection_matrix()
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective(fovy_, (GLfloat)width_/(GLfloat)height_, near_, far_);
  glGetDoublev( GL_PROJECTION_MATRIX, projection_matrix_);
  glMatrixMode( GL_MODELVIEW );
}


//----------------------------------------------------------------------------


void
GlutExaminer::set_scene( const Vec3f& _cog, float _radius )
{
  center_ = _cog;
  radius_ = _radius;

  near_  = 0.01 * radius_;
  far_   = 10.0 * radius_;
  update_projection_matrix();

  view_all();
}


//----------------------------------------------------------------------------


void
GlutExaminer::view_all()
{  
  translate( Vec3f( -(modelview_matrix_[0]*center_[0] + 
		      modelview_matrix_[4]*center_[1] +
		      modelview_matrix_[8]*center_[2] + 
		      modelview_matrix_[12]),
		    -(modelview_matrix_[1]*center_[0] + 
		      modelview_matrix_[5]*center_[1] +
		      modelview_matrix_[9]*center_[2] + 
		      modelview_matrix_[13]),
		    -(modelview_matrix_[2]*center_[0] + 
		      modelview_matrix_[6]*center_[1] +
		      modelview_matrix_[10]*center_[2] + 
		      modelview_matrix_[14] +
		      3.0*radius_) ) );
}


//----------------------------------------------------------------------------


bool
GlutExaminer::map_to_sphere( const Vec2i& _v2D, Vec3f& _v3D )
{
  if ( (_v2D[0] >= 0) && (_v2D[0] <= width_) &&
       (_v2D[1] >= 0) && (_v2D[1] <= height_) ) 
  {
    double x  = (double)(_v2D[0] - 0.5*width_)  / (double)width_;
    double y  = (double)(0.5*height_ - _v2D[1]) / (double)height_;
    double sinx         = sin(M_PI * x * 0.5);
    double siny         = sin(M_PI * y * 0.5);
    double sinx2siny2   = sinx * sinx + siny * siny;
    
    _v3D[0] = sinx;
    _v3D[1] = siny;
    _v3D[2] = sinx2siny2 < 1.0 ? sqrt(1.0 - sinx2siny2) : 0.0;
    
    return true;
  }
  else return false;
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::draw(const std::string& _draw_mode)
{
  if (_draw_mode == "Wireframe")
  {
    glDisable(GL_LIGHTING);
    glutWireTeapot(0.5);
  }

  else if (_draw_mode == "Solid Flat")
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glutSolidTeapot(0.5);
  }

  else if (_draw_mode == "Solid Smooth")
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glutSolidTeapot(0.5);
  }
  else if (_draw_mode == "Hidden Line")
  {
	  glDisable(GL_LIGHTING);
	  glShadeModel(GL_SMOOTH);
	  glColor3f(0.0, 0.0, 0.0);

	  glDepthRange(0.01, 1.0);
	  glutSolidTeapot(0.5);

	  glColor3f(1.0, 1.0, 1.0);
	  glDepthRange(0.0, 1.0);
	  glutWireTeapot(0.5);
  } 
  else 
  {
	  std::cout << "This view mode is not supported for this geometry, you need to load a mesh!" << std::endl;
  }
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::mouse(int button, int state, int x, int y)
{
  // mouse press
  if (state == GLUT_DOWN)
  {
    last_point_2D_ = Vec2i(x,y);
    last_point_ok_ = map_to_sphere( last_point_2D_, last_point_3D_ );
    button_down_[button] = true;
  }


  // mouse release
  else
  {
    last_point_ok_ = false;
    button_down_[button] = false;

    // GLUT: button 3 or 4 -> mouse wheel clicked
    if (button == 3)       
      zoom(0, (int)(last_point_2D_[1] - 0.05*width_));
    else if (button == 4)
      zoom(0, (int)(last_point_2D_[1] + 0.05*width_));
  }


  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::motion(int x, int y)
{
  // zoom
  if (button_down_[0] && button_down_[1])
  {
    zoom(x, y);
  }

  // rotation
  else if (button_down_[0])
  {
    rotation(x, y);
  }

  // translation
  else if (button_down_[1])
  {
    translation(x, y);
  }


  // remeber points
  last_point_2D_ = Vec2i(x, y);
  last_point_ok_ = map_to_sphere(last_point_2D_, last_point_3D_);

  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::rotation(int x, int y)
{
  if (last_point_ok_) 
  {
    Vec2i  new_point_2D;
    Vec3f  new_point_3D;
    bool   new_point_ok;

    new_point_2D = Vec2i(x, y);
    new_point_ok = map_to_sphere(new_point_2D, new_point_3D);
    
    if (new_point_ok)
    {
      Vec3f axis      = (last_point_3D_ % new_point_3D);
      float cos_angle = (last_point_3D_ | new_point_3D);

      if (fabs(cos_angle) < 1.0) 
      {
	float angle = 2.0*acos(cos_angle) * 180.0 / M_PI;
	rotate(axis, angle);
      }
    }
  }
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::translation(int x, int y)
{
  float dx = x - last_point_2D_[0];
  float dy = y - last_point_2D_[1];

  float z = - ((modelview_matrix_[ 2]*center_[0] + 
		modelview_matrix_[ 6]*center_[1] + 
		modelview_matrix_[10]*center_[2] + 
		modelview_matrix_[14]) /
	       (modelview_matrix_[ 3]*center_[0] + 
		modelview_matrix_[ 7]*center_[1] + 
		modelview_matrix_[11]*center_[2] + 
		modelview_matrix_[15]));

  float aspect = (float)width_ / (float)height_;
  float up     = tan(fovy_/2.0f*M_PI/180.f) * near_;
  float right  = aspect*up;

  translate(Vec3f(2.0*dx/width_*right/near_*z, 
		  -2.0*dy/height_*up/near_*z, 
		  0.0f));
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::zoom(int x, int y)
{
  float dy = y - last_point_2D_[1];
  float h  = height_;
  translate(Vec3f(0.0, 0.0, radius_ * dy * 3.0 / h));
}


//----------------------------------------------------------------------------


void
GlutExaminer::translate( const Vec3f& _trans )
{
  glLoadIdentity();
  glTranslated( _trans[0], _trans[1], _trans[2] );
  glMultMatrixd( modelview_matrix_ );
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview_matrix_);
}


//----------------------------------------------------------------------------


void
GlutExaminer::rotate( const Vec3f& _axis, float _angle )
{
  Vec3f t( modelview_matrix_[0]*center_[0] + 
	   modelview_matrix_[4]*center_[1] +
	   modelview_matrix_[8]*center_[2] + 
	   modelview_matrix_[12],
	   modelview_matrix_[1]*center_[0] + 
	   modelview_matrix_[5]*center_[1] +
	   modelview_matrix_[9]*center_[2] + 
	   modelview_matrix_[13],
	   modelview_matrix_[2]*center_[0] + 
	   modelview_matrix_[6]*center_[1] +
	   modelview_matrix_[10]*center_[2] + 
	   modelview_matrix_[14] );
  
  glLoadIdentity();
  glTranslatef(t[0], t[1], t[2]);
  glRotated( _angle, _axis[0], _axis[1], _axis[2]);
  glTranslatef(-t[0], -t[1], -t[2]); 
  glMultMatrixd(modelview_matrix_);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::keyboard(int key, int x, int y) 
{
  switch (key)
  {
    case 'f':
    {
      std::cerr << "Performance test: ";
      double fps = measure_fps();
      std::cerr << fps << " FPS\n";
      break;
    }

    default:
    {
      GlutViewer::keyboard(key, x, y);
      break;
    }
  }
}


//-----------------------------------------------------------------------------


double 
GlutExaminer::measure_fps()
{
  double fps(0.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  unsigned int  frames = 90;
  const float   angle  = 360.0/(float)frames;
  unsigned int  i;
  Vec3f         axis;

  OpenMesh::Utils::Timer timer; timer.start();
	
  for (i=0, axis=Vec3f(1,0,0); i<frames; ++i)
  { rotate(axis, angle); display(); }
  for (i=0, axis=Vec3f(0,1,0); i<frames; ++i)
  { rotate(axis, angle); display(); }
  for (i=0, axis=Vec3f(0,0,1); i<frames; ++i)
  { rotate(axis, angle); display(); }

  glFinish();

  timer.stop();
  fps = (1000.0 / timer.mseconds() * (3.0 * frames));

  glPopMatrix();
  glutPostRedisplay();

  return fps;
}


//=============================================================================
