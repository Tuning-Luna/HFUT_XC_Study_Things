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
//  CLASS ValenceViewer
//
//=============================================================================


#ifndef VALENCEVIEWERWIDGET_HH
#define VALENCEVIEWERWIDGET_HH


//== INCLUDES =================================================================


#include "MeshViewer.hh"



//== CLASS DEFINITION =========================================================



class ValenceViewer : public MeshViewer
{
public:

	/// default constructor
	ValenceViewer(const char* _title, int _width, int _height);

	// destructor
	~ValenceViewer();

	/// open mesh
	virtual bool open_mesh(const char* _filename);



protected:

	virtual void draw(const std::string& _draw_mode);


	/// calculate vertex valences
	void calc_valences();

	/// set vertex color from vertex valence
	void color_coding();


	// 定义顶点度数属性，用于存储每个顶点的度数
private:
	OpenMesh::VPropHandleT<int> valence_handle_;
};


//=============================================================================
#endif // VALENCEVIEWERWIDGET_HH defined
//=============================================================================

